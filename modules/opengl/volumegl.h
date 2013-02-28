#ifndef IVW_VOLUMEGL_H
#define IVW_VOLUMEGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/inviwo.h>
#include <inviwo/core/datastructures/volumerepresentation.h>
#include "glwrap/texture3d.h"

namespace inviwo {

class IVW_MODULE_OPENGL_API VolumeGL : public VolumeRepresentation {

public:
    VolumeGL(uvec3 dimensions = uvec3(128,128,128), DataFormatBase format = DataFormatBase());
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
    VolumeGLPrecision(uvec3 dimensions = uvec3(128,128,128), DataFormatBase format = GenericDataFormat(T)());
    VolumeGLPrecision(T* texels, uvec3 dimensions = uvec3(128,128,128), DataFormatBase format = GenericDataFormat(T)());
    virtual ~VolumeGLPrecision() {};
    using VolumeGL::initialize;
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

template<typename T, size_t B>
class IVW_MODULE_OPENGL_API VolumeGLCustomPrecision : public VolumeGLPrecision<T> {
public:
    VolumeGLCustomPrecision(uvec3 dimensions = uvec3(128,128,128), DataFormatBase format = DataFormat<T, B>()) : VolumeGLPrecision<T>(dimensions, format) {};
    VolumeGLCustomPrecision(T* texels, uvec3 dimensions = uvec3(128,128,128), DataFormatBase format = DataFormat<T, B>()) : VolumeGLPrecision<T>(texels, dimensions, format) {};
};

template<typename T>
VolumeGLPrecision<T>::VolumeGLPrecision(uvec3 dimensions, DataFormatBase format) : VolumeGL(dimensions, format) {
    VolumeGLPrecision<T>::setTypeAndFormat();
    VolumeGLPrecision<T>::initialize(0);
}

template<typename T>
VolumeGLPrecision<T>::VolumeGLPrecision(T* texels, uvec3 dimensions, DataFormatBase format) : VolumeGL(dimensions, format) {
    VolumeGLPrecision<T>::setTypeAndFormat();
    VolumeGLPrecision<T>::initialize(texels);
}

template<typename T>
void VolumeGLPrecision<T>::setTypeAndFormat() {
    if (typeid(this) == typeid(VolumeGLPrecision<DataUINT8::type>*)) {
        dataType_ = GL_UNSIGNED_BYTE;
        format_ = GL_ALPHA;
        internalFormat_ = GL_ALPHA8;
        return;
    }
    else if (typeid(this) == typeid(VolumeGLPrecision<DataINT8::type>*)) {
        dataType_ = GL_BYTE;
        format_ = GL_ALPHA;
        internalFormat_ = GL_ALPHA8;
        return;
    }
    else if (typeid(this) == typeid(VolumeGLPrecision<DataUINT16::type>*)) {
        dataType_ = GL_UNSIGNED_SHORT;
        format_ = GL_ALPHA;

        if(dataFormatBase_.getBitsStored() == DataUINT12::bits){
            internalFormat_ = GL_ALPHA12;
        }
        else{
            internalFormat_ = GL_ALPHA16;
        }
        return;
    }
    else if (typeid(this) == typeid(VolumeGLPrecision<DataINT16::type>*)) {
        dataType_ = GL_SHORT;
        format_ = GL_ALPHA;
        if(dataFormatBase_.getBitsStored() == DataINT12::bits){
            internalFormat_ = GL_ALPHA12;
        }
        else{
            internalFormat_ = GL_ALPHA16;
        }
        return;
    }
    else if (typeid(this) == typeid(VolumeGLPrecision<DataUINT32::type>*)) {
        dataType_ = GL_UNSIGNED_INT;
        format_ = GL_ALPHA;
        internalFormat_ = GL_ALPHA;
        return;
    }
    else if (typeid(this) == typeid(VolumeGLPrecision<DataINT32::type>*)) {
        dataType_ = GL_INT;
        format_ = GL_ALPHA;
        internalFormat_ = GL_ALPHA;
        return;
    }

    format_ = GL_ALPHA;
    dataType_ = GL_UNSIGNED_BYTE;
    internalFormat_ = GL_ALPHA8;
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

typedef VolumeGLPrecision<DataUINT8::type>     VolumeGLuint8;
typedef VolumeGLPrecision<DataINT8::type>      VolumeGLint8;
typedef VolumeGLPrecision<DataUINT16::type>    VolumeGLuint16;
typedef VolumeGLPrecision<DataINT16::type>     VolumeGLint16;
typedef VolumeGLPrecision<DataUINT32::type>    VolumeGLuint32;
typedef VolumeGLPrecision<DataINT32::type>     VolumeGLint32;
typedef VolumeGLPrecision<DataFLOAT16::type>   VolumeGLfloat16;
typedef VolumeGLPrecision<DataFLOAT32::type>   VolumeGLfloat32;
typedef VolumeGLPrecision<DataFLOAT64::type>   VolumeGLfloat64;

typedef VolumeGLCustomPrecision<DataUINT12::type, DataUINT12::bits> VolumeGLuint12;
typedef VolumeGLCustomPrecision<DataINT12::type, DataINT12::bits> VolumeGLint12;

} // namespace

#endif // IVW_VOLUMEGL_H
