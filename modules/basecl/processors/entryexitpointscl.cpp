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
#include <modules/opencl/kernelmanager.h>

namespace inviwo {

ProcessorClassName(EntryExitPointsCL, "EntryExitPointsCL"); 
ProcessorCategory(EntryExitPointsCL, "Geometry Rendering");
ProcessorCodeState(EntryExitPointsCL, CODE_STATE_STABLE);

EntryExitPointsCL::EntryExitPointsCL()
    : Processor(),
    geometryPort_("geometry"),

    //entryPort_("entry-points"),
    //exitPort_("exit-points"),
    // Temporally disabled /Peter
    entryPort_("entry-points", COLOR_DEPTH, DataVec4FLOAT32::get()), // Using 8-bits will create artifacts when entering the volume
    exitPort_("exit-points", COLOR_DEPTH, DataVec4FLOAT32::get()),
    camera_("camera", "Camera", vec3(0.0f, 0.0f, -2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f))
{
    addPort(geometryPort_);
    addPort(entryPort_, "ImagePortGroup1");
    addPort(exitPort_, "ImagePortGroup1");

    addProperty(camera_);
	addInteractionHandler(new Trackball(&camera_));
    entryPort_.addResizeEventListener(&camera_);
}

EntryExitPointsCL::~EntryExitPointsCL() {}

void EntryExitPointsCL::initialize() {
    Processor::initialize();

	try {
		cl::Program* program = KernelManager::getRef().buildProgram(IVW_DIR+"modules/basecl/cl/entryexitpoints.cl");
		capNearClippingPrg_ = KernelManager::getRef().getKernel(program, "entryexitpoints");


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
	ImageCL* entryPointsCL = entryPort_.getData()->getEditableRepresentation<ImageCL>();
	ImageCL* exitPointsCL = exitPort_.getData()->getEditableRepresentation<ImageCL>();
	mat4 NDCToTextureMat = worldToTexMat*camera_.inverseViewMatrix()*camera_.inverseProjectionMatrix();
	vec4 camPosInTextureSpace = worldToTexMat*vec4(camera_.getLookFrom(), 1.f);
	try
	{
		cl_uint arg = 0;
		capNearClippingPrg_->setArg(arg++, NDCToTextureMat);
		capNearClippingPrg_->setArg(arg++, camPosInTextureSpace);
		capNearClippingPrg_->setArg(arg++, vec2(camera_.getNearPlaneDist(), camera_.getFarPlaneDist()));
		capNearClippingPrg_->setArg(arg++, *entryPointsCL);
		capNearClippingPrg_->setArg(arg++, *exitPointsCL);

		OpenCL::instance()->getQueue().enqueueNDRangeKernel(*capNearClippingPrg_, cl::NullRange, static_cast<glm::svec2>(outportDim));
	} catch (cl::Error& err) {
		LogError(getCLErrorString(err));
	}

}

} // namespace
