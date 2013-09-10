#include "portgroup.h"

#include <modules/opengl/image/imagegl.h>

namespace inviwo {

PortGroup::PortGroup()
    : frameBufferObject_()
{}

void PortGroup::initialize() {
    frameBufferObject_ = new FrameBufferObject();
}

void PortGroup::deinitialize() {
    delete frameBufferObject_;
    frameBufferObject_ = 0;
}

void PortGroup::activate() {
    ivwAssert(frameBufferObject_!=0, "PortGroup not initialized.");
    frameBufferObject_->activate();

    std::vector<GLenum> attachementIDVector = frameBufferObject_->getColorAttachementIDs();
    GLenum* attachementIDs = new GLenum[attachementIDVector.size()];
    for (size_t i=0; i<attachementIDVector.size(); i++)
        attachementIDs[i] = attachementIDVector[i];
    glDrawBuffers(static_cast<GLsizei>(attachementIDVector.size()), attachementIDs);
    delete attachementIDs;
    LGL_ERROR;
}

void PortGroup::deactivate() {
    ivwAssert(frameBufferObject_!=0, "PortGroup not initialized.");
    frameBufferObject_->deactivate();
}

void PortGroup::addPort(ImageOutport& port) {
    ports_.push_back(&port);
}

void PortGroup::reattachTargets() {
    ivwAssert(frameBufferObject_!=0, "PortGroup not initialized.");
    ivwAssert(!ports_.empty(), "PortGroup is empty.");

    // acquire all images to be attached
    std::vector<ImageGL*> images;
    for (size_t i=0; i<ports_.size(); i++) {
        const Image* image = ports_[i]->getConstData();
        if (image)
            images.push_back(const_cast<ImageGL*>(image->getRepresentation<ImageGL>()));
        else LogWarn("Empty image in outport.");
    }

    // attach textures to FBO
    frameBufferObject_->activate();
    frameBufferObject_->detachAllTextures();
    for (size_t i=0; i<images.size(); i++) {
        frameBufferObject_->attachTexture(images[i]->getColorTexture(), static_cast<GLenum>(GL_COLOR_ATTACHMENT0_EXT+i));
        if (i==0)
            // depth values only valid for the first render target
            frameBufferObject_->attachTexture(images[i]->getDepthTexture(), static_cast<GLenum>(GL_DEPTH_ATTACHMENT_EXT));
    }

    LGL_ERROR;
    frameBufferObject_->checkStatus();
    frameBufferObject_->deactivate();
}

void PortGroup::addShaderDefines(Shader* shader) {
    for (size_t i=0; i<ports_.size(); i++) {
        shader->getFragmentShaderObject()->addShaderDefine("OP"+toString(i));
        if (i!=0)
            // FragData0 is already defined in openglinfo.cpp
            shader->getFragmentShaderObject()->addOutDeclaration("FragData"+toString(i));
    }
}

} // namespace
