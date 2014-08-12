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

BufferObjectArray::BufferObjectArray() : id_(0u) {
    initialize();
}

BufferObjectArray::BufferObjectArray(const BufferObjectArray& rhs) : id_(0u) {
    initialize();

    bind();
    for (std::vector<const BufferObject*>::const_iterator it = rhs.attachedBuffers_.begin(); it != rhs.attachedBuffers_.end(); ++it) {
        attachBufferObject((*it));
    }
    unbind();
}

BufferObjectArray* BufferObjectArray::clone() const {
    return new BufferObjectArray(*this);
}

BufferObjectArray::~BufferObjectArray() {
    deinitialize();
}

void BufferObjectArray::initialize() {
    glGenVertexArrays(1, &id_);
    attachedBuffers_.resize(NUMBER_OF_BUFFER_TYPES);
    attachedNum_ = 0;
}

void BufferObjectArray::deinitialize() {
    glDeleteVertexArrays(1, &id_);
}

GLuint BufferObjectArray::getId() const {
    return id_;
}

void BufferObjectArray::clear() {
    for (GLuint i=0; i < static_cast<GLuint>(attachedBuffers_.size()); ++i) {
        glDisableVertexAttribArray(i);
        attachedBuffers_[i] = NULL;
    }
    attachedNum_ = 0;
}

void BufferObjectArray::bind() const {
    glBindVertexArray(id_);
}

void BufferObjectArray::unbind() const {
    glBindVertexArray(0);
}

void BufferObjectArray::attachBufferObjectToGenericLocation(const BufferObject* bo){
    if(!attachedBuffers_[bo->getBufferType()]){
        pointToObject(bo, static_cast<GLuint>(bo->getBufferType()));
        attachedBuffers_[bo->getBufferType()] = bo;
        attachedNum_++;
    }
    else{
        attachBufferObject(bo, static_cast<GLuint>(attachedNum_));
    }
}

void BufferObjectArray::attachBufferObject(const BufferObject* bo){
    attachBufferObject(bo, static_cast<GLuint>(attachedNum_));
}

void BufferObjectArray::attachBufferObject(const BufferObject* bo, GLuint location) {
    if(bo)
        pointToObject(bo, location);

    attachedBuffers_.insert(attachedBuffers_.begin()+location, bo);
    attachedNum_++;
}

void BufferObjectArray::pointToObject(const BufferObject* bo, GLuint location) {
    glEnableVertexAttribArray(location);
    bo->bind();
    glVertexAttribPointer(location, bo->getGLFormat().channels, bo->getGLFormat().type, GL_FALSE, 0, (void*)0);
}

const BufferObject* BufferObjectArray::getBufferObject(size_t idx) const{
    if(idx<attachedBuffers_.size())
        return attachedBuffers_[idx];
    else
        return NULL;
}

} // namespace

