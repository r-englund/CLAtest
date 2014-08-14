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

#include <inviwo/core/util/formats.h>
#include <modules/opengl/image/imagegl.h>
#include <modules/opengl/glwrap/textureunit.h>
#include <modules/opengl/canvasgl.h>

namespace inviwo {

ImageGL::ImageGL()
    : ImageRepresentation()
    , rectArray_(NULL)
    , frameBufferObject_(NULL)
    , shader_(NULL)
    , initialized_(false) {
    initialize();
}

ImageGL::ImageGL(const ImageGL& rhs)
    : ImageRepresentation(rhs)
    , rectArray_(NULL)
    , frameBufferObject_(NULL)
    , shader_(NULL)
    , initialized_(false) {
    initialize();
}

ImageGL::~ImageGL() { deinitialize(); }

void ImageGL::initialize() {
    if (initialized_) return;

    if (shader_) delete shader_;

    shader_ = new Shader("standard.vert", "img_copy.frag");

    if (frameBufferObject_) delete frameBufferObject_;

    frameBufferObject_ = new FrameBufferObject();
    initialized_ = true;
}

void ImageGL::deinitialize() {
    frameBufferObject_->deactivate();
    delete frameBufferObject_;
    frameBufferObject_ = NULL;
    delete shader_;
    shader_ = NULL;
    delete rectArray_;
    rectArray_ = NULL;
}

ImageGL* ImageGL::clone() const { return new ImageGL(*this); }

void ImageGL::reAttachAllLayers(bool clearLayers) {
    frameBufferObject_->activate();
    frameBufferObject_->detachAllTextures();
    pickingAttachmentID_ = 0;
    GLenum id = 0;

    // GLuint clearColor[4] = {0, 0, 0, 0};
    for (size_t i = 0; i < colorLayersGL_.size(); ++i) {
        colorLayersGL_[i]->getTexture()->bind();
        id = frameBufferObject_->attachColorTexture(colorLayersGL_[i]->getTexture());
        /*if(clearLayers){
            glDrawBuffer(id);
            glClearBufferuiv(GL_COLOR, 0, clearColor);
        }*/
    }

    // Layer* depthLayer = this->getOwner()->getDepthLayer();
    if (depthLayerGL_) {
        depthLayerGL_->getTexture()->bind();
        frameBufferObject_->attachTexture(depthLayerGL_->getTexture(),
                                          static_cast<GLenum>(GL_DEPTH_ATTACHMENT));
        /*if(clearLayers){
            id = GL_DEPTH_ATTACHMENT;
            glDrawBuffer(id);
            //const GLfloat clearDepth = 0.f;
            //glClearBufferfv(GL_DEPTH, 0, &clearDepth);
        }*/
    }

    // Layer* pickingLayer = this->getOwner()->getPickingLayer();
    if (pickingLayerGL_) {
        pickingLayerGL_->getTexture()->bind();
        id = pickingAttachmentID_ =
            frameBufferObject_->attachColorTexture(pickingLayerGL_->getTexture(), 0, true, 1);
        /*if(clearLayers){
            glDrawBuffer(id);
            //glClearBufferuiv(GL_COLOR, 0, clearColor);
        }*/
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    frameBufferObject_->checkStatus();
    frameBufferObject_->deactivate();
}

void ImageGL::activateBuffer() {
    // invalidatePBOs();
    frameBufferObject_->activate();
    frameBufferObject_->defineDrawBuffers();
    uvec2 dim = getDimension();
    glViewport(0, 0, dim.x, dim.y);
    //frameBufferObject_->checkStatus();
}

void ImageGL::deactivateBuffer() { frameBufferObject_->deactivate(); }

bool ImageGL::copyAndResizeRepresentation(DataRepresentation* targetRep) const {
    const ImageGL* source = this;
    ImageGL* target = dynamic_cast<ImageGL*>(targetRep);
    TextureUnit colorUnit, depthUnit, pickingUnit;
    source->getColorLayerGL()->bindTexture(colorUnit.getEnum());
    source->getDepthLayerGL()->bindTexture(depthUnit.getEnum());
    source->getPickingLayerGL()->bindTexture(pickingUnit.getEnum());
    // Render to FBO, with correct scaling
    target->activateBuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float ratioSource = (float)source->getDimension().x / (float)source->getDimension().y;
    float ratioTarget = (float)target->getDimension().x / (float)target->getDimension().y;
    glm::mat4 scale;

    if (ratioTarget < ratioSource)
        scale = glm::scale(glm::vec3(1.0f, ratioTarget / ratioSource, 1.0f));
    else
        scale = glm::scale(glm::vec3(ratioSource / ratioTarget, 1.0f, 1.0f));

    shader_->activate();
    shader_->setUniform("color_", colorUnit.getUnitNumber());
    shader_->setUniform("depth_", depthUnit.getUnitNumber());
    shader_->setUniform("picking_", pickingUnit.getUnitNumber());
    shader_->setUniform("modelViewProjectionMatrix_", scale);
    glDepthMask(GL_TRUE);
    LGL_ERROR;
    //target->frameBufferObject_->checkStatus();
    target->renderImagePlaneRect();
    LGL_ERROR;
    shader_->deactivate();
    target->deactivateBuffer();
    LGL_ERROR;
    return true;
}

bool ImageGL::updateFrom(const ImageGL* source) {
    ImageGL* target = this;
    // Primarily Copy by FBO blitting, all from source FBO to target FBO
    const FrameBufferObject* srcFBO = source->getFBO();
    FrameBufferObject* tgtFBO = target->getFBO();
    const Texture2D* sTex = source->getColorLayerGL()->getTexture();
    Texture2D* tTex = target->getColorLayerGL()->getTexture();

    const std::vector<bool>& srcBuffers = srcFBO->getDrawBuffersInUse();
    const std::vector<bool>& targetBuffers = tgtFBO->getDrawBuffersInUse();
    srcFBO->setRead_Blit();
    tgtFBO->setDraw_Blit();
    GLbitfield mask = GL_COLOR_BUFFER_BIT;

    if (srcFBO->hasDepthAttachment() && tgtFBO->hasDepthAttachment()) mask |= GL_DEPTH_BUFFER_BIT;

    if (srcFBO->hasStencilAttachment() && tgtFBO->hasStencilAttachment())
        mask |= GL_STENCIL_BUFFER_BIT;

    glBlitFramebufferEXT(0, 0, sTex->getWidth(), sTex->getHeight(), 0, 0, tTex->getWidth(),
                         tTex->getHeight(), mask, GL_NEAREST);
    bool pickingCopied = false;

    for (int i = 1; i < srcFBO->getMaxColorAttachments(); i++) {
        if (srcBuffers[i] && targetBuffers[i]) {
            glReadBuffer(GL_COLOR_ATTACHMENT0_EXT + i);
            glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + i);
            glBlitFramebufferEXT(0, 0, sTex->getWidth(), sTex->getHeight(), 0, 0, tTex->getWidth(),
                                 tTex->getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

            if (GL_COLOR_ATTACHMENT0_EXT + i == pickingAttachmentID_) pickingCopied = true;
        }
    }

    glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
    glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
    srcFBO->setRead_Blit(false);
    tgtFBO->setDraw_Blit(false);
    LGL_ERROR;

    // Secondary copy using PBO

    // Depth texture
    if ((mask & GL_DEPTH_BUFFER_BIT) == 0) {
        sTex = source->getDepthLayerGL()->getTexture();
        tTex = target->getDepthLayerGL()->getTexture();

        if (sTex && tTex) tTex->loadFromPBO(sTex);
    }

    LGL_ERROR;

    // Picking texture
    if (!pickingCopied && pickingAttachmentID_ != 0) {
        sTex = source->getPickingLayerGL()->getTexture();
        tTex = target->getPickingLayerGL()->getTexture();

        if (sTex && tTex) tTex->loadFromPBO(sTex);
    }

    LGL_ERROR;
    return true;
}

FrameBufferObject* ImageGL::getFBO() { return frameBufferObject_; }

const FrameBufferObject* ImageGL::getFBO() const { return frameBufferObject_; }

LayerGL* ImageGL::getLayerGL(LayerType type, size_t idx) {
    switch (type) {
        case COLOR_LAYER:
            return getColorLayerGL(idx);

        case DEPTH_LAYER:
            return getDepthLayerGL();

        case PICKING_LAYER:
            return getPickingLayerGL();
    }

    return NULL;
}

const LayerGL* ImageGL::getLayerGL(LayerType type, size_t idx) const {
    switch (type) {
        case COLOR_LAYER:
            return getColorLayerGL(idx);

        case DEPTH_LAYER:
            return getDepthLayerGL();

        case PICKING_LAYER:
            return getPickingLayerGL();
    }

    return NULL;
}

LayerGL* ImageGL::getColorLayerGL(size_t idx) { return colorLayersGL_.at(idx); }

LayerGL* ImageGL::getDepthLayerGL() { return depthLayerGL_; }

LayerGL* ImageGL::getPickingLayerGL() { return pickingLayerGL_; }

const LayerGL* ImageGL::getColorLayerGL(size_t idx) const { return colorLayersGL_.at(idx); }

const LayerGL* ImageGL::getDepthLayerGL() const { return depthLayerGL_; }

const LayerGL* ImageGL::getPickingLayerGL() const { return pickingLayerGL_; }

void ImageGL::updateExistingLayers() const {
    const Image* owner = this->getOwner();

    for (size_t i = 0; i < owner->getNumberOfColorLayers(); ++i) {
        owner->getColorLayer(i)->getRepresentation<LayerGL>();
    }

    const Layer* depthLayer = owner->getDepthLayer();

    if (depthLayer) depthLayer->getRepresentation<LayerGL>();

    const Layer* pickingLayer = owner->getPickingLayer();

    if (pickingLayer) pickingLayer->getRepresentation<LayerGL>();
}

void ImageGL::update(bool editable) {
    Image* owner = this->getOwner();

    bool reAttachTargets = (!isValid() || colorLayersGL_.empty());

    colorLayersGL_.clear();
    depthLayerGL_ = NULL;
    pickingLayerGL_ = NULL;

    if (editable) {
        for (size_t i = 0; i < owner->getNumberOfColorLayers(); ++i) {
            colorLayersGL_.push_back(owner->getColorLayer(i)->getEditableRepresentation<LayerGL>());
            owner->getColorLayer(i)->setDataFormat(getColorLayerGL(i)->getDataFormat());
            owner->getColorLayer(i)->setDimension(getColorLayerGL(i)->getDimension());
        }

        Layer* depthLayer = owner->getDepthLayer();

        if (depthLayer) depthLayerGL_ = depthLayer->getEditableRepresentation<LayerGL>();

        Layer* pickingLayer = owner->getPickingLayer();

        if (pickingLayer) {
            pickingLayer->setDataFormat(getColorLayerGL()->getDataFormat());
            pickingLayer->setDimension(getColorLayerGL()->getDimension());
            pickingLayerGL_ = pickingLayer->getEditableRepresentation<LayerGL>();
        }
    } else {
        for (size_t i = 0; i < owner->getNumberOfColorLayers(); ++i) {
            colorLayersGL_.push_back(
                const_cast<LayerGL*>(owner->getColorLayer(i)->getRepresentation<LayerGL>()));
            owner->getColorLayer(i)->setDataFormat(getColorLayerGL(i)->getDataFormat());
            owner->getColorLayer(i)->setDimension(getColorLayerGL(i)->getDimension());
        }

        Layer* depthLayer = owner->getDepthLayer();

        if (depthLayer)
            depthLayerGL_ = const_cast<LayerGL*>(depthLayer->getRepresentation<LayerGL>());

        Layer* pickingLayer = owner->getPickingLayer();

        if (pickingLayer) {
            pickingLayer->setDataFormat(getColorLayerGL()->getDataFormat());
            pickingLayer->setDimension(getColorLayerGL()->getDimension());
            pickingLayerGL_ = const_cast<LayerGL*>(pickingLayer->getRepresentation<LayerGL>());
        }
    }

    // Attach all targets
    if (reAttachTargets) reAttachAllLayers(true);
}

void ImageGL::renderImagePlaneRect() const {
    delete rectArray_;
    rectArray_ = new BufferObjectArray();
    CanvasGL::attachImagePlanRect(rectArray_);
    LGL_ERROR;
    glDepthFunc(GL_ALWAYS);
    rectArray_->bind();
    LGL_ERROR;
    CanvasGL::singleDrawImagePlaneRect();
    rectArray_->unbind();
    glDepthFunc(GL_LESS);
    LGL_ERROR;
}

GLenum ImageGL::getPickingAttachmentID() const { return pickingAttachmentID_; }

}  // namespace
