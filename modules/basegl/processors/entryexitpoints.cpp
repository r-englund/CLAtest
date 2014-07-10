/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Main file authors: Timo Ropinski, Erik Sundén, Daniel Jönsson
 *
 *********************************************************************************/

#include "entryexitpoints.h"
#include <inviwo/core/interaction/trackball.h>
#include <inviwo/core/rendering/geometryrendererfactory.h>
#include <modules/opengl/clockgl.h>

namespace inviwo {

ProcessorClassIdentifier(EntryExitPoints,  "EntryExitPoints");
ProcessorDisplayName(EntryExitPoints,  "Entry Exit Points");
ProcessorTags(EntryExitPoints, Tags::GL);
ProcessorCategory(EntryExitPoints, "Geometry Rendering");
ProcessorCodeState(EntryExitPoints, CODE_STATE_STABLE);

EntryExitPoints::EntryExitPoints()
    : ProcessorGL()
    , geometryPort_("geometry")
    , entryPort_("entry-points", COLOR_DEPTH, DataVec4UINT16::get())
    , exitPort_("exit-points", COLOR_DEPTH, DataVec4UINT16::get())
    , camera_("camera", "Camera", vec3(0.0f, 0.0f, -2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), &geometryPort_)
    , capNearClipping_("capNearClipping", "Cap near plane clipping", true)
    , handleInteractionEvents_("handleEvents", "Handle interaction events", true)
    , trackball_(NULL)
    , genericShader_(NULL)
    , capNearClippingPrg_(NULL)
    , tmpEntryPoints_(NULL)
    , renderer_(NULL) {
    addPort(geometryPort_);
    addPort(entryPort_, "ImagePortGroup1");
    addPort(exitPort_, "ImagePortGroup1");
    addProperty(camera_);
    addProperty(capNearClipping_);
    addProperty(handleInteractionEvents_);
    handleInteractionEvents_.onChange(this, &EntryExitPoints::handleInteractionEventsChanged);
    trackball_ = new Trackball(&camera_);
    addInteractionHandler(trackball_);
    entryPort_.addResizeEventListener(&camera_);

    geometryPort_.onChange(this,&EntryExitPoints::onGeometryChange);
}

EntryExitPoints::~EntryExitPoints() {
    removeInteractionHandler(trackball_);
    delete trackball_;
    trackball_ = NULL;
}


void EntryExitPoints::initialize() {
    ProcessorGL::initialize();
    genericShader_ =  new Shader("standard.vert", "standard.frag");
    capNearClippingPrg_ = new Shader("img_identity.vert", "capnearclipping.frag");
}

void EntryExitPoints::deinitialize() {
    delete tmpEntryPoints_;
    tmpEntryPoints_ = NULL;
    delete genericShader_;
    genericShader_ = NULL;
    delete capNearClippingPrg_;
    capNearClippingPrg_ = NULL;
    delete renderer_;
    renderer_ = NULL;
    ProcessorGL::deinitialize();
}

void EntryExitPoints::process() {
    const Geometry* geom = geometryPort_.getData();

    // Check if no renderer exist or if geometry changed
    if (renderer_ == NULL) {
        onGeometryChange();
    }

    if (renderer_ == NULL) {
        return;
    }
    IVW_OPENGL_PROFILING("")

    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_ALWAYS);
    // generate exit points
    activateAndClearTarget(exitPort_);
    glPointSize(1.f);
    glCullFace(GL_FRONT);
    genericShader_->activate();
    mat4 modelMatrix = geom->getWorldTransform()*geom->getBasisAndOffset();
    genericShader_->setUniform("modelViewProjectionMatrix_", camera_.projectionMatrix()*camera_.viewMatrix()*modelMatrix);
    renderer_->render();
    deactivateCurrentTarget();
    // generate entry points
    ImageGL* tmpEntryPointsGL;

    if (capNearClipping_.get()) {
        if (
            tmpEntryPoints_ == NULL ||
            tmpEntryPoints_->getDimension() != entryPort_.getDimension() ||
            tmpEntryPoints_->getImageType() != entryPort_.getData()->getImageType() ||
            tmpEntryPoints_->getDataFormat() != entryPort_.getData()->getDataFormat()
            ) {
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
    genericShader_->deactivate();
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
        deactivateCurrentTarget();
    }

    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);

}

void EntryExitPoints::handleInteractionEventsChanged() {
    if (handleInteractionEvents_.get()) {
        addInteractionHandler(trackball_);
    } else {
        removeInteractionHandler(trackball_);
    }
}

void EntryExitPoints::onGeometryChange(){
    delete renderer_; 
    renderer_ = NULL;
    if(geometryPort_.hasData())
        renderer_ = GeometryRendererFactory::getPtr()->create(geometryPort_.getData());
}

} // namespace
