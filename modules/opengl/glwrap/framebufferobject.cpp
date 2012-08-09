#include "framebufferobject.h"

namespace inviwo {

    FrameBufferObject::FrameBufferObject() {
        glGenFramebuffersEXT(1, &id_);
    }

    FrameBufferObject::~FrameBufferObject() {}

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
        //ivwAssert(status==GL_FRAMEBUFFER_COMPLETE_EXT, "incomplete framebuffer object");
        if (status!=GL_FRAMEBUFFER_COMPLETE_EXT)
            std::cout << "FB incomplete" << std::endl;
    }

} // namespace
