#ifndef IVW_TEXTURE3D_H
#define IVW_TEXTURE3D_H

#include "inviwo/core/inviwo.h"
#include "../inviwoopengl.h"
#include <io.h>
#include <fstream>

namespace inviwo {

    class Texture3D {

    public:
        Texture3D(ivec3 dimensions, GLint format, GLint internalformat, GLenum dataType, GLenum filtering);
        virtual ~Texture3D();

        unsigned int getID() const { return id_; }

        // TODO: remove this function
        void loadTexture(std::string fileName, ivec3 dimensions) {
            bind();

            texels_ = new GLubyte[dimensions.x*dimensions.y*dimensions.z];

            std::fstream fin(fileName.c_str(), std::ios::in | std::ios::binary);
            if (!fin.good()) {
                std::cout << "Can't open volume file" << fileName << std::endl;
                exit(-1);
            }
            fin.read((char*)texels_, dimensions.x*dimensions.y*dimensions.z);
            fin.close();
            dimensions_ = dimensions;
            upload();
        }

        void setTexels(GLubyte* texels) { texels_ = texels; }

        void bind();
        void upload();

    private:
        ivec3 dimensions_;
        GLenum format_;
        GLenum internalformat_;
        GLenum dataType_;
        GLenum filtering_;

        GLuint id_;
        GLubyte* texels_;
    };

} // namespace

#endif // IVW_TEXTURE3D_H
