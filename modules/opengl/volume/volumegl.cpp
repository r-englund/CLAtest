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

VolumeGL::VolumeGL(uvec3 dimensions, const DataFormatBase* format, Texture3D* tex)
    : VolumeRepresentation(dimensions, format)
    , volumeTexture_(tex)
{
    initialize();
}

VolumeGL::VolumeGL(const VolumeGL& rhs) 
    : VolumeRepresentation(rhs){
    volumeTexture_ = rhs.volumeTexture_->clone();
}

VolumeGL& VolumeGL::operator=(const VolumeGL& rhs) {
    if(this != &rhs) {
        VolumeRepresentation::operator=(rhs);
        volumeTexture_ = rhs.volumeTexture_->clone();
    }
    return *this;
}

VolumeGL::~VolumeGL() {
    deinitialize();
}

VolumeGL* VolumeGL::clone() const {
    return new VolumeGL(*this);
}

void VolumeGL::initialize() {
    if(!volumeTexture_){
        GLFormats::GLFormat glFormat = getGLFormats()->getGLFormat(getDataFormatId());
        volumeTexture_ = new Texture3D(dimensions_, glFormat, GL_LINEAR);
    }
}

void VolumeGL::deinitialize() {
    delete volumeTexture_;
    volumeTexture_ = NULL;
}

void VolumeGL::bindTexture(GLenum texUnit) const{
    glActiveTexture(texUnit);
    volumeTexture_->bind();
}

void VolumeGL::unbindTexture() const{
    volumeTexture_->unbind();
}

Texture3D* VolumeGL::getTexture() { 
    return volumeTexture_; 
}

const Texture3D* VolumeGL::getTexture() const { 
    return volumeTexture_; 
}

} // namespace
