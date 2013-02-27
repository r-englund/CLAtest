#ifndef IVW_TEXTURE2D_H
#define IVW_TEXTURE2D_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/ext/tgaload/tgaload.h>


namespace inviwo {

class IVW_MODULE_OPENGL_API Texture2D {

public:
    Texture2D(uvec2 dimensions, GLint format, GLint internalformat, GLenum dataType, GLenum filtering);
    virtual ~Texture2D();

    unsigned int getID() const { return id_; }

    void setTexels(GLubyte* texels) { texels_ = texels; }
    GLubyte* getTexels() { return texels_; }

    void bind();
    void upload();
    void unbind();

    int getWidth() { return dimensions_.x; }
    int getHeight() { return dimensions_.y; }
    void setWidth(int x) { dimensions_.x = x; }
    void setHeight(int y) { dimensions_.y = y; }

private:
    uvec2 dimensions_;
    GLenum format_;
    GLenum internalformat_;
    GLenum dataType_;
    GLenum filtering_;

    GLuint id_;
    GLubyte* texels_;
};

} // namespace

#endif // IVW_TEXTURE2D_H
