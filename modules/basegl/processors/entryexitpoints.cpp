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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include "entryexitpoints.h"
#include <inviwo/core/interaction/trackball.h>
#include <inviwo/core/rendering/geometryrendererfactory.h>

namespace inviwo {

ProcessorClassName(EntryExitPoints, "EntryExitPoints"); 
ProcessorCategory(EntryExitPoints, "Geometry Rendering");
ProcessorCodeState(EntryExitPoints, CODE_STATE_STABLE);

EntryExitPoints::EntryExitPoints()
    : ProcessorGL(),
    geometryPort_("geometry"),
    entryPort_("entry-points", COLOR_DEPTH, DataVec4FLOAT32::get()), // Using 8-bits will create artifacts when entering the volume
    exitPort_("exit-points", COLOR_DEPTH, DataVec4FLOAT32::get()),
    camera_("camera", "Camera", vec3(0.0f, 0.0f, -2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)),
	capNearClipping_("capNearClipping", "Cap near plane clipping", true),
    tmpEntryPoints_(NULL),
    renderer_(NULL)
{
    addPort(geometryPort_);
    addPort(entryPort_, "ImagePortGroup1");
    addPort(exitPort_, "ImagePortGroup1");

    addProperty(camera_);
	addProperty(capNearClipping_);
    trackball_ = new Trackball(&camera_);
	addInteractionHandler(trackball_);
    entryPort_.addResizeEventListener(&camera_);
}

EntryExitPoints::~EntryExitPoints() {
    deinitialize();
    delete trackball_;
}


void EntryExitPoints::initialize() {
    ProcessorGL::initialize();
	capNearClippingPrg_ = new Shader("capnearclipping.frag");

}

void EntryExitPoints::deinitialize() {
    delete tmpEntryPoints_;
    tmpEntryPoints_ = 0;
    delete capNearClippingPrg_;
    capNearClippingPrg_ = 0;
    delete renderer_; renderer_ = 0;
    ProcessorGL::deinitialize();
}

void EntryExitPoints::process() {
    const Geometry* geom = geometryPort_.getData();
    // Check if no renderer exist or if geometry changed
    if (renderer_ == NULL) {
        renderer_ = GeometryRendererFactory::getPtr()->create(geom);
    } else if (renderer_->getGeometry() != geom) {
        delete renderer_; // Nothing will happen if NULL
        renderer_ = GeometryRendererFactory::getPtr()->create(geom);
    }
    if (renderer_ == NULL) {
        // No renderer found
        return;
    }
    glEnable(GL_CULL_FACE);    

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(camera_.projectionMatrix()));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(camera_.viewMatrix()));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);

	// generate exit points
	activateAndClearTarget(exitPort_);
	glCullFace(GL_FRONT);
    renderer_->render();
	deactivateCurrentTarget();

	// generate entry points

    ImageGL* tmpEntryPointsGL;
	if (capNearClipping_.get()) {
        if (tmpEntryPoints_ == NULL || 
            tmpEntryPoints_->getDimension() != entryPort_.getDimension() || 
            tmpEntryPoints_->getImageType() != entryPort_.getData()->getImageType() || 
            tmpEntryPoints_->getDataFormat() != entryPort_.getData()->getDataFormat()) {
            delete tmpEntryPoints_;
            tmpEntryPoints_ = new Image(entryPort_.getDimension(), entryPort_.getData()->getImageType(), entryPort_.getData()->getDataFormat());
        }

		tmpEntryPointsGL = tmpEntryPoints_->getEditableRepresentation<ImageGL>();
		tmpEntryPointsGL->activateBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	} else {
		activateAndClearTarget(entryPort_);
	}
	glCullFace(GL_BACK);
	renderer_->render();
	deactivateCurrentTarget();

	if (capNearClipping_.get()) {
		// render an image plane aligned quad to cap the proxy geometry
		activateAndClearTarget(entryPort_);

		TextureUnit entryColorUnit, entryDepthUnit, exitColorUnit, exitDepthUnit;
		tmpEntryPointsGL->getColorLayerGL()->bindTexture(entryColorUnit.getEnum());
		tmpEntryPointsGL->getDepthLayerGL()->bindTexture(entryDepthUnit.getEnum());
		bindTextures(exitPort_, exitColorUnit.getEnum(), exitDepthUnit.getEnum());

		capNearClippingPrg_->activate();
		setGlobalShaderParameters(capNearClippingPrg_);

		capNearClippingPrg_->setUniform("entryColorTex_", entryColorUnit.getUnitNumber());
		capNearClippingPrg_->setUniform("entryDepthTex_", entryDepthUnit.getUnitNumber());
		setTextureParameters(entryPort_, capNearClippingPrg_, "entryParameters_");
		capNearClippingPrg_->setUniform("exitColorTex_", exitColorUnit.getUnitNumber());
		capNearClippingPrg_->setUniform("exitDepthTex_", exitDepthUnit.getUnitNumber());
		setTextureParameters(exitPort_, capNearClippingPrg_, "exitParameters_");

		// the rendered plane is specified in camera coordinates
		// thus we must transform from camera to world to texture coordinates
        mat4 worldToTexMat = geom->getCoordinateTransformer().getWorldToTextureMatrix();
        capNearClippingPrg_->setUniform("NDCToTextureMat_", worldToTexMat*camera_.inverseViewMatrix()*camera_.inverseProjectionMatrix());
        capNearClippingPrg_->setUniform("nearDist_", camera_.getNearPlaneDist());

        renderImagePlaneRect();

		capNearClippingPrg_->deactivate();

	}

    glDepthFunc(GL_LESS);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    
    glDisable(GL_CULL_FACE);
}

} // namespace
