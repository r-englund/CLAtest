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
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

#include "canvasgl.h"
#include <modules/opengl/glwrap/textureunit.h>
#include <inviwo/core/datastructures/geometry/mesh.h>
#include <modules/opengl/image/imagegl.h>
#include <modules/opengl/buffer/buffergl.h>
#include <modules/opengl/rendering/meshrenderer.h>

namespace inviwo {

bool CanvasGL::glewInitialized_ = false;
GLuint CanvasGL::screenAlignedVerticesId_ = 0;
GLuint CanvasGL::screenAlignedTexCoordsId_ = 1;

CanvasGL::CanvasGL(uvec2 dimensions)
    : Canvas(dimensions) {
    imageGL_ = NULL;
    shader_ = NULL;
    noiseShader_ = NULL;
    layerType_ = COLOR_LAYER;
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

void CanvasGL::render(const Image* image, LayerType layerType) {
    if (image) {
        imageGL_ = image->getRepresentation<ImageGL>();
        layerType_ = layerType;
        pickingContainer_->setPickingSource(image);
        renderLayer();
    } else {
        pickingContainer_->setPickingSource(NULL);
        imageGL_  = NULL;
        renderNoise();
    }
}

void CanvasGL::resize(uvec2 size) {
    if (imageGL_)
        imageGL_->updateExistingLayers();

    Canvas::resize(size);
    glViewport(0, 0, size[0], size[1]);
}

void CanvasGL::glSwapBuffers() {
}

void CanvasGL::update() {
    if (imageGL_) {
        renderLayer();
    } else {
        renderNoise();
    }
}

void CanvasGL::renderLayer() {
    const LayerGL* layerGL = imageGL_->getLayerGL(layerType_);
    TextureUnit textureUnit;
    layerGL->bindTexture(textureUnit.getEnum());
    renderTexture(textureUnit.getUnitNumber());
    layerGL->unbindTexture();
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
