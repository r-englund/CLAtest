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
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

#include <inviwo/core/datastructures/geometry/mesh.h>
#include <modules/opengl/buffer/elementbuffergl.h>
#include <modules/opengl/geometry/meshgl.h>
#include <modules/opengl/geometry/attributebuffergl.h>

namespace inviwo {

MeshGL::MeshGL()
    : GeometryGL() {
}

MeshGL::MeshGL(const MeshGL& rhs)
    : GeometryGL(rhs) {
}

MeshGL::~MeshGL() {
    deinitialize();
}

void MeshGL::initialize() {}

void MeshGL::deinitialize() {}

MeshGL* MeshGL::clone() const {
    return new MeshGL(*this);
}

void MeshGL::enable() const {
    for (std::vector<const BufferGL*>::const_iterator it = attributesGL_.begin(); it != attributesGL_.end(); ++it) {
        (*it)->enable();
    }

    glPushMatrix();
    mat4 modelToWorld = owner_->getWorldTransform();
    glMultMatrixf(glm::value_ptr(modelToWorld));
    mat4 dataToModel = owner_->getBasisAndOffset();
    glMultMatrixf(glm::value_ptr(dataToModel));
}

void MeshGL::disable() const {
    glPopMatrix();

    for (std::vector<const BufferGL*>::const_iterator it = attributesGL_.begin(); it != attributesGL_.end(); ++it) {
        (*it)->disable();
    }
}

void MeshGL::update(bool editable) {
    attributesGL_.clear();

    if (editable) {
        for (std::vector<Buffer*>::const_iterator it = owner_->getBuffers().begin(); it != owner_->getBuffers().end(); ++it) {
            attributesGL_.push_back((*it)->getEditableRepresentation<BufferGL>());
        }
    }
    else {
        for (std::vector<Buffer*>::const_iterator it = owner_->getBuffers().begin(); it != owner_->getBuffers().end(); ++it) {
            attributesGL_.push_back((*it)->getRepresentation<BufferGL>());
        }
    }
}

void MeshGL::setPointerToOwner(DataGroup* owner) {
    owner_ = dynamic_cast<Mesh*>(owner);
}


} // namespace

