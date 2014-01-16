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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include "texture3d.h"

namespace inviwo {

Texture3D::Texture3D(uvec3 dimensions, GLFormats::GLFormat glFormat, GLenum filtering)
    : Texture(GL_TEXTURE_3D, glFormat, filtering)
    , dimensions_(dimensions) {}

Texture3D::Texture3D(uvec3 dimensions, GLint format, GLint internalformat, GLenum dataType, GLenum filtering)
    : Texture(GL_TEXTURE_3D, format, internalformat, dataType, filtering)
    , dimensions_(dimensions) {}

Texture3D::Texture3D(const Texture3D& rhs)
    : Texture(rhs)
    , dimensions_(rhs.dimensions_) 
{
    initialize(NULL);   
    // TODO: Copy texture from other
    // bind();
    // glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, dimensions_.x, dimensions_.y);
}

Texture3D& Texture3D::operator=(const Texture3D& rhs) {
    if (this != &rhs) {
        Texture::operator=(rhs);
        dimensions_ = rhs.dimensions_;
        initialize(NULL);
        // TODO: Copy other texture content
    }
    return *this;
}

Texture3D::~Texture3D() {}

Texture3D* Texture3D::clone() const {
    return new Texture3D(*this);
}

void Texture3D::initialize(const void* data) {
    // Allocate data
    bind();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filtering_);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, filtering_);
    glTexImage3D(GL_TEXTURE_3D, 0, internalformat_, dimensions_.x, dimensions_.y, dimensions_.z, 0, format_, dataType_, data);
    LGL_ERROR;
}

size_t Texture3D::getNumberOfValues() const{
    return static_cast<size_t>(dimensions_.x*dimensions_.y);
}

void Texture3D::upload(const void* data) {
    bind();
    glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, dimensions_.x, dimensions_.y, dimensions_.z, format_, dataType_, data);  
    LGL_ERROR;
}

} // namespace
