#include "framebufferobject.h"

namespace inviwo {

const std::string FrameBufferObject::logSource_ = "FrameBufferObject";

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

void FrameBufferObject::attachTexture(Texture2D* texture, GLenum attachementType) {
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachementType, GL_TEXTURE_2D, texture->getID(), 0);
}

void FrameBufferObject::checkStatus() {
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
        LogWarn("incomplete framebuffer object");
}

} // namespace
