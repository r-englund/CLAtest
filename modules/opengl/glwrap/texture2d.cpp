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
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &id_);
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

void Texture2D::download(void* data) {
    bind();
    glGetTexImage(GL_TEXTURE_2D, 0, format_, dataType_, data);
    // Using FBO
    //glReadBuffer((GLenum)GL_COLOR_ATTACHMENT0_EXT);
    //glReadPixels(0, 0, dimensions_.x, dimensions_.y, format_, dataType_, (GLvoid*)data);
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

void Texture2D::unbind() const{
    glBindTexture(GL_TEXTURE_2D, 0);
    LGL_ERROR;
}

void Texture2D::resize(uvec2 dimension) {
    //Warning: Resize texture will corrupt existing texel
    //TODO: Requires efficient up/down sampling
    GLubyte* newTexel = new GLubyte[dimension.x*dimension.y*getSizeInBytes()];
    size_t pixelOffsetInBytes = getSizeInBytes();
    for (size_t i=0; i<dimension.x; i++) {
            for (size_t j=0; j<dimension.y; j++) {                        
                //check pattern
                if ( ( ((i+1)/8)%2 == 0 && ((j+1)/8)%2 == 0) || ( ((i+1)/8)%2 == 1 && ((j+1)/8)%2 == 1) ) {                        
                    for (size_t k=0; k<pixelOffsetInBytes; k++)
                        newTexel[(j*dimension.x+i)*pixelOffsetInBytes+k] = 128;                            
                }
                else {
                    for (size_t k=0; k<pixelOffsetInBytes; k++)
                        newTexel[(j*dimension.x+i)*pixelOffsetInBytes+k] = 255; 
                }
                                           
            }
    }
    setWidth(dimension.x);
    setHeight(dimension.y);
    upload(newTexel);
    delete[] newTexel;

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
