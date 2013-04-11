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

    void Texture2D::download(void* data) {
        bind();
        // Using FBO
        glReadBuffer((GLenum)GL_COLOR_ATTACHMENT0_EXT);
        glReadPixels(0, 0, dimensions_.x, dimensions_.y, format_, dataType_, (GLvoid*)data);
        LGL_ERROR;
        //// Using PBO ( could be performed asynchronos
        //glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, id_);
        //// Yet to be implemented
        //GLuint dataSize = getSizeInBytes();
        //glBufferData(GL_PIXEL_PACK_BUFFER_ARB, dataSize, NULL, GL_STREAM_READ);
        //glReadPixels (0, 0, dimensions_.x, dimensions_.y, format_, dataType_, 
        //    static_cast<char*>(NULL));
        //void* mem = glMapBuffer(GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY);   
        //assert(mem);
        //std::copy(mem, mem+dataSize, data);
        //glUnmapBuffer(GL_PIXEL_PACK_BUFFER_ARB);
        //glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0); 
    }

    void Texture2D::unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
        LGL_ERROR;
    }

    size_t Texture2D::getSizeInBytes() const
    {
        size_t channels = static_cast<size_t>(getNChannels());

        size_t dataTypeSize;
        switch (dataType_) {
            case GL_UNSIGNED_BYTE:
            case GL_BYTE:
                dataTypeSize = 1;
                break;
            case GL_UNSIGNED_SHORT:
            case GL_SHORT:
            case GL_HALF_FLOAT:
                dataTypeSize = 2;
                break;
            case GL_UNSIGNED_INT:
            case GL_INT:
            case GL_FLOAT:
                dataTypeSize = 4;
                break;
            default:
                dataTypeSize = 0;
                LogErrorS("Texture2D::getSizeInBytes()", "Invalid data type: " << dataTypeSize);
        }
        return channels*dataTypeSize;
    }

    unsigned int Texture2D::getNChannels() const
    {
        unsigned int channels;
        switch (format_) {
            case GL_STENCIL_INDEX:
            case GL_DEPTH_COMPONENT:
            case GL_DEPTH_STENCIL: 
            case GL_RED:
            case GL_GREEN:
            case GL_BLUE:
                channels = 1;
                break;
            case GL_RGB:
            case GL_BGR:
                channels = 3;
                break;
            case GL_RGBA:
            case GL_BGRA:
                channels = 4;
                break;
            default:
                channels = 0;
                LogErrorS("Texture2D::getSizeInBytes()", "Invalid format: " << format_);
        }
        return channels;
    }

} // namespace
