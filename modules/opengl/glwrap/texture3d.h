#ifndef IVW_TEXTURE3D_H
#define IVW_TEXTURE3D_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include <stdio.h>
#include <fstream>

namespace inviwo {

class IVW_MODULE_OPENGL_API Texture3D {

public:
    Texture3D(uvec3 dimensions, GLint format, GLint internalformat, GLenum dataType, GLenum filtering);
    virtual ~Texture3D();

    unsigned int getID() const { return id_; }

    // TODO: remove this function
    void loadTexture(std::string fileName, uvec3 dimensions) {
        bind();

        texels_ = new GLubyte[dimensions.x*dimensions.y*dimensions.z];

        std::fstream fin(fileName.c_str(), std::ios::in | std::ios::binary);
        ivwAssert(fin.good(), "cannot open volume file");
        fin.read((char*)texels_, dimensions.x*dimensions.y*dimensions.z);
        fin.close();
        dimensions_ = dimensions;
        upload();
    }

    void setTexels(void* texels) { texels_ = texels; }

    void bind();
    void unbind();
    void upload();

private:
    uvec3 dimensions_;
    GLenum format_;
    GLenum internalformat_;
    GLenum dataType_;
    GLenum filtering_;

    GLuint id_;
    void* texels_;

    static const std::string logSource_; ///< Source string to be displayed for log messages.
};

} // namespace

#endif // IVW_TEXTURE3D_H
