/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include "portgroup.h"
#include <inviwo/core/ports/imageport.h>
#include <modules/opengl/image/imagegl.h>
#include <modules/opengl/glwrap/framebufferobject.h>
#include <modules/opengl/glwrap/shader.h>

namespace inviwo {

PortGroup::PortGroup()
    : frameBufferObject_()
{
    ports_ = new std::vector<ImageOutport*>();
}

void PortGroup::initialize() {
    frameBufferObject_ = new FrameBufferObject();
}

void PortGroup::deinitialize() {
    delete frameBufferObject_;
    frameBufferObject_ = 0;
    delete ports_;
}

void PortGroup::activate() {
    ivwAssert(frameBufferObject_!=0, "PortGroup not initialized.");
    frameBufferObject_->activate();
    frameBufferObject_->defineDrawBuffers();
    LGL_ERROR;
}

void PortGroup::deactivate() {
    ivwAssert(frameBufferObject_!=0, "PortGroup not initialized.");
    frameBufferObject_->deactivate();
}

void PortGroup::addPort(ImageOutport& port) {
    ports_->push_back(&port);
}

void PortGroup::reattachTargets() {
    ivwAssert(frameBufferObject_!=0, "PortGroup not initialized.");
    ivwAssert(!ports_->empty(), "PortGroup is empty.");

    // acquire all images to be attached
    std::vector<ImageGL*> images;
    for (size_t i=0; i<ports_->size(); i++) {
        Image* image = ports_->at(i)->getData();
        if (image)
            images.push_back(image->getEditableRepresentation<ImageGL>());
        else LogWarn("Empty image in outport.");
    }

    // attach textures to FBO
    frameBufferObject_->activate();
    frameBufferObject_->defineDrawBuffers();
    frameBufferObject_->detachAllTextures();
    for (unsigned int i=0; i<static_cast<unsigned int>(images.size()); i++) {
        frameBufferObject_->attachColorTexture(images[i]->getColorLayerGL()->getTexture(), i);
        if (i==0)
            // depth values only valid for the first render target
            frameBufferObject_->attachTexture(images[i]->getDepthLayerGL()->getTexture(), static_cast<GLenum>(GL_DEPTH_ATTACHMENT_EXT));
    }

    LGL_ERROR;
    frameBufferObject_->checkStatus();
    frameBufferObject_->deactivate();
}

void PortGroup::addShaderDefines(Shader* shader) {
    for (size_t i=0; i<ports_->size(); i++) {
        shader->getFragmentShaderObject()->addShaderDefine("OP"+toString(i));
        if (i!=0)
            // FragData0 is already defined in openglinfo.cpp
            shader->getFragmentShaderObject()->addOutDeclaration("FragData"+toString(i));
    }
}

} // namespace
