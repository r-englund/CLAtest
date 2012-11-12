#ifndef IVW_TEXTURE2D_H
#define IVW_TEXTURE2D_H

#include "inviwo/core/inviwo.h"
#include "modules/opengl/inviwoopengl.h"
#include "modules/opengl/ext/tgaload/tgaload.h"


namespace inviwo {

    class Texture2D {

    public:
        Texture2D(ivec2 dimensions, GLint format, GLint internalformat, GLenum dataType, GLenum filtering);
        virtual ~Texture2D();

        unsigned int getID() const { return id_; }

        // TODO: remove this function
        void loadTexture(std::string fileName, ivec2 &dimensions=ivec2(0,0)) {
            bind();
            image_t temp_image;
            tgaLoad((char*)(fileName.c_str()), &temp_image, TGA_FREE | TGA_LOW_QUALITY);
            dimensions = ivec2(temp_image.info.width, temp_image.info.height );
        }

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
        ivec2 dimensions_;
        GLenum format_;
        GLenum internalformat_;
        GLenum dataType_;
        GLenum filtering_;

        GLuint id_;
        GLubyte* texels_;
    };

} // namespace

#endif // IVW_TEXTURE2D_H
