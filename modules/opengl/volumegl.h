#ifndef IVW_VOLUMEGL_H
#define IVW_VOLUMEGL_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/datastructures/volumerepresentation.h"
#include "glwrap/texture3d.h"

namespace inviwo {

class VolumeGL : public VolumeRepresentation {

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

template<class T=UINT8>
class VolumeGLPrecision : public VolumeGL {
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

template<class T>
VolumeGLPrecision<T>::VolumeGLPrecision() : VolumeGL() {
    VolumeGLPrecision<T>::setTypeAndFormat();
    VolumeGLPrecision<T>::initialize(0);
}

template<class T>
VolumeGLPrecision<T>::VolumeGLPrecision(ivec3 dimensions) : VolumeGL(dimensions) {
    VolumeGLPrecision<T>::setTypeAndFormat();
    VolumeGLPrecision<T>::initialize(0);
}

template<class T>
VolumeGLPrecision<T>::VolumeGLPrecision(T* texels, ivec3 dimensions) : VolumeGL(dimensions) {
    VolumeGLPrecision<T>::setTypeAndFormat();
    VolumeGLPrecision<T>::initialize(texels);
}

template<class T>
void VolumeGLPrecision<T>::setTypeAndFormat() {
    if (dynamic_cast< VolumeGLPrecision<UINT8>* >(this)) {
        dataFormat_ = "UINT8";
        dataType_ = GL_UNSIGNED_BYTE;
        format_ = GL_ALPHA;
        internalFormat_ = GL_ALPHA8;
        return;
    }
    else if (dynamic_cast< VolumeGLPrecision<INT8>* >(this)) {
        dataFormat_ = "UINT8";
        dataType_ = GL_BYTE;
        format_ = GL_ALPHA;
        internalFormat_ = GL_ALPHA8;
        return;
    }
    else if (dynamic_cast< VolumeGLPrecision<UINT16>* >(this)) {
        dataFormat_ = "UINT16";
        dataType_ = GL_UNSIGNED_SHORT;
        format_ = GL_ALPHA;
        internalFormat_ = GL_ALPHA16;
        return;
    }
    else if (dynamic_cast< VolumeGLPrecision<INT16>* >(this)) {
        dataFormat_ = "INT16";
        dataType_ = GL_SHORT;
        format_ = GL_ALPHA;
        internalFormat_ = GL_ALPHA16;
        return;
    }
    else if (dynamic_cast< VolumeGLPrecision<UINT32>* >(this)) {
        dataFormat_ = "UINT32";
        dataType_ = GL_UNSIGNED_INT;
        format_ = GL_ALPHA;
        internalFormat_ = GL_ALPHA;
        return;
    }
    else if (dynamic_cast< VolumeGLPrecision<INT32>* >(this)) {
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


template<class T>
GLenum VolumeGLPrecision<T>::getDataType() {
    return dataType_; 
}

template<class T>
GLint VolumeGLPrecision<T>::getFormat() {
    return format_; 
}

template<class T>
GLint VolumeGLPrecision<T>::getInternalFormat() {
    return internalFormat_; 
}

template<class T>
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

template<class T>
DataRepresentation* VolumeGLPrecision<T>::clone() {
    VolumeGLPrecision* newVolumeGL = new VolumeGLPrecision<T>(dimensions_);
    //TODO:: Copy volume textures if necessary
    return newVolumeGL;
}

template<class T>
void VolumeGLPrecision<T>::deinitialize() {
    delete volumeTexture_;
    volumeTexture_ = 0;
    VolumeGL::deinitialize();
}

typedef VolumeGLPrecision<UINT8> VolumeGLuint8;
typedef VolumeGLPrecision<INT8> VolumeGLint8;
typedef VolumeGLPrecision<UINT16> VolumeGLuint16;
typedef VolumeGLPrecision<INT16> VolumeGLint16;
typedef VolumeGLPrecision<UINT32> VolumeGLuint32;
typedef VolumeGLPrecision<INT32> VolumeGLint32;

} // namespace

#endif // IVW_VOLUMEGL_H
