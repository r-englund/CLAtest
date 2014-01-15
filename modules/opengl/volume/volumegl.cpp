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

#include "volumegl.h"

namespace inviwo {

VolumeGL::VolumeGL(uvec3 dimensions, const DataFormatBase* format)
    : VolumeRepresentation(dimensions, format)
    , volumeTexture_(0){

    GLFormats::GLFormat glFormat = getGLFormats()->getGLFormat(format->getId());
    format_ = glFormat.format;
    internalFormat_ = glFormat.internalFormat;
    dataType_ = glFormat.type;
    initialize();
}

VolumeGL::VolumeGL(uvec3 dimensions, GLint format, GLint internalFormat, GLenum dataType)
    : VolumeRepresentation(dimensions, DataFormatBase::get())
    , format_(format)
    , internalFormat_(internalFormat)
    , dataType_(dataType)
    , volumeTexture_(0){

    initialize();
}

VolumeGL::VolumeGL(const void* texels, uvec3 dimensions, const DataFormatBase* format)
    : VolumeRepresentation(dimensions, format)
    , volumeTexture_(0) {

    GLFormats::GLFormat glFormat = getGLFormats()->getGLFormat(format->getId());
    format_ = glFormat.format;
    internalFormat_ = glFormat.internalFormat;
    dataType_ = glFormat.type;
    initialize(texels);
}

VolumeGL::VolumeGL(const void* texels, uvec3 dimensions, GLint format, GLint internalFormat, GLenum dataType)
    : VolumeRepresentation(dimensions, DataFormatBase::get())
    , format_(format)
    , internalFormat_(internalFormat)
    , dataType_(dataType)
    , volumeTexture_(0) {

    initialize(texels);
}

VolumeGL::VolumeGL(const VolumeGL& rhs) 
    : VolumeRepresentation(rhs){

    //TODO: copy texels.
}

VolumeGL& VolumeGL::operator=(const VolumeGL& that) {
    if(this != &that) {
        VolumeRepresentation::operator=(that);
        //TODO: copy texels.
    }
    return *this;
}

VolumeGL* VolumeGL::clone() const {
    return new VolumeGL(*this);
}

VolumeGL::~VolumeGL() {
    deinitialize();
}

void VolumeGL::initialize() {}

void VolumeGL::initialize(const void* texels) {
    volumeTexture_ = new Texture3D(dimensions_, getFormat(), getInternalFormat(), getDataType(), GL_LINEAR);

    if (texels) {
         upload(texels);
    }

    VolumeGL::initialize();
}

void VolumeGL::deinitialize() {
    delete volumeTexture_;
    volumeTexture_ = 0;
}

GLint VolumeGL::getFormat() {
    return format_; 
}

GLint VolumeGL::getInternalFormat() {
    return internalFormat_; 
}

GLenum VolumeGL::getDataType() {
    return dataType_; 
}

void VolumeGL::bindTexture(GLenum texUnit) const{
    glActiveTexture(texUnit);
    volumeTexture_->bind();
}

void VolumeGL::unbindTexture() const{
    volumeTexture_->unbind();
}

void VolumeGL::upload( const void* data )
{
    volumeTexture_->upload(data);
}

void VolumeGL::download( void* data ) const
{
    volumeTexture_->download(data);

}

} // namespace
