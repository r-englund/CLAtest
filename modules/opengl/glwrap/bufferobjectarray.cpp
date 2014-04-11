/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
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
 * Main file authors: Erik Sundén
 *
 *********************************************************************************/

#include <modules/opengl/glwrap/bufferobjectarray.h>
#include <modules/opengl/glwrap/bufferobject.h>

namespace inviwo {

BufferObjectArray::BufferObjectArray(){
    initialize();
}

BufferObjectArray::BufferObjectArray(const BufferObjectArray& rhs) {
    initialize();

    for (std::vector<const BufferObject*>::const_iterator it = rhs.attachedBuffers_.begin(); it != rhs.attachedBuffers_.end(); ++it) {
        attachBufferObject((*it));
    }
}

BufferObjectArray* BufferObjectArray::clone() const {
    return new BufferObjectArray(*this);
}

BufferObjectArray::~BufferObjectArray() {
    deinitialize();
}

void BufferObjectArray::initialize() {
    glGenVertexArrays(1, &id_);
}

void BufferObjectArray::deinitialize() {
    glDeleteVertexArrays(1, &id_);
}

GLuint BufferObjectArray::getId() const {
    return id_;
}

void BufferObjectArray::enable() const {
    glEnableVertexAttribArray(id_);
}

void BufferObjectArray::disable() const {
    glDisableVertexAttribArray(0);
}

void BufferObjectArray::bind() const {
    glBindVertexArray(id_);
}

void BufferObjectArray::attachBufferObject(const BufferObject* bo) {
    bo->bind();
    glVertexAttribPointer((GLuint)0, bo->getGLFormat().channels, bo->getGLFormat().type, GL_FALSE, 0, 0);
    bo->unbind();
}

const BufferObject* BufferObjectArray::getBufferObject(size_t idx) const{
    if(idx<attachedBuffers_.size())
        return attachedBuffers_[idx];
    else
        return NULL;
}

} // namespace

