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
 * Main file author: Erik Sund�n
 *
 *********************************************************************************/

#include "canvasgl.h"
#include <inviwo/core/datastructures/geometry/mesh.h>
#include <modules/opengl/buffer/buffergl.h>
#include <modules/opengl/glwrap/textureunit.h>
#include <modules/opengl/image/imagegl.h>
#include <modules/opengl/rendering/meshrenderer.h>
#include <modules/opengl/openglcapabilities.h>

namespace inviwo {

bool CanvasGL::glewInitialized_ = false;
const MeshGL* CanvasGL::screenAlignedRectGL_ = NULL;

CanvasGL::CanvasGL(uvec2 dimensions)
    : Canvas(dimensions)
    , rectArray_(NULL)
    , imageGL_(NULL)
    , layerType_(COLOR_LAYER)
    , shader_(NULL)
    , noiseShader_(NULL)
    , singleChannel_(false) {}

CanvasGL::~CanvasGL() {}

void CanvasGL::initialize() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    LGL_ERROR;
    shader_ = new Shader("img_texturequad.vert", "img_texturequad.frag");
    LGL_ERROR;
    noiseShader_ = new Shader("img_texturequad.vert", "img_noise.frag");
    LGL_ERROR;
}

void CanvasGL::initializeGLEW() {
    if (!glewInitialized_) {
#ifndef __APPLE__
        std::string preferProfile = OpenGLCapabilities::getPreferredProfile();
        if (preferProfile == "core") glewExperimental = GL_TRUE;
        GLenum glewError = glewInit();
        if (GLEW_OK != glewError) {
            LogError(glewGetErrorString(glewError));
        }
#endif
        LGL_ERROR_SUPPRESS;
        glewInitialized_ = true;
    }
}

void CanvasGL::initializeSquare() {
    const Mesh* screenAlignedRectMesh = dynamic_cast<const Mesh*>(screenAlignedRect_);

    if (screenAlignedRectMesh) {
        screenAlignedRectGL_ = screenAlignedRectMesh->getRepresentation<MeshGL>();
        LGL_ERROR;
    }
}

void CanvasGL::deinitialize() {
    delete shader_;
    shader_ = NULL;
    delete noiseShader_;
    noiseShader_ = NULL;
    delete rectArray_;
    rectArray_ = NULL;
}

void CanvasGL::activate() {}

void CanvasGL::render(const Image* image, LayerType layerType) {
    if (image) {
        imageGL_ = image->getRepresentation<ImageGL>();
        layerType_ = layerType;
        pickingContainer_->setPickingSource(image);
        checkChannels(image->getDataFormat()->getComponents());
        renderLayer();
    } else {
        pickingContainer_->setPickingSource(NULL);
        imageGL_ = NULL;
        renderNoise();
    }
}

void CanvasGL::resize(uvec2 size) {
    resize(size, size);
}

void CanvasGL::resize(uvec2 canvasSize, uvec2 imageSize) {
    if (imageGL_) imageGL_->updateExistingLayers();

    activate();
    glViewport(0, 0, canvasSize[0], canvasSize[1]);
    Canvas::resize(canvasSize, imageSize);
}

void CanvasGL::glSwapBuffers() {}

void CanvasGL::update() {
    if (imageGL_) {
        renderLayer();
    } else {
        renderNoise();
    }
}

void CanvasGL::attachImagePlanRect(BufferObjectArray* arrayObject) {
    if (arrayObject) {
        arrayObject->bind();
        arrayObject->attachBufferObjectToGenericLocation(
            screenAlignedRectGL_->getBufferGL(0)->getBufferObject());
        arrayObject->attachBufferObjectToGenericLocation(
            screenAlignedRectGL_->getBufferGL(1)->getBufferObject());
        arrayObject->unbind();
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    noiseShader_->activate();
    drawRect();
    noiseShader_->deactivate();
    glSwapBuffers();
    activateDefaultRenderContext();
}

void CanvasGL::renderTexture(GLint unitNumber) {
    activate();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shader_->activate();
    shader_->setUniform("tex_", unitNumber);
    drawRect();
    shader_->deactivate();
    glDisable(GL_BLEND);
    glSwapBuffers();
    activateDefaultRenderContext();
}

void CanvasGL::drawRect() {
    if (!rectArray_) {
        rectArray_ = new BufferObjectArray();
        rectArray_->bind();
        rectArray_->attachBufferObjectToGenericLocation(
            screenAlignedRectGL_->getBufferGL(0)->getBufferObject());
        rectArray_->attachBufferObjectToGenericLocation(
            screenAlignedRectGL_->getBufferGL(1)->getBufferObject());
    } else {
        rectArray_->bind();
    }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    rectArray_->unbind();
}

void CanvasGL::checkChannels(int channels) {
    if (!singleChannel_ && channels == 1) {
        shader_->getFragmentShaderObject()->addShaderDefine("SINGLE_CHANNEL");
        shader_->getFragmentShaderObject()->build();
        shader_->link();
    } else if (singleChannel_ && channels == 4) {
        shader_->getFragmentShaderObject()->removeShaderDefine("SINGLE_CHANNEL");
        shader_->getFragmentShaderObject()->build();
        shader_->link();
    }
}

}  // namespace
