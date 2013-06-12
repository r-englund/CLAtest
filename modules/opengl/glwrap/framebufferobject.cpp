#include "framebufferobject.h"

namespace inviwo {

FrameBufferObject::FrameBufferObject() {
    glGenFramebuffersEXT(1, &id_);
}

FrameBufferObject::~FrameBufferObject() {
    glDeleteFramebuffers(1, &id_);
}

void FrameBufferObject::activate() {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id_);
}

void FrameBufferObject::deactivate() {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void FrameBufferObject::attachTexture(Texture2D* texture, GLenum attachementID) {
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachementID, GL_TEXTURE_2D, texture->getID(), 0);
    attachedTextures_.push_back(attachementID);
}

void FrameBufferObject::detachTexture(GLenum attachementID) {
    for (size_t i=0; i<attachedTextures_.size(); i++) {
        if (attachedTextures_[i] == attachementID) {
            attachedTextures_.erase(attachedTextures_.begin()+i);
            return;
        }
    }
}

void FrameBufferObject::detachAllTextures() {
    while (!attachedTextures_.empty())
        detachTexture(attachedTextures_[0]);
}

void FrameBufferObject::checkStatus() {
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
        LogWarn("incomplete framebuffer object");
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
