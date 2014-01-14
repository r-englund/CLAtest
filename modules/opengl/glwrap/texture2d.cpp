/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include "texture2d.h"

namespace inviwo {

Texture2D::Texture2D(uvec2 dimensions, GLFormats::GLFormat glFormat, GLenum filtering)
    : Observable<TextureObserver>(), ReferenceCounter()
    , dimensions_(dimensions)
    , format_(glFormat.format)
    , internalformat_(glFormat.internalFormat)
    , dataType_(glFormat.type)
    , filtering_(filtering) {
        
    glGenTextures(1, &id_);
    numChannels_ = glFormat.channels;
    byteSize_ = numChannels_*glFormat.typeSize;
    glGenBuffers(1, &pboBack_);
    LGL_ERROR_SUPPRESS;
    setupAsyncReadBackPBO();
}

Texture2D::Texture2D(uvec2 dimensions, GLint format, GLint internalformat, GLenum dataType, GLenum filtering)
    : Observable<TextureObserver>(), ReferenceCounter()
    , dimensions_(dimensions)
    , format_(format)
    , internalformat_(internalformat)
    , dataType_(dataType)
    , filtering_(filtering) {
        
    glGenTextures(1, &id_);
    setNChannels();
    setSizeInBytes();
    glGenBuffers(1, &pboBack_);
    LGL_ERROR_SUPPRESS;
    setupAsyncReadBackPBO();
}

Texture2D::Texture2D( const Texture2D& other )
    : Observable<TextureObserver>(), ReferenceCounter()
    , dimensions_(other.dimensions_)
    , format_(other.format_)
    , internalformat_(other.internalformat_)
    , dataType_(other.dataType_)
    , filtering_(other.filtering_)
    , byteSize_(other.byteSize_)
    , numChannels_(other.numChannels_) {
        
    glGenTextures(1, &id_);
    glGenBuffers(1, &pboBack_);
    LGL_ERROR_SUPPRESS;
    setupAsyncReadBackPBO();
    initialize(NULL);
    
    // TODO: Copy texture from other
    // bind();
    // glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, dimensions_.x, dimensions_.y);
}

Texture2D& Texture2D::operator=( const Texture2D& other ) {
    if (this != &other) {
        // Check if this object is shared with OpenCL/CUDA/DirectX
        if(getRefCount() > 1) {
            LogError("This object is shared and cannot changed (size/format etc.) until the shared object has been released");
        }

        dimensions_ = other.dimensions_;
        format_ = other.format_;
        internalformat_ = other.internalformat_;
        dataType_ = other.dataType_;
        filtering_ = other.filtering_;
        numChannels_ = other.numChannels_;
        byteSize_ = other.byteSize_;
        initialize(NULL);
        // TODO: Copy other texture content
    }
    return *this;
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &id_);
    glDeleteBuffers(1, &pboBack_);
    LGL_ERROR;
}

Texture2D* Texture2D::clone() const {
    Texture2D* cloneTexture = new Texture2D(dimensions_, format_, internalformat_, dataType_, filtering_);
    // TODO: Copy texture content
    // glCopyTexImage2D(
    
    return cloneTexture;
}

void Texture2D::bind() const{
    glBindTexture(GL_TEXTURE_2D, id_);
    LGL_ERROR;
}

void Texture2D::bindFromPBO() const{
    if(!dataInReadBackPBO_){
        downloadToPBO();
    }
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, pboBack_);
    LGL_ERROR;
}

void Texture2D::bindToPBO() const{
    glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, pboBack_);
    LGL_ERROR;
}

void Texture2D::initialize(const void* data) {
    // Notify observers
    ObserverSet::iterator endIt = observers_->end();
    for(ObserverSet::iterator it = observers_->begin(); it != endIt; ++it) {
        // static_cast can be used since only template class objects can be added
        static_cast<TextureObserver*>(*it)->notifyBeforeTextureInitialization();    
    }
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, internalformat_, dimensions_.x, dimensions_.y, 0, format_, dataType_, data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering_);
    LGL_ERROR;
    for(ObserverSet::iterator it = observers_->begin(); it != endIt; ++it) {
        // static_cast can be used since only template class objects can be added
        static_cast<TextureObserver*>(*it)->notifyAfterTextureInitialization();    
    }
}

void Texture2D::uploadFromPBO(const Texture2D* src){
    src->bindFromPBO();
    upload(NULL);
    unbind();
    src->unbindFromPBO();
    LGL_ERROR;
}

void Texture2D::upload( const void* data )
{
    bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, dimensions_.x, dimensions_.y, format_, dataType_, data);
    LGL_ERROR;
}

void Texture2D::download(void* data) const {
    if(dataInReadBackPBO_){
        // Copy from PBO
        bindToPBO();
        void* mem = glMapBuffer(GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY);   
        assert(mem);
        memcpy(data, mem, dimensions_.x*dimensions_.y*getSizeInBytes());

        //Release PBO data
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER_ARB);
        unbindToPBO();
        dataInReadBackPBO_ = false;
    }
    else{
        bind();
        glGetTexImage(GL_TEXTURE_2D, 0, format_, dataType_, data);
        unbind();
    }
    LGL_ERROR;
}

void Texture2D::downloadToPBO() const{
    if(!dataInReadBackPBO_){
        bind();
        bindToPBO();
        glGetTexImage(GL_TEXTURE_2D, 0, format_, dataType_, 0);
        unbindToPBO();
        dataInReadBackPBO_ = true;
    }
}

void Texture2D::unbind() const{
    glBindTexture(GL_TEXTURE_2D, 0);
    LGL_ERROR_SUPPRESS;
}

void Texture2D::unbindFromPBO() const{
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
    LGL_ERROR;
}

void Texture2D::unbindToPBO() const{
    glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0);
    LGL_ERROR;
}

void Texture2D::invalidatePBO(){
    dataInReadBackPBO_ = false;
}

void Texture2D::resize(uvec2 dimension) {
    setWidth(dimension.x);
    setHeight(dimension.y);
    initialize(NULL);
    setupAsyncReadBackPBO();
}

void Texture2D::setupAsyncReadBackPBO(){
    bind();
    glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, pboBack_);
    glBufferDataARB(GL_PIXEL_PACK_BUFFER_ARB, dimensions_.x*dimensions_.y*getSizeInBytes(), NULL, GL_STREAM_READ_ARB);
    glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, 0);
    unbind();
    dataInReadBackPBO_ = false;
    LGL_ERROR;
}

void Texture2D::setNChannels(){
    switch (format_) {
        case GL_STENCIL_INDEX:
        case GL_DEPTH_COMPONENT:
        case GL_DEPTH_STENCIL: 
        case GL_RED:
        case GL_GREEN:
        case GL_BLUE:
            numChannels_ = 1;
            break;
        case GL_RGB:
        case GL_BGR:
            numChannels_ = 3;
            break;
        case GL_RGBA:
        case GL_BGRA:
            numChannels_ = 4;
            break;
        default:
            numChannels_ = 0;
            LogError("Invalid format: " << format_);
    }
}

void Texture2D::setSizeInBytes(){
    GLuint dataTypeSize;
    switch (dataType_) {
        case GL_UNSIGNED_BYTE:
        case GL_BYTE:
            dataTypeSize = 1;
            break;
        case GL_UNSIGNED_SHORT:
        case GL_SHORT:
        case GL_HALF_FLOAT:
            dataTypeSize = 2;
            break;
        case GL_UNSIGNED_INT:
        case GL_INT:
        case GL_FLOAT:
            dataTypeSize = 4;
            break;
        default:
            dataTypeSize = 0;
            LogError("Invalid data type: " << dataTypeSize);
    }
    byteSize_ = numChannels_*dataTypeSize;
}





} // namespace
