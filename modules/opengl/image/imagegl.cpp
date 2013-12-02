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
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/glwrap/textureunit.h>
#include <modules/opengl/canvasgl.h>

namespace inviwo {

ImageGL::ImageGL(uvec2 dimensions, ImageType type, const DataFormatBase* format, Texture2D* colorTexture, Texture2D* depthTexture, Texture2D* pickingTexture)
: ImageRepresentation(dimensions, type, format), colorTexture_(colorTexture), depthTexture_(depthTexture), pickingTexture_(pickingTexture)
{
    initialize();
}

ImageGL::~ImageGL() {
    deinitialize();
}

void ImageGL::initialize() {
    program_ = new Shader("img_copy.frag");
    program_->build();
    frameBufferObject_ = new FrameBufferObject();
    frameBufferObject_->activate();
    //if(colorTexture_) { colorTexture_->increaseRefCount(); }
    //if(depthTexture_) { depthTexture_->increaseRefCount(); }
    //if(pickingTexture_) { pickingTexture_->increaseRefCount(); }
    colorConstTexture_ = colorTexture_;
    depthConstTexture_ = depthTexture_;
    pickingConstTexture_ = pickingTexture_;
    pickingAttachmentID_ = 0;
    if(typeContainsColor(getImageType())){
        if(!colorTexture_){
            createColorLayer();
        }
        else{
            colorTexture_->bind();
        }

        frameBufferObject_->attachColorTexture(colorTexture_);
    }
    if(typeContainsDepth(getImageType())){
        if(!depthTexture_){
            createDepthLayer();
        }
        else{
            depthTexture_->bind();
        }

        frameBufferObject_->attachTexture(depthTexture_, static_cast<GLenum>(GL_DEPTH_ATTACHMENT));
    }
    if(typeContainsPicking(getImageType())){
        if(!pickingTexture_){
            createPickingLayer();
        }
        else{
            pickingTexture_->bind();
        }
        
        pickingAttachmentID_ = frameBufferObject_->attachColorTexture(pickingTexture_, 0, true);
    }
    frameBufferObject_->deactivate();
    frameBufferObject_->checkStatus();
}

void ImageGL::deinitialize() {
    frameBufferObject_->deactivate();
    delete frameBufferObject_;
    frameBufferObject_ = NULL;
    if(colorTexture_ && colorTexture_->decreaseRefCount() <= 0){
        delete colorTexture_;
        colorTexture_ = NULL;
    }
    colorConstTexture_ = NULL;
    if(depthTexture_ && depthTexture_->decreaseRefCount() <= 0){
        delete depthTexture_;
        depthTexture_ = NULL;
    }
    depthConstTexture_ = NULL;
    if(pickingTexture_ && pickingTexture_->decreaseRefCount() <= 0){
        delete pickingTexture_;
        pickingTexture_ = NULL;
    }
    pickingConstTexture_ = NULL;
    delete program_;
    program_ = 0;
}

DataRepresentation* ImageGL::clone() const {
    Texture2D* colorTexture = NULL;
    Texture2D* depthTexture = NULL;
    Texture2D* pickingTexture = NULL;
    if(colorConstTexture_)
        colorTexture = colorConstTexture_->clone();
    if(depthConstTexture_)
        depthTexture = depthConstTexture_->clone();
    if(pickingConstTexture_)
        pickingTexture = pickingConstTexture_->clone();
    ImageGL* newImageGL = new ImageGL(dimensions_, getImageType(), getDataFormat(), colorTexture, depthTexture, pickingTexture);
    return newImageGL;
}

void ImageGL::useInputSource(ImageLayerType layer, const Image* src){
    const ImageGL* srcGL = src->getRepresentation<ImageGL>();
    if(layer == COLOR_LAYER && !typeContainsColor(getImageType()) && !colorTexture_){
        colorConstTexture_ = srcGL->getColorTexture();
    }
    if(layer == DEPTH_LAYER &&!typeContainsDepth(getImageType()) && !depthTexture_){
        depthConstTexture_ = srcGL->getDepthTexture();
    }
    if(layer == PICKING_LAYER &&!typeContainsPicking(getImageType()) && !pickingTexture_){
        pickingConstTexture_ = srcGL->getPickingTexture();
    }
}

void ImageGL::createColorLayer(){
    colorTexture_ = new Texture2D(getDimensions(), getGLFormats()->getGLFormat(getDataFormatId()), GL_LINEAR);
    colorTexture_->initialize(NULL);
    //colorTexture_->increaseRefCount();
    colorConstTexture_ = colorTexture_;
}

void ImageGL::createDepthLayer(){
    depthTexture_ = new Texture2D(getDimensions(), GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT24, GL_FLOAT, GL_NEAREST);
    depthTexture_->initialize(NULL);
    //depthTexture_->increaseRefCount();
    depthConstTexture_ = depthTexture_;
}

void ImageGL::createPickingLayer(){
    pickingTexture_ = new Texture2D(getDimensions(), getGLFormats()->getGLFormat(getDataFormatId()), GL_LINEAR);
    pickingTexture_->initialize(NULL);
    //pickingTexture_->increaseRefCount();
    pickingConstTexture_ = pickingTexture_;
}

void ImageGL::createAndAddLayer(ImageLayerType layer){
    frameBufferObject_->activate();
    GLenum id = 0;

    if(layer == COLOR_LAYER){
        createColorLayer();
        id = frameBufferObject_->attachColorTexture(pickingTexture_);
        glDrawBuffer(id);
        GLuint clearColor[4] = {0, 0, 0, 0};
        glClearBufferuiv(GL_COLOR, 0, clearColor);
    }
    else if(layer == DEPTH_LAYER){
        createDepthLayer();
        frameBufferObject_->attachTexture(depthTexture_, static_cast<GLenum>(GL_DEPTH_ATTACHMENT));
        id = GL_DEPTH_ATTACHMENT;
        glDrawBuffer(id);
        //const GLfloat clearDepth = 0.f;
        //glClearBufferfv(GL_DEPTH, 0, &clearDepth);
    }
    else if(layer == PICKING_LAYER){
        createPickingLayer();
        id = frameBufferObject_->attachColorTexture(pickingTexture_, 0, true);
        glDrawBuffer(id);
        //GLuint clearColor[4] = {0, 0, 0, 0};
        //glClearBufferuiv(GL_COLOR, 0, clearColor);
    }
    else{
        frameBufferObject_->deactivate();
        return;
    }

    glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
    frameBufferObject_->detachTexture(id);
    frameBufferObject_->deactivate();
}

void ImageGL::activateBuffer() {
    invalidatePBOs();
    frameBufferObject_->activate();
    glViewport(0, 0, colorConstTexture_->getWidth(), colorConstTexture_->getHeight());
}

void ImageGL::deactivateBuffer() {
    frameBufferObject_->deactivate();
}

void ImageGL::bindColorTexture(GLenum texUnit) const {
    glActiveTexture(texUnit);
    colorConstTexture_->bind();
}

void ImageGL::bindDepthTexture(GLenum texUnit) const {
    glActiveTexture(texUnit);
    depthConstTexture_->bind();
}

void ImageGL::bindPickingTexture(GLenum texUnit) const {
    glActiveTexture(texUnit);
    pickingConstTexture_->bind();
}

void ImageGL::bindTextures(GLenum colorTexUnit, GLenum depthTexUnit) const {
    bindColorTexture(colorTexUnit);
    bindDepthTexture(depthTexUnit);
}

void ImageGL::unbindDepthTexture() const {
    depthConstTexture_->unbind();
}

void ImageGL::unbindColorTexture() const {
    colorConstTexture_->unbind();
}

void ImageGL::unbindPickingTexture() const {
    pickingConstTexture_->unbind();
}

void ImageGL::resize(uvec2 dimensions) {
    dimensions_ = dimensions;
    if(colorTexture_){
        colorTexture_->unbind();
        colorTexture_->resize(dimensions_);    
        colorTexture_->unbind();
    }
    
    if(depthTexture_){
        depthTexture_->unbind();
        depthTexture_->resize(dimensions_);
        depthTexture_->unbind();
    }

    if(pickingTexture_){
        pickingTexture_->unbind();
        pickingTexture_->resize(dimensions_);
        pickingTexture_->unbind();
    }
}

void ImageGL::invalidatePBOs(){
    if(colorTexture_)
        colorTexture_->invalidatePBO();
    if(depthTexture_)
        depthTexture_->invalidatePBO();
    if(pickingTexture_)
        pickingTexture_->invalidatePBO();
}

bool ImageGL::copyAndResizeImage(DataRepresentation* targetRep) {
    ImageGL* target = dynamic_cast<ImageGL*>(targetRep);
    ImageGL* source = this;

    //Render to FBO, with correct scaling
    target->activateBuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    TextureUnit colorUnit, depthUnit, pickingUnit;
    source->bindColorTexture(colorUnit.getEnum());
    source->bindDepthTexture(depthUnit.getEnum());
    source->bindPickingTexture(pickingUnit.getEnum());

    float ratioSource = (float)source->getDimensions().x / (float)source->getDimensions().y;
    float ratioTarget = (float)target->getDimensions().x / (float)target->getDimensions().y;

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
    source->unbindColorTexture();
    source->unbindDepthTexture();
    source->unbindPickingTexture();

    LGL_ERROR;

    return true;
}

bool ImageGL::updateFrom(const ImageGL* source) {
    ImageGL* target = this;

    //Primarily Copy by FBO blitting, all from source FBO to target FBO
    const FrameBufferObject* srcFBO = source->getFBO();
    FrameBufferObject* tgtFBO = target->getFBO();
    const Texture2D* sTex = source->getColorTexture();
    Texture2D* tTex = target->getColorTexture();

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
        sTex = source->getDepthTexture();
        tTex = target->getDepthTexture();

        if(sTex && tTex)
            tTex->uploadFromPBO(sTex);
    }
    LGL_ERROR;

    //Picking texture
    if(!pickingCopied && pickingAttachmentID_ != 0){
        sTex = source->getPickingTexture();
        tTex = target->getPickingTexture();

        if(sTex && tTex)
            tTex->uploadFromPBO(sTex);
    }
    LGL_ERROR;

    return true;
}

void ImageGL::setDimensions( uvec2 dimensions )
{
    // TODO: We do not need to resize (downscale/upscale), just change dimension.
    resize(dimensions);
}

} // namespace
