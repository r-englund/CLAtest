#include "texture2d.h"

namespace inviwo {

Texture2D::Texture2D(uvec2 dimensions, GLFormats::GLFormat glFormat, GLenum filtering)
    : dimensions_(dimensions),
      format_(glFormat.format),
      internalformat_(glFormat.internalFormat),
      dataType_(glFormat.type),
      filtering_(filtering) {
    glGenTextures(1, &id_);
    numChannels_ = glFormat.channels;
    byteSize_ = numChannels_*glFormat.typeSize;
    LGL_ERROR;
    glGenBuffers(1, &pboBack_);
    setupAsyncReadBackPBO();
    LGL_ERROR;
}

Texture2D::Texture2D(uvec2 dimensions, GLint format, GLint internalformat, GLenum dataType, GLenum filtering)
    : dimensions_(dimensions),
      format_(format),
      internalformat_(internalformat),
      dataType_(dataType),
      filtering_(filtering) {
    glGenTextures(1, &id_);
    setNChannels();
    setSizeInBytes();
    LGL_ERROR;
    glGenBuffers(1, &pboBack_);
    setupAsyncReadBackPBO();
    LGL_ERROR;
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &id_);
    glDeleteBuffers(1, &pboBack_);
    LGL_ERROR;
}

Texture2D* Texture2D::clone() const {
    Texture2D* cloneTexture = new Texture2D(dimensions_, format_, internalformat_, dataType_, filtering_);
    // TODO: Copy texture content
    // glCopyTexImage2D(
    
    return cloneTexture;
}

void Texture2D::bind() const{
    glBindTexture(GL_TEXTURE_2D, id_);
    LGL_ERROR;
}

void Texture2D::upload(const void* data) {
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, internalformat_, dimensions_.x, dimensions_.y, 0, format_, dataType_, data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    LGL_ERROR;
}

void Texture2D::download(void* data) const {
    if(dataInReadBackPBO_){
        // Copy from PBO
        glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, pboBack_);
        void* mem = glMapBuffer(GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY);   
        assert(mem);
        memcpy(data, mem, dimensions_.x*dimensions_.y*getSizeInBytes());

        //Release PBO data
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER_ARB);
        glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0);
        dataInReadBackPBO_ = false;
    }
    else{
        bind();
        glGetTexImage(GL_TEXTURE_2D, 0, format_, dataType_, data);
    }
    LGL_ERROR;
}

void Texture2D::downloadToPBO() const{
    bind();
    glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, pboBack_);
    glGetTexImage(GL_TEXTURE_2D, 0, format_, dataType_, 0);
    glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0);
    dataInReadBackPBO_ = true;
}

void Texture2D::unbind() const{
    glBindTexture(GL_TEXTURE_2D, 0);
    LGL_ERROR;
}

void Texture2D::resize(uvec2 dimension) {
    setWidth(dimension.x);
    setHeight(dimension.y);
    upload(NULL);
    setupAsyncReadBackPBO();
}

void Texture2D::setupAsyncReadBackPBO(){
    glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, pboBack_);
    glBufferDataARB(GL_PIXEL_PACK_BUFFER_ARB, dimensions_.x*dimensions_.y*getSizeInBytes(), NULL, GL_STREAM_READ_ARB);
    glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, 0);
    dataInReadBackPBO_ = false;
}

void Texture2D::setNChannels(){
    switch (format_) {
        case GL_STENCIL_INDEX:
        case GL_DEPTH_COMPONENT:
        case GL_DEPTH_STENCIL: 
        case GL_RED:
        case GL_GREEN:
        case GL_BLUE:
            numChannels_ = 1;
            break;
        case GL_RGB:
        case GL_BGR:
            numChannels_ = 3;
            break;
        case GL_RGBA:
        case GL_BGRA:
            numChannels_ = 4;
            break;
        default:
            numChannels_ = 0;
            LogError("Invalid format: " << format_);
    }
}

void Texture2D::setSizeInBytes(){
    GLuint dataTypeSize;
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
            LogError("Invalid data type: " << dataTypeSize);
    }
    byteSize_ = numChannels_*dataTypeSize;
}

} // namespace
