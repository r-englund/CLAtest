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
 * Main file authors: Daniel Jönsson
 *
 *********************************************************************************/

#include <modules/opengl/glwrap/bufferobject.h>

namespace inviwo {

BufferObject::BufferObject(size_t size, const DataFormatBase* format, BufferType type, BufferUsage usage,
                           GLenum target /*= GL_ARRAY_BUFFER*/)
    : Observable<BufferObjectObserver>(), ReferenceCounter()
    , target_(target)
    , type_(type), glFormat_(getGLFormats()->getGLFormat(format->getId())) {
    //
    switch (usage)
    {
        case DYNAMIC:
            usageGL_ = GL_DYNAMIC_DRAW;
            break;

        default:
            usageGL_ = GL_STATIC_DRAW;
            break;
    }

    initialize();
    LGL_ERROR_SUPPRESS;
}

BufferObject::BufferObject(const BufferObject& rhs) {
    // TODO: Copy data
}

BufferObject::~BufferObject() {
    deinitialize();
}

void BufferObject::initialize() {
    // Generate a new buffer
    glGenBuffers(1, &id_);
}

void BufferObject::deinitialize() {
    glDeleteBuffers(1, &id_);
}

GLenum BufferObject::getFormatType() const {
    return glFormat_.type;
}

GLuint BufferObject::getId() const {
    return id_;
}

void BufferObject::enable() const {
    glEnableClientState(state_);
    bind();
    specifyLocation();
}

void BufferObject::disable() const {
    glDisableClientState(state_);
}

void BufferObject::bind() const {
    glBindBuffer(target_, id_);
}

void BufferObject::specifyLocation() const {
    (this->*locationPointerFunc_)();
}


void BufferObject::initialize(const void* data, GLsizeiptr sizeInBytes) {
    sizeInBytes_ = sizeInBytes;

    //Specify location and state
    switch (getBufferType())
    {
        case COLOR_ATTRIB:
            locationPointerFunc_ = &BufferObject::colorPointer;
            state_ = GL_COLOR_ARRAY;
            break;

        case NORMAL_ATTRIB:
            locationPointerFunc_ = &BufferObject::normalPointer;
            state_ = GL_NORMAL_ARRAY;
            break;

        case TEXCOORD_ATTRIB:
            locationPointerFunc_ = &BufferObject::texCoordPointer;
            state_ = GL_TEXTURE_COORD_ARRAY;
            break;

        case POSITION_ATTRIB:
            locationPointerFunc_ = &BufferObject::vertexPointer;
            state_ = GL_VERTEX_ARRAY;
            break;

        default:
            locationPointerFunc_ = &BufferObject::emptyFunc;
            state_ = GL_VERTEX_ARRAY;
            break;
    }

    // Notify observers
    ObserverSet::iterator endIt = observers_->end();

    for (ObserverSet::iterator it = observers_->begin(); it != endIt; ++it) {
        // static_cast can be used since only template class objects can be added
        static_cast<BufferObjectObserver*>(*it)->notifyBeforeBufferInitialization();
    }

    bind();
    // Allocate and transfer possible data
    glBufferData(target_, sizeInBytes, data, usageGL_);
    specifyLocation();

    for (ObserverSet::iterator it = observers_->begin(); it != endIt; ++it) {
        // static_cast can be used since only template class objects can be added
        static_cast<BufferObjectObserver*>(*it)->notifyAfterBufferInitialization();
    }
}

void BufferObject::upload(const void* data, GLsizeiptr sizeInBytes) {
    bind();
    glBufferSubData(target_, 0, sizeInBytes, data);
}

void BufferObject::download(void* data) const {
    bind();
    // Map data
    void* gldata = glMapBuffer(target_, GL_READ_ONLY);

    // Copy data if valid pointer
    if (gldata)
    {
        memcpy(data, gldata, sizeInBytes_);
        // Unmap buffer after using it
        glUnmapBufferARB(target_);
    } else {
        LogError("Unable to map data");
    }
}

void BufferObject::colorPointer() const {
    glColorPointer(glFormat_.channels, glFormat_.type, 0, 0);
}

void BufferObject::normalPointer() const {
    glNormalPointer(glFormat_.type, 0, 0);
}

void BufferObject::texCoordPointer() const {
    glTexCoordPointer(glFormat_.channels, glFormat_.type, 0, 0);
}

void BufferObject::vertexPointer() const {
    glVertexPointer(glFormat_.channels, glFormat_.type, 0, 0);
}

void BufferObject::emptyFunc() const {}

BufferObject* BufferObject::clone() const {
    return new BufferObject(*this);
}








} // namespace

