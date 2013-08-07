#include "portgroup.h"

#include <modules/opengl/image/imagegl.h>

#include <sstream>

namespace inviwo {

PortGroup::PortGroup()
    : fbo_()
{}

void PortGroup::initialize() {
    fbo_ = new FrameBufferObject();
}

void PortGroup::deinitialize() {
    delete fbo_;
    fbo_ = 0;
}

void PortGroup::activate() {
    ivwAssert(fbo_!=0, "PortGroup not initialized.");
    //fbo_->activate();
    GLenum* attachementIDs = new GLenum[ports_.size()];
    std::vector<GLenum> attachementIDVector = fbo_->getAttachementIDs();
    for (size_t i=0; i<ports_.size(); i++)
        attachementIDs[i] = attachementIDVector[i];
    glDrawBuffers(ports_.size(), attachementIDs);
    LGL_ERROR;
    delete attachementIDs;
}

void PortGroup::deactivate() {
    ivwAssert(fbo_!=0, "PortGroup not initialized.");
    fbo_->deactivate();
}

void PortGroup::addPort(ImageOutport port) {
    ports_.push_back(&port);
    //reattachTargets();
}

void PortGroup::reattachTargets() {
    ivwAssert(fbo_!=0, "PortGroup not initialized.");
    ivwAssert(!ports_.empty(), "PortGroup is empty.");
    fbo_->activate();
    fbo_->detachAllTextures();
    for (size_t i=0; i<ports_.size(); i++) {
        const Image* image = ports_[i]->getConstData();
        if (image) {
            ImageGL* imageGL = const_cast<ImageGL*>(image->getRepresentation<ImageGL>());
            fbo_->attachTexture(imageGL->getColorTexture(), static_cast<GLenum>(GL_COLOR_ATTACHMENT0_EXT+i));
            if (i==0)
                // depth values only valid for the first render target
                fbo_->attachTexture(imageGL->getDepthTexture(), static_cast<GLenum>(GL_DEPTH_ATTACHMENT_EXT));
        }
    }
}

void PortGroup::addShaderDefines(Shader* shader) {
    for (size_t i=0; i<ports_.size(); i++) {
        shader->getFragmentShaderObject()->addShaderDefine("OP"+i);
        shader->getFragmentShaderObject()->addOutDeclaration("FragData"+i);
    }
}

} // namespace
