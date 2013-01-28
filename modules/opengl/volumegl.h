#ifndef IVW_VOLUMEGL_H
#define IVW_VOLUMEGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/inviwo.h>
#include <inviwo/core/datastructures/volumerepresentation.h>
#include "glwrap/texture3d.h"

namespace inviwo {

class IVW_MODULE_OPENGL_API VolumeGL : public VolumeRepresentation {

public:
    VolumeGL();
    VolumeGL(ivec3 dimensions);
    virtual ~VolumeGL();

    virtual void initialize();
    virtual void deinitialize();
    virtual DataRepresentation* clone()=0;
    virtual GLenum getDataType()=0;
    virtual GLint getFormat()=0;
    virtual GLint getInternalFormat()=0;
    virtual void bindTexture(GLenum texUnit);
    virtual void unbindTexture();

protected:
    Texture3D* volumeTexture_;
};

template<typename T>
class IVW_MODULE_OPENGL_API VolumeGLPrecision : public VolumeGL {
public:
    VolumeGLPrecision();
    VolumeGLPrecision(ivec3 dimensions);
    VolumeGLPrecision(T* texels, ivec3 dimensions);
    virtual ~VolumeGLPrecision() {};
    virtual void initialize(void* texels);
    virtual void deinitialize();
    virtual DataRepresentation* clone();
    virtual GLenum getDataType();
    virtual GLint getFormat();
    virtual GLint getInternalFormat();
private :
    void setTypeAndFormat();
    GLenum dataType_;
    GLint format_;
    GLint internalFormat_;
};

template<typename T>
VolumeGLPrecision<T>::VolumeGLPrecision() : VolumeGL() {
    VolumeGLPrecision<T>::setTypeAndFormat();
    VolumeGLPrecision<T>::initialize(0);
}

template<typename T>
VolumeGLPrecision<T>::VolumeGLPrecision(ivec3 dimensions) : VolumeGL(dimensions) {
    VolumeGLPrecision<T>::setTypeAndFormat();
    VolumeGLPrecision<T>::initialize(0);
}

template<typename T>
VolumeGLPrecision<T>::VolumeGLPrecision(T* texels, ivec3 dimensions) : VolumeGL(dimensions) {
    VolumeGLPrecision<T>::setTypeAndFormat();
    VolumeGLPrecision<T>::initialize(texels);
}

template<typename T>
void VolumeGLPrecision<T>::setTypeAndFormat() {
    if (dynamic_cast< VolumeGLPrecision<uint8_t>* >(this)) {
        dataFormat_ = "UINT8";
        dataType_ = GL_UNSIGNED_BYTE;
        format_ = GL_ALPHA;
        internalFormat_ = GL_ALPHA8;
        return;
    }
    else if (dynamic_cast< VolumeGLPrecision<int8_t>* >(this)) {
        dataFormat_ = "UINT8";
        dataType_ = GL_BYTE;
        format_ = GL_ALPHA;
        internalFormat_ = GL_ALPHA8;
        return;
    }
    else if (dynamic_cast< VolumeGLPrecision<uint16_t>* >(this)) {
        dataFormat_ = "UINT16";
        dataType_ = GL_UNSIGNED_SHORT;
        format_ = GL_ALPHA;
        internalFormat_ = GL_ALPHA16;
        return;
    }
    else if (dynamic_cast< VolumeGLPrecision<int16_t>* >(this)) {
        dataFormat_ = "INT16";
        dataType_ = GL_SHORT;
        format_ = GL_ALPHA;
        internalFormat_ = GL_ALPHA16;
        return;
    }
    else if (dynamic_cast< VolumeGLPrecision<uint32_t>* >(this)) {
        dataFormat_ = "UINT32";
        dataType_ = GL_UNSIGNED_INT;
        format_ = GL_ALPHA;
        internalFormat_ = GL_ALPHA;
        return;
    }
    else if (dynamic_cast< VolumeGLPrecision<int32_t>* >(this)) {
        dataFormat_ = "INT32";
        dataType_ = GL_INT;
        format_ = GL_ALPHA;
        internalFormat_ = GL_ALPHA;
        return;
    }

    dataFormat_ = "UINT8";
    format_ = GL_ALPHA;
    internalFormat_ = GL_ALPHA8;
    dataType_ = GL_UNSIGNED_BYTE;
}


template<typename T>
GLenum VolumeGLPrecision<T>::getDataType() {
    return dataType_; 
}

template<typename T>
GLint VolumeGLPrecision<T>::getFormat() {
    return format_; 
}

template<typename T>
GLint VolumeGLPrecision<T>::getInternalFormat() {
    return internalFormat_; 
}

template<typename T>
void VolumeGLPrecision<T>::initialize(void* texels) {
    volumeTexture_ = new Texture3D(dimensions_, getFormat(), getInternalFormat(), getDataType(), GL_LINEAR);

    if (!texels) {
        //volumeTexture_->loadTexture(IVW_DIR+"data/volumes/nucleon.raw", dimensions_=ivec3(41,41,41));
        volumeTexture_->loadTexture(IVW_DIR+"data/volumes/hydrogenatom.raw", dimensions_=ivec3(128,128,128));
        //volumeTexture_->loadTexture(IVW_DIR+"data/volumes/neghip.raw", dimensions_=ivec3(64,64,64));
        //volumeTexture_->loadTexture(IVW_DIR+"data/volumes/marschnerlobb.raw", dimensions_=ivec3(41,41,41));
        //volumeTexture_->loadTexture(IVW_DIR+"data/volumes/aneurysm.raw", dimensions_=ivec3(256,256,256));
    }
    else {
        volumeTexture_->bind();
        volumeTexture_->setTexels(texels);
        volumeTexture_->upload();
    }

    VolumeGL::initialize();
}

template<typename T>
DataRepresentation* VolumeGLPrecision<T>::clone() {
    VolumeGLPrecision* newVolumeGL = new VolumeGLPrecision<T>(dimensions_);
    //TODO:: Copy volume textures if necessary
    return newVolumeGL;
}

template<typename T>
void VolumeGLPrecision<T>::deinitialize() {
    delete volumeTexture_;
    volumeTexture_ = 0;
    VolumeGL::deinitialize();
}

typedef VolumeGLPrecision<uint8_t> VolumeGLuint8;
typedef VolumeGLPrecision<int8_t> VolumeGLint8;
typedef VolumeGLPrecision<uint16_t> VolumeGLuint16;
typedef VolumeGLPrecision<int16_t> VolumeGLint16;
typedef VolumeGLPrecision<uint32_t> VolumeGLuint32;
typedef VolumeGLPrecision<int32_t> VolumeGLint32;

} // namespace

#endif // IVW_VOLUMEGL_H
