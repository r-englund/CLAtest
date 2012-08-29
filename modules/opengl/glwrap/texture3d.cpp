#include "texture3d.h"

namespace inviwo {

const std::string Texture3D::logSource_ = "Texture3D";

Texture3D::Texture3D(ivec3 dimensions, GLint format, GLint internalformat, GLenum dataType, GLenum filtering)
    : dimensions_(dimensions),
      format_(format),
      internalformat_(internalformat),
      dataType_(dataType),
      filtering_(filtering) {
    glGenTextures(1, &id_);
    texels_ = 0;
}

Texture3D::~Texture3D() {}

void Texture3D::bind() {
    glBindTexture(GL_TEXTURE_3D, id_);
    LGL_ERROR;
}

void Texture3D::upload() {
    bind();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_ALPHA, dimensions_.x, dimensions_.y, dimensions_.z, 0, GL_ALPHA, GL_UNSIGNED_BYTE, texels_);
    LGL_ERROR;
}

} // namespace
