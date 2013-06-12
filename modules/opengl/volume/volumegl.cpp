#include "volumegl.h"

namespace inviwo {

VolumeGL::VolumeGL(uvec3 dimensions, DataFormatBase format)
    : VolumeRepresentation(dimensions, format), volumeTexture_(0)
{
    GLFormats::GLFormat glFormat = getGLFormats()->getGLFormat(format.getId());
    format_ = glFormat.format;
    internalFormat_ = glFormat.internalFormat;
    dataType_ = glFormat.type;
    initialize();
}

VolumeGL::VolumeGL(uvec3 dimensions, GLint format, GLint internalFormat, GLenum dataType)
: VolumeRepresentation(dimensions, DataFormatBase()), format_(format), internalFormat_(internalFormat), dataType_(dataType), volumeTexture_(0)
{
    initialize();
}

VolumeGL::VolumeGL(void* texels, uvec3 dimensions, DataFormatBase format)
: VolumeRepresentation(dimensions, format), volumeTexture_(0)
{
    GLFormats::GLFormat glFormat = getGLFormats()->getGLFormat(format.getId());
    format_ = glFormat.format;
    internalFormat_ = glFormat.internalFormat;
    dataType_ = glFormat.type;
    initialize(texels);
}

VolumeGL::VolumeGL(void* texels, uvec3 dimensions, GLint format, GLint internalFormat, GLenum dataType)
: VolumeRepresentation(dimensions, DataFormatBase()), format_(format), internalFormat_(internalFormat), dataType_(dataType), volumeTexture_(0)
{
    initialize(texels);
}

VolumeGL::~VolumeGL() {}

void VolumeGL::initialize() {}

void VolumeGL::initialize(void* texels) {
    volumeTexture_ = new Texture3D(dimensions_, getFormat(), getInternalFormat(), getDataType(), GL_LINEAR);

    if (!texels) {
        //TODO: remove loading here
        //volumeTexture_->loadTexture(IVW_DIR+"data/volumes/nucleon.raw", dimensions_=ivec3(41,41,41));
        volumeTexture_->loadTexture(IVW_DIR+"data/volumes/hydrogenatom.raw", dimensions_=ivec3(128,128,128));
        //volumeTexture_->loadTexture(IVW_DIR+"data/volumes/neghip.raw", dimensions_=ivec3(64,64,64));
        //volumeTexture_->loadTexture(IVW_DIR+"data/volumes/marschnerlobb.raw", dimensions_=ivec3(41,41,41));
        //volumeTexture_->loadTexture(IVW_DIR+"data/volumes/aneurysm.raw", dimensions_=ivec3(256,256,256));
    }
    else {
        volumeTexture_->bind();
        volumeTexture_->upload(texels);
    }

    VolumeGL::initialize();
}

void VolumeGL::deinitialize() {
    delete volumeTexture_;
    volumeTexture_ = 0;
}

DataRepresentation* VolumeGL::clone() const {
    VolumeGL* newVolumeGL = new VolumeGL(dimensions_);
    //TODO:: Copy volume textures if necessary
    return newVolumeGL;
}

GLint VolumeGL::getFormat() {
    return format_; 
}

GLint VolumeGL::getInternalFormat() {
    return internalFormat_; 
}

GLenum VolumeGL::getDataType() {
    return dataType_; 
}

void VolumeGL::bindTexture(GLenum texUnit) const{
    glActiveTexture(texUnit);
    volumeTexture_->bind();
    glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(vec4(0.0)));
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void VolumeGL::unbindTexture() const{
    volumeTexture_->unbind();
}

} // namespace
