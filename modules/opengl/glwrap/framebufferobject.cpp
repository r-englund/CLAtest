#include "framebufferobject.h"

namespace inviwo {

FrameBufferObject::FrameBufferObject() {
    glGenFramebuffersEXT(1, &id_);
}

FrameBufferObject::~FrameBufferObject() {
    glDeleteFramebuffersEXT(1, &id_);
}

void FrameBufferObject::activate() {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id_);
}

void FrameBufferObject::deactivate() {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void FrameBufferObject::attachTexture(Texture2D* texture, GLenum attachementID) {
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachementID, GL_TEXTURE_2D, texture->getID(), 0);
    GLint maxColorAttachements;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &maxColorAttachements);
    if (attachedColorTextures_.size() < (unsigned int)maxColorAttachements) {
        // manage color textures in dedicated vector
        if (attachementID >= GL_COLOR_ATTACHMENT0_EXT && attachementID <= GL_COLOR_ATTACHMENT15_EXT)
            attachedColorTextures_.push_back(attachementID);
    } else
        LogError("Maximum number of " << maxColorAttachements << " color textures attached.");
}

void FrameBufferObject::attachTexture(Texture2D* texture, int attachementNumber, bool attachFromRear) {
    GLint maxColorAttachements;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &maxColorAttachements);
    GLenum attachementID = (attachFromRear ? static_cast<GLenum>(GL_COLOR_ATTACHMENT0_EXT+maxColorAttachements-attachementNumber-1) : static_cast<GLenum>(GL_COLOR_ATTACHMENT0_EXT+attachementNumber));
    attachTexture(texture, attachementID);
}

void FrameBufferObject::detachTexture(GLenum attachementID) {
    for (size_t i=0; i<attachedColorTextures_.size(); i++) {
        if (attachedColorTextures_[i] == attachementID) {
            attachedColorTextures_.erase(attachedColorTextures_.begin()+i);
            return;
        }
    }
}

void FrameBufferObject::detachAllTextures() {
    while (!attachedColorTextures_.empty())
        detachTexture(attachedColorTextures_[0]);
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

void FrameBufferObject::setRead_Blit(bool set) {
    if (set)
        glBindFramebufferEXT(GL_READ_FRAMEBUFFER, id_ );
    else
        glBindFramebufferEXT(GL_READ_FRAMEBUFFER, 0 );
}

void FrameBufferObject::setDraw_Blit(bool set) {
    if (set)
        glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, id_ );
    else
        glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, 0 );
}


} // namespace
