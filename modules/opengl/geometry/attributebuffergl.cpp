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
 * Main file authors: Daniel Jönsson, Erik Sundén
 *
 *********************************************************************************/

#include <modules/opengl/geometry/attributebuffergl.h>

namespace inviwo {

BufferGL::BufferGL(size_t size, const DataFormatBase* format, BufferType type, BufferUsage usage)
    : BufferRepresentation(size, format, type, usage)
    , glFormat_(getGLFormats()->getGLFormat(format->getId())){
    initialize();
    LGL_ERROR_SUPPRESS;
}

BufferGL::~BufferGL() {
    deinitialize();
}

GLenum BufferGL::getFormatType() const {
    return glFormat_.type;
}

GLuint BufferGL::getId() const {
    return bufferId_->getId();
}

void BufferGL::enable() const {
    glEnableClientState(state_);
    bind();
    specifyLocation();
}

void BufferGL::disable() const {
    glDisableClientState(state_);
}

void BufferGL::bind() const {
    glBindBuffer(target_, bufferId_->getId());
}

void BufferGL::specifyLocation() const {
    (this->*locationPointerFunc_)();
}


void BufferGL::initialize( const void* data, GLsizeiptr sizeInBytes, GLenum target /*= GL_ARRAY_BUFFER*/ )
{
    //Set global variables
    target_ = target;

    //Get GL Format
    glFormat_ = getGLFormats()->getGLFormat(getDataFormat()->getId());

    //Specify location and state
    switch(getBufferType())
    {
    case COLOR_ATTRIB:
        locationPointerFunc_ = &BufferGL::colorPointer;
        state_ = GL_COLOR_ARRAY;
        break;
    case NORMAL_ATTRIB:
        locationPointerFunc_ = &BufferGL::normalPointer;
        state_ = GL_NORMAL_ARRAY;
        break;
    case TEXCOORD_ATTRIB:
        locationPointerFunc_ = &BufferGL::texCoordPointer;
        state_ = GL_TEXTURE_COORD_ARRAY;
        break;
    case POSITION_ATTRIB:
        locationPointerFunc_ = &BufferGL::vertexPointer;
        state_ = GL_VERTEX_ARRAY;
        break;
    default:
        locationPointerFunc_ = &BufferGL::emptyFunc;
        state_ = GL_VERTEX_ARRAY;
        break;
    }

    //Specify location and state
    switch(getBufferUsage())
    {
        case DYNAMIC:
            usageGL_ = GL_DYNAMIC_DRAW;
            break;
        default:
            usageGL_ = GL_STATIC_DRAW;
            break;
    }

    bind();
    // Allocate and transfer possible data
    glBufferData(target_, sizeInBytes, data, usageGL_);
    specifyLocation();
}

void BufferGL::upload( const void* data, GLsizeiptr sizeInBytes ) {
    bind();
    glBufferSubData(target_, 0, sizeInBytes, data);
}

void BufferGL::colorPointer() const {
    glColorPointer(glFormat_.channels, glFormat_.type, 0, 0);
}

void BufferGL::normalPointer() const {
    glNormalPointer(glFormat_.type, 0, 0);
}

void BufferGL::texCoordPointer() const {
    glTexCoordPointer(glFormat_.channels, glFormat_.type, 0, 0);
}

void BufferGL::vertexPointer() const {
    glVertexPointer(glFormat_.channels, glFormat_.type, 0, 0);
}

void BufferGL::emptyFunc() const {}

BufferGL* BufferGL::clone() const{
    return new BufferGL(*this);
}

void BufferGL::initialize(){
    //Generate a new buffer
    GLuint id;
    glGenBuffers(1, &id);
    bufferId_ = new BufferGLObjectId(id);
}

void BufferGL::deinitialize() {
    if(bufferId_->decreaseRefCount() <= 0) {
        delete bufferId_; bufferId_ = NULL;
    }
}

void BufferGL::download( void* data ) const
{
    bind();
    // Map data
    void* gldata = glMapBuffer(target_, GL_READ_ONLY);
    // Copy data if valid pointer
    if(gldata)
    {
        memcpy(data, gldata, getSize()*getSizeOfElement());
        // Unmap buffer after using it
        glUnmapBufferARB(target_); 
    } else {
        LogError("Unable to map data");
    }

}




} // namespace

