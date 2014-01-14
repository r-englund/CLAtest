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

#include <inviwo/core/util/formats.h>
#include <modules/opengl/image/imagegl.h>
#include <modules/opengl/glwrap/textureunit.h>
#include <modules/opengl/canvasgl.h>

namespace inviwo {

ImageGL::ImageGL()
    : ImageRepresentation(), frameBufferObject_(NULL), program_(NULL), initialized_(false){   
    initialize();
}

ImageGL::ImageGL(const ImageGL& rhs) 
: ImageRepresentation(rhs), frameBufferObject_(NULL), program_(NULL), initialized_(false) {
    initialize();
    update(true);
}

ImageGL::~ImageGL() {
    deinitialize();
}

void ImageGL::initialize() {
    if(initialized_)
        return;

    if(program_)
        delete program_;
    program_ = new Shader("img_copy.frag");

    if(frameBufferObject_)
        delete frameBufferObject_;
    frameBufferObject_ = new FrameBufferObject();

    initialized_ = true;
}

void ImageGL::deinitialize() {
    frameBufferObject_->deactivate();
    delete frameBufferObject_;
    frameBufferObject_ = NULL;

    delete program_;
    program_ = NULL;
}

DataRepresentation* ImageGL::clone() const {
    return new ImageGL(*this);
}

void ImageGL::reAttachAllLayers(bool clearLayers){
    frameBufferObject_->activate();
    frameBufferObject_->detachAllTextures();
    pickingAttachmentID_ = 0;

    GLenum id = 0;
    GLuint clearColor[4] = {0, 0, 0, 0};
    for (size_t i=0; i<colorLayersGL_.size(); ++i) {
        colorLayersGL_[i]->getTexture()->bind();
        id = frameBufferObject_->attachColorTexture(colorLayersGL_[i]->getTexture());
        /*if(clearLayers){
            glDrawBuffer(id);
            glClearBufferuiv(GL_COLOR, 0, clearColor);
        }*/
    }

    Layer* depthLayer = owner_->getDepthLayer();
    if(depthLayerGL_){
        depthLayerGL_->getTexture()->bind();
        frameBufferObject_->attachTexture(depthLayerGL_->getTexture(), static_cast<GLenum>(GL_DEPTH_ATTACHMENT));
        /*if(clearLayers){
            id = GL_DEPTH_ATTACHMENT;
            glDrawBuffer(id);
            //const GLfloat clearDepth = 0.f;
            //glClearBufferfv(GL_DEPTH, 0, &clearDepth);
        }*/
    }

    Layer* pickingLayer = owner_->getPickingLayer();
    if(pickingLayerGL_){
        pickingLayerGL_->getTexture()->bind();
        id = pickingAttachmentID_ = frameBufferObject_->attachColorTexture(pickingLayerGL_->getTexture(), 0, true);
        /*if(clearLayers){
            glDrawBuffer(id);
            //glClearBufferuiv(GL_COLOR, 0, clearColor);
        }*/
    }

    frameBufferObject_->checkStatus();
    frameBufferObject_->deactivate();
}

void ImageGL::activateBuffer() {
    //invalidatePBOs();
    frameBufferObject_->activate();
    uvec2 dim = getDimension();
    glViewport(0, 0, dim.x, dim.y);
}

void ImageGL::deactivateBuffer() {
    frameBufferObject_->deactivate();
}

bool ImageGL::copyAndResizeRepresentation(DataRepresentation* targetRep) const {
    const ImageGL* source = this;
    ImageGL* target = dynamic_cast<ImageGL*>(targetRep);

    //Render to FBO, with correct scaling
    target->activateBuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    TextureUnit colorUnit, depthUnit, pickingUnit;
    source->getColorLayerGL()->bindTexture(colorUnit.getEnum());
    source->getDepthLayerGL()->bindTexture(depthUnit.getEnum());
    source->getPickingLayerGL()->bindTexture(pickingUnit.getEnum());

    float ratioSource = (float)source->getDimension().x / (float)source->getDimension().y;
    float ratioTarget = (float)target->getDimension().x / (float)target->getDimension().y;

    glm::mat4 scale;
    if (ratioTarget < ratioSource)
        scale = glm::scale(1.0f, ratioTarget/ratioSource, 1.0f);
    else
        scale = glm::scale(ratioSource/ratioTarget, 1.0f, 1.0f);

    program_->activate();
    program_->setUniform("color_", colorUnit.getUnitNumber());
    program_->setUniform("depth_", depthUnit.getUnitNumber());
    program_->setUniform("picking_", pickingUnit.getUnitNumber());

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(scale));
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_ALWAYS);
    CanvasGL::renderImagePlaneRect();
    glDepthFunc(GL_LESS);
    glDisable(GL_DEPTH_TEST);
    glPopMatrix();

    program_->deactivate();

    target->deactivateBuffer();
    source->getColorLayerGL()->unbindTexture();
    source->getDepthLayerGL()->unbindTexture();
    source->getPickingLayerGL()->unbindTexture();

    LGL_ERROR;

    return true;
}

bool ImageGL::updateFrom(const ImageGL* source) {
    ImageGL* target = this;

    //Primarily Copy by FBO blitting, all from source FBO to target FBO
    const FrameBufferObject* srcFBO = source->getFBO();
    FrameBufferObject* tgtFBO = target->getFBO();
    const Texture2D* sTex = source->getColorLayerGL()->getTexture();
    Texture2D* tTex = target->getColorLayerGL()->getTexture();

    const GLenum* srcIDs = srcFBO->getDrawBuffers();
    const GLenum* targetIDs = tgtFBO->getDrawBuffers();

    srcFBO->setRead_Blit(); 
    tgtFBO->setDraw_Blit();

    GLbitfield mask = GL_COLOR_BUFFER_BIT;
    if(srcFBO->hasDepthAttachment() && tgtFBO->hasDepthAttachment())
        mask |= GL_DEPTH_BUFFER_BIT;
    if(srcFBO->hasStencilAttachment() && tgtFBO->hasStencilAttachment())
        mask |= GL_STENCIL_BUFFER_BIT;

    glBlitFramebufferEXT(0, 0, sTex->getWidth(), sTex->getHeight(),
        0, 0, tTex->getWidth(), tTex->getHeight(),
        mask, GL_NEAREST);

    bool pickingCopied = false;

    for(int i = 1; i < srcFBO->getMaxColorAttachments(); i++){
        if(srcIDs[i] != GL_NONE && srcIDs[i] == targetIDs[i]){
            glReadBuffer(srcIDs[i]);
            glDrawBuffer(targetIDs[i]);
            glBlitFramebufferEXT(0, 0, sTex->getWidth(), sTex->getHeight(),
                0, 0, tTex->getWidth(), tTex->getHeight(),
                GL_COLOR_BUFFER_BIT, GL_NEAREST);
            if(srcIDs[i] == pickingAttachmentID_)
                pickingCopied = true;
        }
    }
    glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
    glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

    srcFBO->setRead_Blit(false); 
    tgtFBO->setDraw_Blit(false);        
    FrameBufferObject::deactivate();
    LGL_ERROR;

    //Secondary copy using PBO

    //Depth texture
    if((mask & GL_DEPTH_BUFFER_BIT) == 0){
        sTex = source->getDepthLayerGL()->getTexture();
        tTex = target->getDepthLayerGL()->getTexture();

        if(sTex && tTex)
            tTex->uploadFromPBO(sTex);
    }
    LGL_ERROR;

    //Picking texture
    if(!pickingCopied && pickingAttachmentID_ != 0){
        sTex = source->getPickingLayerGL()->getTexture();
        tTex = target->getPickingLayerGL()->getTexture();

        if(sTex && tTex)
            tTex->uploadFromPBO(sTex);
    }
    LGL_ERROR;

    return true;
}

FrameBufferObject* ImageGL::getFBO() {
    return frameBufferObject_;
}

const FrameBufferObject* ImageGL::getFBO() const {
    return frameBufferObject_;
}

LayerGL* ImageGL::getColorLayerGL(size_t idx) {
    return colorLayersGL_.at(idx);
}

LayerGL* ImageGL::getDepthLayerGL() {
    return depthLayerGL_;
}

LayerGL* ImageGL::getPickingLayerGL() {
    return pickingLayerGL_;
}

const LayerGL* ImageGL::getColorLayerGL(size_t idx) const {
    return colorLayersGL_.at(idx);
}

const LayerGL* ImageGL::getDepthLayerGL() const {
    return depthLayerGL_;
}

const LayerGL* ImageGL::getPickingLayerGL() const {
    return pickingLayerGL_;
}

void ImageGL::update(bool editable) {
    bool reAttachTargets = colorLayersGL_.empty();
    colorLayersGL_.clear();
    depthLayerGL_ = NULL;
    pickingLayerGL_ = NULL;
    if(editable){
        for (size_t i=0; i<owner_->getNumberOfColorLayers(); ++i) {
            colorLayersGL_.push_back(owner_->getColorLayer(i)->getEditableRepresentation<LayerGL>());
        }

        Layer* depthLayer = owner_->getDepthLayer();
        if(depthLayer)
            depthLayerGL_ = depthLayer->getEditableRepresentation<LayerGL>();

        Layer* pickingLayer = owner_->getPickingLayer();
        if(pickingLayer)
            pickingLayerGL_ = pickingLayer->getEditableRepresentation<LayerGL>();
    }
    else{
        for (size_t i=0; i<owner_->getNumberOfColorLayers(); ++i) {
            colorLayersGL_.push_back(const_cast<LayerGL*>(owner_->getColorLayer(i)->getRepresentation<LayerGL>()));
        }

        Layer* depthLayer = owner_->getDepthLayer();
        if(depthLayer)
            depthLayerGL_ = const_cast<LayerGL*>(depthLayer->getRepresentation<LayerGL>());

        Layer* pickingLayer = owner_->getPickingLayer();
        if(pickingLayer)
            pickingLayerGL_ = const_cast<LayerGL*>(pickingLayer->getRepresentation<LayerGL>());
    }

    //Attach all targets
    if(reAttachTargets)
        reAttachAllLayers(true);
}

} // namespace
