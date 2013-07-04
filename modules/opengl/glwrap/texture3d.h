#ifndef IVW_TEXTURE3D_H
#define IVW_TEXTURE3D_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
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

        GLubyte* texels = new GLubyte[dimensions.x*dimensions.y*dimensions.z];

        std::fstream fin(fileName.c_str(), std::ios::in | std::ios::binary);
        ivwAssert(fin.good(), "cannot open volume file");
        fin.read((char*)texels, dimensions.x*dimensions.y*dimensions.z);
        fin.close();
        dimensions_ = dimensions;
        upload(texels);
        delete[] texels;
    }


    void bind() const;
    void unbind() const;
    void upload(const void* data);
    void download(void* data) const;

    const uvec3& getDimension() const { return dimensions_; }

private:
    uvec3 dimensions_;
    GLenum format_;
    GLenum internalformat_;
    GLenum dataType_;
    GLenum filtering_;

    GLuint id_;
};

} // namespace

#endif // IVW_TEXTURE3D_H
