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

#include "framebufferobject.h"

namespace inviwo {

const GLenum FrameBufferObject::colorAttachmentEnums_[] = {
    GL_COLOR_ATTACHMENT0_EXT,GL_COLOR_ATTACHMENT1_EXT,GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT, GL_COLOR_ATTACHMENT4_EXT,GL_COLOR_ATTACHMENT5_EXT,
    GL_COLOR_ATTACHMENT6_EXT,GL_COLOR_ATTACHMENT7_EXT,GL_COLOR_ATTACHMENT8_EXT, GL_COLOR_ATTACHMENT9_EXT,GL_COLOR_ATTACHMENT10_EXT,
    GL_COLOR_ATTACHMENT11_EXT,GL_COLOR_ATTACHMENT12_EXT,GL_COLOR_ATTACHMENT13_EXT, GL_COLOR_ATTACHMENT14_EXT,GL_COLOR_ATTACHMENT15_EXT};

FrameBufferObject::FrameBufferObject() {
    glGenFramebuffersEXT(1, &id_);
    hasDepthAttachment_ = false;
    hasStencilAttachment_ = false;

    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &maxColorAttachements_);

    drawBuffers_ = new GLenum[maxColorAttachements_];

    for(int i=0; i < maxColorAttachements_; i++)
        drawBuffers_[i] = GL_NONE;
}

FrameBufferObject::~FrameBufferObject() {
    glDeleteFramebuffersEXT(1, &id_);
    delete drawBuffers_;
}

void FrameBufferObject::activate() {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id_);
    glDrawBuffers(static_cast<GLsizei>(maxColorAttachements_), drawBuffers_);
}

void FrameBufferObject::deactivate() {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void FrameBufferObject::attachTexture(Texture2D* texture, GLenum attachementID) {
    attachTexture(attachementID);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachementID, GL_TEXTURE_2D, texture->getID(), 0);
}

GLenum FrameBufferObject::attachColorTexture(Texture2D* texture) {
    GLenum attachementID;
    if(attachColorTexture(attachementID)){
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachementID, GL_TEXTURE_2D, texture->getID(), 0);
    }
    return attachementID;
}

GLenum FrameBufferObject::attachColorTexture(Texture2D* texture, int attachementNumber, bool attachFromRear) {
    GLenum attachementID;
    if(attachColorTexture(attachementID, attachementNumber, attachFromRear)){
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachementID, GL_TEXTURE_2D, texture->getID(), 0);
    }
    return attachementID;
}

void FrameBufferObject::attachTexture(Texture3D* texture, GLenum attachementID, int layer) {
    attachTexture(attachementID);
    glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, attachementID, GL_TEXTURE_3D, texture->getID(), 0, layer);
}

GLenum FrameBufferObject::attachColorTexture(Texture3D* texture, int layer) {
    GLenum attachementID;
    if(attachColorTexture(attachementID)){
        glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, attachementID, GL_TEXTURE_3D, texture->getID(), 0, layer);
    }
    return attachementID;
}

GLenum FrameBufferObject::attachColorTexture(Texture3D* texture, int attachementNumber, int layer, bool attachFromRear) {
    GLenum attachementID;
    if(attachColorTexture(attachementID, attachementNumber, attachFromRear)){
        glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, attachementID, GL_TEXTURE_3D, texture->getID(), 0, layer);
    }
    return attachementID;
}

void FrameBufferObject::detachTexture(GLenum attachementID) {
    if(attachementID == GL_DEPTH_ATTACHMENT)
        hasDepthAttachment_ = false;
    else if(attachementID == GL_STENCIL_ATTACHMENT)
        hasStencilAttachment_ = false;
    else{
        int attachementNumber = -1;
        for(int i=0; i<maxColorAttachements_; i++){
            if(colorAttachmentEnums_[i] == attachementID){
                attachementNumber = i;
                break;
            }
        }

        if(attachementNumber>=0){
            drawBuffers_[attachementNumber] = GL_NONE;
        }
        else{
            LogError("Could not detach " << attachementID << " from framebuffer");
            return;
        }
    }

    glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, attachementID, 0, 0);    
}

void FrameBufferObject::detachAllTextures() {
    detachTexture(GL_DEPTH_ATTACHMENT);
    detachTexture(GL_STENCIL_ATTACHMENT);

    for(int i=0; i<maxColorAttachements_; i++){
        if(drawBuffers_[i] != GL_NONE){
            glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, drawBuffers_[i], 0, 0);  
            drawBuffers_[i] = GL_NONE;
        }
    }
}

unsigned int FrameBufferObject::getID() const{
    return id_;
}

const GLenum* FrameBufferObject::getDrawBuffers() const { 
    return drawBuffers_; 
}

int FrameBufferObject::getMaxColorAttachments() const { 
    return maxColorAttachements_; 
}

bool FrameBufferObject::hasColorAttachment() const {
    for(int i=0; i<maxColorAttachements_; i++){
        if(drawBuffers_[i] != GL_NONE){
            return true;
        }
    }
    return false;
}

bool FrameBufferObject::hasDepthAttachment() const { 
    return hasDepthAttachment_; 
}

bool FrameBufferObject::hasStencilAttachment() const { 
    return hasStencilAttachment_; 
}

void FrameBufferObject::checkStatus() {
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch (status) {
        case GL_FRAMEBUFFER_UNDEFINED :
            LogWarn("GL_FRAMEBUFFER_UNDEFINED"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT :
            LogWarn("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT :
            LogWarn("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT :
            LogWarn("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT :
            LogWarn("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"); break;
        case GL_FRAMEBUFFER_UNSUPPORTED_EXT :
            LogWarn("GL_FRAMEBUFFER_UNSUPPORTED"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT :
            LogWarn("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT :
            LogWarn("GL_FRAMEBUFFER_INCOMPLETE_FORMATS"); break;
    }
}

void FrameBufferObject::setRead_Blit(bool set) const {
    if (set)
        glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, id_ );
    else
        glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, 0 );
}

void FrameBufferObject::setDraw_Blit(bool set) {
    if (set)
        glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, id_ );
    else
        glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0 );
}

void FrameBufferObject::attachTexture(GLenum attachementID) {
    if(attachementID == GL_DEPTH_ATTACHMENT)
        hasDepthAttachment_ = true;
    else if(attachementID == GL_STENCIL_ATTACHMENT)
        hasStencilAttachment_ = true;
    else{
        int attachementNumber = -1;
        for(int i=0; i<maxColorAttachements_; i++){
            if(colorAttachmentEnums_[i] == attachementID){
                attachementNumber = i;
                break;
            }
        }

        if(attachementNumber>=0){
            drawBuffers_[attachementNumber] = attachementID;
        }
        else{
            LogError("Attachments ID " << attachementID << " exceeds maximum amount of color attachments");
            return;
        }
    }
}

bool FrameBufferObject::attachColorTexture(GLenum& outAttachNumber) {
    int attachementNumber = -1;
    for(int i=0; i<maxColorAttachements_; i++){
        if(drawBuffers_[i] == GL_NONE){
            attachementNumber = i;
            break;
        }
    }

    if(attachementNumber>=0){
        drawBuffers_[attachementNumber] = static_cast<GLenum>(GL_COLOR_ATTACHMENT0_EXT+attachementNumber);
        outAttachNumber = drawBuffers_[attachementNumber];
        return true;
    }
    else{
        LogError("Attachments number exceeds maximum amount of color attachments");
        outAttachNumber = GL_COLOR_ATTACHMENT15_EXT;
        return false;
    }
}

bool FrameBufferObject::attachColorTexture(GLenum& outAttachNumber, int attachementNumber, bool attachFromRear) {
    attachementNumber = (attachFromRear ? maxColorAttachements_-attachementNumber-1 : attachementNumber);
    GLenum attachementID = static_cast<GLenum>(GL_COLOR_ATTACHMENT0_EXT+attachementNumber);
    outAttachNumber = attachementID;

    if(attachementNumber<maxColorAttachements_ && attachementNumber>=0){
        drawBuffers_[attachementNumber] = attachementID;
        return true;
    }
    else{
        LogError("AttachmentID " << attachementID << " exceeds maximum amount of color attachments");
        return false;
    }
}


} // namespace
