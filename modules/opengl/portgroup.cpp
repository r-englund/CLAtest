/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file authors: Timo Ropinski, Erik Sundén
 *
 *********************************************************************************/

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
    frameBufferObject_ = NULL;
    delete ports_;
}

void PortGroup::activate() {
    ivwAssert(frameBufferObject_!=NULL, "PortGroup not initialized.");
    frameBufferObject_->activate();
    frameBufferObject_->defineDrawBuffers();
    LGL_ERROR;
}

void PortGroup::deactivate() {
    ivwAssert(frameBufferObject_!=NULL, "PortGroup not initialized.");
    frameBufferObject_->deactivate();
}

void PortGroup::addPort(ImageOutport& port) {
    ports_->push_back(&port);
}

void PortGroup::reattachTargets() {
    ivwAssert(frameBufferObject_!=NULL, "PortGroup not initialized.");
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
            frameBufferObject_->attachTexture(images[i]->getDepthLayerGL()->getTexture(), static_cast<GLenum>(GL_DEPTH_ATTACHMENT));
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
