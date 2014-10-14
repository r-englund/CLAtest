/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Contact: Daniel Jönsson
 *
 *********************************************************************************/

#include "entryexitpointscl.h"
#include <inviwo/core/datastructures/geometry/mesh.h>
#include <modules/opencl/image/imagecl.h>
#include <modules/opencl/image/imageclgl.h>
#include <modules/opencl/settings/openclsettings.h>
#include <modules/opencl/syncclgl.h>

namespace inviwo {

ProcessorClassIdentifier(EntryExitPointsCL, "org.inviwo.EntryExitPointsCL");
ProcessorDisplayName(EntryExitPointsCL,  "Entry Exit Points");
ProcessorTags(EntryExitPointsCL, Tags::CL);
ProcessorCategory(EntryExitPointsCL, "Geometry Rendering");
ProcessorCodeState(EntryExitPointsCL, CODE_STATE_STABLE);

EntryExitPointsCL::EntryExitPointsCL()
    : Processor(), ProcessorKernelOwner(this)
    , geometryPort_("geometry")
    , entryPort_("entry-points", COLOR_DEPTH, DataVec4FLOAT32::get()) // Using 8-bits will create artifacts when entering the volume
    , exitPort_("exit-points", COLOR_DEPTH, DataVec4FLOAT32::get())
    , camera_("camera", "Camera", vec3(0.0f, 0.0f, -2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), &geometryPort_)
    , workGroupSize_("wgsize", "Work group size", ivec2(8, 8), ivec2(0), ivec2(256))
    , useGLSharing_("glsharing", "Use OpenGL sharing", true)
    , handleInteractionEvents_("handleEvents", "Handle interaction events", true)
{
    addPort(geometryPort_);
    addPort(entryPort_, "ImagePortGroup1");
    addPort(exitPort_, "ImagePortGroup1");
    addProperty(camera_);
    addProperty(workGroupSize_);
    addProperty(useGLSharing_);
    addProperty(handleInteractionEvents_);
    handleInteractionEvents_.onChange(this, &EntryExitPointsCL::handleInteractionEventsChanged);
    trackball_ = new CameraTrackball(&camera_);
    addInteractionHandler(trackball_);
    entryPort_.addResizeEventListener(&camera_);
}

EntryExitPointsCL::~EntryExitPointsCL() {}

void EntryExitPointsCL::initialize() {
    Processor::initialize();
    entryExitKernel_ = addKernel("entryexitpoints.cl", "entryexitpoints");
    if (!InviwoApplication::getPtr()->getSettingsByType<OpenCLSettings>()->isSharingEnabled()) {
        useGLSharing_.setReadOnly(true);
        useGLSharing_.set(false);
    }
}

void EntryExitPointsCL::deinitialize() {
    delete trackball_;
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
#if IVW_PROFILING
    cl::Event* profilingEvent = new cl::Event();
#else
    cl::Event* profilingEvent = NULL;
#endif

    if (useGLSharing_.get()) {
        SyncCLGL glSync;
        LayerCLGL* entryCL = entryPort_.getData()->getEditableRepresentation<ImageCLGL>()->getLayerCL();
        LayerCLGL* exitCL = exitPort_.getData()->getEditableRepresentation<ImageCLGL>()->getLayerCL();
        entryCL->aquireGLObject(glSync.getGLSyncEvent());
        exitCL->aquireGLObject();
        const cl::Image& entry = entryCL->get();
        const cl::Image& exit = exitCL->get();
        computeEntryExitPoints(NDCToTextureMat, entry, exit, outportDim, profilingEvent);
        exitCL->releaseGLObject();
        entryCL->releaseGLObject(NULL, glSync.getLastReleaseGLEvent());
    } else {
        const cl::Image& entry = entryPort_.getData()->getEditableRepresentation<ImageCL>()->getLayerCL()->get();
        const cl::Image& exit = exitPort_.getData()->getEditableRepresentation<ImageCL>()->getLayerCL()->get();
        computeEntryExitPoints(NDCToTextureMat, entry, exit, outportDim, profilingEvent);
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

void EntryExitPointsCL::computeEntryExitPoints(const mat4& NDCToTextureMat, const cl::Image& entryPointsCL, const cl::Image& exitPointsCL,
        const uvec2& outportDim, cl::Event* profilingEvent)
{
    svec2 localWorkGroupSize(workGroupSize_.get());
    svec2 globalWorkGroupSize(getGlobalWorkGroupSize(entryPort_.getData()->getDimension().x, localWorkGroupSize.x),
                              getGlobalWorkGroupSize(entryPort_.getData()->getDimension().y, localWorkGroupSize.y));

    try
    {
        cl_uint arg = 0;
        entryExitKernel_->setArg(arg++, NDCToTextureMat);
        entryExitKernel_->setArg(arg++, entryPointsCL);
        entryExitKernel_->setArg(arg++, exitPointsCL);
        OpenCL::getPtr()->getQueue().enqueueNDRangeKernel(*entryExitKernel_, cl::NullRange, globalWorkGroupSize, localWorkGroupSize, NULL,
                profilingEvent);
    } catch (cl::Error& err) {
        LogError(getCLErrorString(err));
    }
}

void EntryExitPointsCL::handleInteractionEventsChanged() {
    if (handleInteractionEvents_.get()) {
        addInteractionHandler(trackball_);
    } else {
        removeInteractionHandler(trackball_);
    }
}
} // namespace
