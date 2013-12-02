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

Texture3D::Texture3D(uvec3 dimensions, GLint format, GLint internalformat, GLenum dataType, GLenum filtering)
    : dimensions_(dimensions),
      format_(format),
      internalformat_(internalformat),
      dataType_(dataType),
      filtering_(filtering) {
    glGenTextures(1, &id_);
    // Allocate data
    bind();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage3D(GL_TEXTURE_3D, 0, internalformat_, dimensions_.x, dimensions_.y, dimensions_.z, 0, format_, dataType_, NULL);
    LGL_ERROR;
}

Texture3D::~Texture3D() {
    glDeleteTextures(1, &id_);
    LGL_ERROR;
}

void Texture3D::bind() const{
    glBindTexture(GL_TEXTURE_3D, id_);
    LGL_ERROR;
}

void Texture3D::unbind() const{
    glBindTexture(GL_TEXTURE_3D, 0);
    LGL_ERROR;
}

void Texture3D::upload(const void* data) {
    bind();
    glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, dimensions_.x, dimensions_.y, dimensions_.z, format_, dataType_, data);  
    LGL_ERROR;
}

void Texture3D::download( void* data ) const{
    bind();
    glGetTexImage(GL_TEXTURE_3D, 0, format_, dataType_, data);
}

} // namespace
