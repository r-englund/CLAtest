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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include "canvasgl.h"
#include <modules/opengl/glwrap/textureunit.h>
#include <inviwo/core/datastructures/geometry/mesh.h>
#include <modules/opengl/image/imagegl.h>
#include <modules/opengl/geometry/attributebuffergl.h>
#include <modules/opengl/geometry/meshglrenderer.h>

namespace inviwo {

bool CanvasGL::glewInitialized_ = false;
GLuint CanvasGL::screenAlignedVerticesId_ = 0;
GLuint CanvasGL::screenAlignedTexCoordsId_ = 1;

CanvasGL::CanvasGL(uvec2 dimensions)
    : Canvas(dimensions) {
    layerGL_ = NULL;
    shader_ = NULL;
    noiseShader_ = NULL;
}

CanvasGL::~CanvasGL() {}

void CanvasGL::initialize() {
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_COLOR_MATERIAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    LGL_ERROR;
    shader_ = new Shader("img_texturequad.frag");
    LGL_ERROR;
    noiseShader_ = new Shader("img_noise.frag");
    LGL_ERROR;
}

void CanvasGL::initializeGLEW() {
    if (!glewInitialized_) {
        LogInfo("Initializing GLEW");
        glewInit();
        LGL_ERROR;
        glewInitialized_ = true;
    }
}

void CanvasGL::initializeSquare() {
    const Mesh* screenAlignedRectMesh = dynamic_cast<const Mesh*>(screenAlignedRect_);
    if (screenAlignedRectMesh) {
        screenAlignedVerticesId_ = screenAlignedRectMesh->getAttributes(0)->getRepresentation<BufferGL>()->getId();
        screenAlignedTexCoordsId_ = screenAlignedRectMesh->getAttributes(1)->getRepresentation<BufferGL>()->getId();
        LGL_ERROR;
    }
}

void CanvasGL::deinitialize() {
    delete shader_;
    shader_ = NULL;
    delete noiseShader_;
    noiseShader_ = NULL;
}

void CanvasGL::activate() {}

void CanvasGL::render(const Image* image, LayerType layerType){
    if (image) {
        layerGL_ = image->getLayer(layerType)->getRepresentation<LayerGL>();
        pickingContainer_->setPickingSource(image);
        renderLayer();
        //Called make sure a full ImageGL is added as representation.
        image->getRepresentation<ImageGL>();
    } else {
        layerGL_ = NULL;
        renderNoise();
    }
}

void CanvasGL::resize(uvec2 size) {
    glViewport(0, 0, size[0], size[1]);
    Canvas::resize(size);
}

void CanvasGL::glSwapBuffers(){
}

void CanvasGL::update() {
    if (layerGL_){
        renderLayer();
    } else {
        renderNoise();
    }
}

void CanvasGL::renderLayer() {
    TextureUnit textureUnit;
    layerGL_->bindTexture(textureUnit.getEnum());
    renderTexture(textureUnit.getUnitNumber());
    layerGL_->unbindTexture();
}

void CanvasGL::renderNoise() {
    activate();
    //FIXME: glViewport should not be here, which indicates this context is not active.
    glViewport(0, 0, dimensions_[0], dimensions_[1]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    noiseShader_->activate();
    renderImagePlaneRect();
    noiseShader_->deactivate();
    glSwapBuffers();
}

void CanvasGL::renderTexture(GLint unitNumber) {
    activate();
    //FIXME: glViewport should not be here, which indicates this context is not active.
    glViewport(0, 0, dimensions_[0], dimensions_[1]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shader_->activate();
    shader_->setUniform("tex_", unitNumber);
    renderImagePlaneRect();
    shader_->deactivate();
    glDisable(GL_BLEND);
    glSwapBuffers();
}

} // namespace
