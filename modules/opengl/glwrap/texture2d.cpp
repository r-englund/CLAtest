#include "texture2d.h"

namespace inviwo {

    Texture2D::Texture2D(uvec2 dimensions, GLint format, GLint internalformat, GLenum dataType, GLenum filtering)
        : dimensions_(dimensions),
          format_(format),
          internalformat_(internalformat),
          dataType_(dataType),
          filtering_(filtering) {
        glGenTextures(1, &id_);
        texels_ = 0;
    }

    Texture2D::~Texture2D() {
        glDeleteTextures(1, &id_);
    }

    void Texture2D::bind() {
        glBindTexture(GL_TEXTURE_2D, id_);
        LGL_ERROR;
    }

    void Texture2D::upload() {
        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, internalformat_, dimensions_.x, dimensions_.y, 0, format_, dataType_, texels_);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        LGL_ERROR;
    }

    void Texture2D::unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
        LGL_ERROR;
    }
} // namespace
