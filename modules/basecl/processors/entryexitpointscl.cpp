/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#include "entryexitpointscl.h"
#include <inviwo/core/interaction/trackball.h>
#include <inviwo/core/datastructures/geometry/mesh.h>
#include <modules/opencl/image/imagecl.h>
#include <modules/opencl/image/imageclgl.h>
#include <modules/opencl/kernelmanager.h>
#include <modules/opencl/syncclgl.h>

namespace inviwo {

ProcessorClassName(EntryExitPointsCL, "EntryExitPointsCL"); 
ProcessorCategory(EntryExitPointsCL, "Geometry Rendering");
ProcessorCodeState(EntryExitPointsCL, CODE_STATE_STABLE);

EntryExitPointsCL::EntryExitPointsCL()
    : Processor()
    , geometryPort_("geometry")
    , entryPort_("entry-points", COLOR_DEPTH, DataVec4FLOAT32::get()) // Using 8-bits will create artifacts when entering the volume
    , exitPort_("exit-points", COLOR_DEPTH, DataVec4FLOAT32::get())
    , camera_("camera", "Camera", vec3(0.0f, 0.0f, -2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f))
    , workGroupSize_("wgsize", "Work group size", ivec2(8, 8), ivec2(0), ivec2(256))
    , useGLSharing_("glsharing", "Use OpenGL sharing", true)
{
    addPort(geometryPort_);
    addPort(entryPort_, "ImagePortGroup1");
    addPort(exitPort_, "ImagePortGroup1");

    addProperty(camera_);
    addProperty(workGroupSize_);
    addProperty(useGLSharing_);
	addInteractionHandler(new Trackball(&camera_));
    entryPort_.addResizeEventListener(&camera_);
}

EntryExitPointsCL::~EntryExitPointsCL() {}

void EntryExitPointsCL::initialize() {
    Processor::initialize();

	try {
		cl::Program* program = KernelManager::getRef().buildProgram(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES)+"basecl/cl/entryexitpoints.cl");
		entryExitKernel_ = KernelManager::getRef().getKernel(program, "entryexitpoints");
	} catch (cl::Error&) {

	}
}

void EntryExitPointsCL::deinitialize() {
    Processor::deinitialize();
}

void EntryExitPointsCL::process() {
    const Geometry* geom = geometryPort_.getData();
    const Mesh* mesh = dynamic_cast<const Mesh*>(geom);
    if (!mesh) return;
	// the rendered plane is specified in camera coordinates
	// thus we must transform from camera to world to texture coordinates
    mat4 worldToTexMat = geom->getCoordinateTransformer().getWorldToTextureMatrix();
	uvec2 outportDim = exitPort_.getDimension();

	mat4 NDCToTextureMat = worldToTexMat*camera_.inverseViewMatrix()*camera_.inverseProjectionMatrix();
	vec4 camPosInTextureSpace = worldToTexMat*vec4(camera_.getLookFrom(), 1.f);
#if IVW_PROFILING
    cl::Event* profilingEvent = new cl::Event(); 
#else 
    cl::Event* profilingEvent = NULL;
#endif
    if (useGLSharing_.get()) {
        SyncCLGL glSync;
        LayerCLGL* entryCL = entryPort_.getData()->getEditableRepresentation<ImageCLGL>()->getLayerCLGL();
        LayerCLGL* exitCL = exitPort_.getData()->getEditableRepresentation<ImageCLGL>()->getLayerCLGL();
        entryCL->aquireGLObject(glSync.getGLSyncEvent());
        exitCL->aquireGLObject();
        const cl::Image2D& entry = entryCL->get();
        const cl::Image2D& exit = exitCL->get();
        computeEntryExitPoints(NDCToTextureMat, camPosInTextureSpace, entry, exit, outportDim, profilingEvent);
        exitCL->releaseGLObject();
        entryCL->releaseGLObject(NULL, glSync.getLastReleaseGLEvent());
    } else {
        const cl::Image2D& entry = entryPort_.getData()->getEditableRepresentation<ImageCL>()->getLayerCL()->get();
        const cl::Image2D& exit = exitPort_.getData()->getEditableRepresentation<ImageCL>()->getLayerCL()->get();
        computeEntryExitPoints(NDCToTextureMat, camPosInTextureSpace, entry, exit, outportDim, profilingEvent);
    }
#if IVW_PROFILING
    try {
        profilingEvent->wait();
        LogInfo("Exec time: " << profilingEvent->getElapsedTime() << " ms");
    } catch (cl::Error& err) {
        LogError(getCLErrorString(err));
    }
    delete profilingEvent;
#endif

}

void EntryExitPointsCL::computeEntryExitPoints(const mat4& NDCToTextureMat, const vec4& camPosInTextureSpace, const cl::Image2D& entryPointsCL, const cl::Image2D& exitPointsCL, const uvec2& outportDim, cl::Event* profilingEvent) {
    svec2 localWorkGroupSize(workGroupSize_.get());
    svec2 globalWorkGroupSize(getGlobalWorkGroupSize(entryPort_.getData()->getDimension().x, localWorkGroupSize.x), getGlobalWorkGroupSize(entryPort_.getData()->getDimension().y, localWorkGroupSize.y));

    try
    {
        cl_uint arg = 0;
        entryExitKernel_->setArg(arg++, NDCToTextureMat);
        entryExitKernel_->setArg(arg++, camPosInTextureSpace);
        entryExitKernel_->setArg(arg++, vec2(camera_.getNearPlaneDist(), camera_.getFarPlaneDist()));
        entryExitKernel_->setArg(arg++, entryPointsCL);
        entryExitKernel_->setArg(arg++, exitPointsCL);
        OpenCL::instance()->getQueue().enqueueNDRangeKernel(*entryExitKernel_, cl::NullRange, globalWorkGroupSize, localWorkGroupSize, NULL, profilingEvent);

    } catch (cl::Error& err) {
        LogError(getCLErrorString(err));
    }
}

} // namespace
