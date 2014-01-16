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

#include "texture.h"

namespace inviwo {

Texture::Texture(GLenum target, GLFormats::GLFormat glFormat, GLenum filtering)
    : target_(target)
    , format_(glFormat.format)
    , internalformat_(glFormat.internalFormat)
    , dataType_(glFormat.type)
    , filtering_(filtering)
    , dataInReadBackPBO_(false)
{
    glGenTextures(1, &id_);
    numChannels_ = glFormat.channels;
    byteSize_ = numChannels_*glFormat.typeSize;
    glGenBuffers(1, &pboBack_);
    LGL_ERROR_SUPPRESS;
}

Texture::Texture(GLenum target, GLint format, GLint internalformat, GLenum dataType, GLenum filtering)
    : target_(target)
    , format_(format)
    , internalformat_(internalformat)
    , dataType_(dataType)
    , filtering_(filtering)
    , dataInReadBackPBO_(false)
{
    glGenTextures(1, &id_);
    setNChannels();
    setSizeInBytes();
    glGenBuffers(1, &pboBack_);
    LGL_ERROR_SUPPRESS;
}

Texture::Texture(const Texture& other)
    : target_(other.target_)
    , format_(other.format_)
    , internalformat_(other.internalformat_)
    , dataType_(other.dataType_)
    , filtering_(other.filtering_)
    , byteSize_(other.byteSize_)
    , numChannels_(other.numChannels_)
    , dataInReadBackPBO_(false)
{        
    glGenTextures(1, &id_);
    glGenBuffers(1, &pboBack_);
    LGL_ERROR_SUPPRESS;
}

Texture& Texture::operator=(const Texture& rhs) {
    if (this != &rhs) {
        target_ = rhs.target_;
        format_ = rhs.format_;
        internalformat_ = rhs.internalformat_;
        dataType_ = rhs.dataType_;
        filtering_ = rhs.filtering_;
        numChannels_ = rhs.numChannels_;
        byteSize_ = rhs.byteSize_;
    }
    return *this;
}

Texture::~Texture() {
    glDeleteTextures(1, &id_);
    glDeleteBuffers(1, &pboBack_);
    LGL_ERROR;
}

GLuint Texture::getID() const { 
    return id_; 
}

GLuint Texture::getNChannels() const { 
    return numChannels_; 
}

GLuint Texture::getSizeInBytes() const { 
    return byteSize_; 
}

GLenum Texture::getFormat() const { 
    return format_; 
}

GLenum Texture::getDataType() const { 
    return dataType_; 
}

void Texture::bind() const{
    glBindTexture(target_, id_);
    LGL_ERROR;
}

void Texture::unbind() const{
    glBindTexture(target_, 0);
    LGL_ERROR;
}

void Texture::bindFromPBO() const{
    if(!dataInReadBackPBO_){
        downloadToPBO();
    }
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, pboBack_);
    LGL_ERROR;
}

void Texture::bindToPBO() const{
    glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, pboBack_);
    LGL_ERROR;
}

void Texture::unbindFromPBO() const{
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
    LGL_ERROR;
}

void Texture::unbindToPBO() const{
    glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0);
    LGL_ERROR;
}

void Texture::download(void* data) const {
    if(dataInReadBackPBO_){
        // Copy from PBO
        bindToPBO();
        void* mem = glMapBuffer(GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY);   
        assert(mem);
        memcpy(data, mem, getNumberOfValues()*getSizeInBytes());

        //Release PBO data
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER_ARB);
        unbindToPBO();
        dataInReadBackPBO_ = false;
    }
    else{
        bind();
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glGetTexImage(target_, 0, format_, dataType_, data);
        unbind();
    }
    LGL_ERROR;
}

void Texture::downloadToPBO() const{
    if(!dataInReadBackPBO_){
        bind();
        bindToPBO();
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glGetTexImage(target_, 0, format_, dataType_, 0);
        unbindToPBO();
        dataInReadBackPBO_ = true;
    }
}

void Texture::loadFromPBO(const Texture* src){
    setupAsyncReadBackPBO();
    src->bindFromPBO();
    upload(NULL);
    unbind();
    src->unbindFromPBO();
    LGL_ERROR;
}

void Texture::invalidatePBO(){
    dataInReadBackPBO_ = false;
}

void Texture::setupAsyncReadBackPBO(){
    bind();
    glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, pboBack_);
    glBufferDataARB(GL_PIXEL_PACK_BUFFER_ARB, getNumberOfValues()*getSizeInBytes(), NULL, GL_STREAM_READ_ARB);
    glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, 0);
    unbind();
    dataInReadBackPBO_ = false;
    LGL_ERROR;
}

void Texture::setNChannels(){
    switch (format_) {
        case GL_STENCIL_INDEX:
        case GL_DEPTH_COMPONENT:
        case GL_DEPTH_STENCIL: 
        case GL_RED:
        case GL_GREEN:
        case GL_BLUE:
        case GL_ALPHA:
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

void Texture::setSizeInBytes(){
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
