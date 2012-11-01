#ifndef IVW_TEXTURE2D_H
#define IVW_TEXTURE2D_H

#include "inviwo/core/inviwo.h"
#include "../inviwoopengl.h"
#include "../ext/tgaload/tgaload.h"


namespace inviwo {

    class Texture2D {

    public:
        Texture2D(ivec2 dimensions, GLint format, GLint internalformat, GLenum dataType, GLenum filtering);
        virtual ~Texture2D();

        unsigned int getID() const { return id_; }

        // TODO: remove this function
        void loadTexture(std::string fileName) {
            bind();
            image_t temp_image;
            tgaLoad((char*)(fileName.c_str()), &temp_image, TGA_FREE | TGA_LOW_QUALITY);
        }

        void setTexels(GLubyte* texels) { texels_ = texels; }

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
