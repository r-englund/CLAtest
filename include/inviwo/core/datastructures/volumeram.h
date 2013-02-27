#ifndef IVW_VOLUMERAM_H
#define IVW_VOLUMERAM_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>
#include <inviwo/core/datastructures/volumerepresentation.h>

namespace inviwo {

class IVW_CORE_API VolumeRAM : public VolumeRepresentation {

public:
    VolumeRAM(uvec3 dimension = uvec3(128,128,128), const VolumeBorders& border = VolumeBorders(), DataFormatBase format = DataFormatBase());
    virtual ~VolumeRAM();
    virtual void initialize();
    virtual void deinitialize();
    virtual DataRepresentation* clone() = 0;
    virtual void* getData();
    virtual const void* getData() const;
protected:
    void* data_;
};

template<typename T>
class IVW_CORE_API VolumeRAMPrecision : public VolumeRAM {
public:
    VolumeRAMPrecision(uvec3 dimensions = uvec3(128,128,128), const VolumeBorders& border = VolumeBorders(), DataFormatBase format = GenericDataFormat(T)());
    VolumeRAMPrecision(T* data, uvec3 dimensions = uvec3(128,128,128), const VolumeBorders& border = VolumeBorders(), DataFormatBase format = GenericDataFormat(T)());
    virtual ~VolumeRAMPrecision() {};
    using VolumeRAM::initialize;
    virtual void initialize(void*);
    virtual void deinitialize();
    virtual DataRepresentation* clone();
    virtual void setSubVolume(const VolumeRAM* vol, const uvec3& offset = uvec3(0,0,0));
    virtual VolumeRAMPrecision<T>* getSubVolume(const uvec3& dimensions, const uvec3& offset = uvec3(0,0,0), const VolumeRepresentation::VolumeBorders& border = VolumeRepresentation::VolumeBorders()) const throw (std::bad_alloc);
};

template<typename T, size_t B>
class IVW_CORE_API VolumeRAMCustomPrecision : public VolumeRAMPrecision<T> {
public:
    VolumeRAMCustomPrecision(uvec3 dimensions = uvec3(128,128,128), const VolumeBorders& border = VolumeBorders(), DataFormatBase format = DataFormat<T, B>()) : VolumeRAMPrecision(dimensions, border, format) {};
    VolumeRAMCustomPrecision(T* data, uvec3 dimensions = uvec3(128,128,128), const VolumeBorders& border = VolumeBorders(), DataFormatBase format = DataFormat<T, B>()) : VolumeRAMPrecision(data, dimensions, border, format) {};
};

template<typename T>
VolumeRAMPrecision<T>::VolumeRAMPrecision(uvec3 dimensions, const VolumeBorders& border, DataFormatBase format) : VolumeRAM(dimensions, border, format) {
    initialize(0);
}

template<typename T>
VolumeRAMPrecision<T>::VolumeRAMPrecision(T* data, uvec3 dimensions, const VolumeBorders& border, DataFormatBase format) : VolumeRAM(dimensions, border, format) { 
    initialize(data);
}

template<typename T>
void VolumeRAMPrecision<T>::initialize(void* data) {
    if (!data)
        data_ = new T[dimensions_.x*dimensions_.y*dimensions_.z*sizeof(T)];
    else
        data_ = data;
    VolumeRAM::initialize();
}

template<typename T>
DataRepresentation* VolumeRAMPrecision<T>::clone() {
    VolumeRAMPrecision* newVolumeRAM = new VolumeRAMPrecision<T>(dimensions_);
    //TODO:: Copy volume textures if necessary
    return newVolumeRAM;
}

template<typename T>
void VolumeRAMPrecision<T>::deinitialize() {
    delete static_cast<T*>(data_);
    data_ = 0;
    VolumeRAM::deinitialize();
}

template<typename T>
VolumeRAMPrecision<T>* VolumeRAMPrecision<T>::getSubVolume(const uvec3& dimensions, const uvec3& offset, const VolumeRepresentation::VolumeBorders& border) const
throw (std::bad_alloc){
    // create new volume
    VolumeRAMPrecision<T>* newVolume = new VolumeRAMPrecision<T>(dimensions, border);
    newVolume->originalDimensions_ = getDimensions();
    T* data = reinterpret_cast<T*>(newVolume->getData());

    // determine parameters
    uvec3 dataDims = getDimensions();
    uvec3 offsetDims = offset-newVolume->getBorderLLF();
    offsetDims = glm::max(offsetDims, uvec3(0,0,0));
    size_t initialStartPos = (offsetDims.z * (dataDims.x*dataDims.y))+(offsetDims.y * dataDims.x) + offsetDims.x;
    uvec3 internalDimension = getDimensionsWithBorder();

    // per row
    size_t dataSize = internalDimension.x*static_cast<size_t>(getDataFormat().getBytesAllocated());

    // memcpy each row for every slice to form sub volume
    size_t volumePos;
    size_t subVolumePos;
    for (size_t i=0; i < internalDimension.z; i++) {
        for (size_t j=0; j < internalDimension.y; j++) {
            volumePos = (j*dataDims.x) + (i*dataDims.x*dataDims.y);
            subVolumePos = (j*internalDimension.x) + (i*internalDimension.x*internalDimension.y);
            //memcpy(data + subVolumePos, (data_ + volumePos + initialStartPos), dataSize);
        }
    }

    return newVolume;
}

template<typename T>
void VolumeRAMPrecision<T>::setSubVolume(const VolumeRAM* vol, const uvec3& offset)
{
    const T* data = reinterpret_cast<const T*>(vol->getData());

    // determine parameters
    uvec3 dataDims = getDimensions();
    size_t initialStartPos = (offset.z * (dataDims.x*dataDims.y))+(offset.y * dataDims.x) + offset.x;

    // per row
    uvec3 dimensions = vol->getDimensions();
    size_t dataSize = dimensions.x*static_cast<size_t>(getDataFormat().getBytesAllocated());

    // internal dimension including volume of the subvolume
    uvec3 internalDimension = vol->getDimensionsWithBorder();

    // memcpy each row for every slice in sub volume to form this volume
    size_t volumePos;
    size_t subVolumePos;
    for (size_t i=0; i < dimensions.z; i++) {
        for (size_t j=0; j < dimensions.y; j++) {
            volumePos =  (j*dataDims.x) + (i*dataDims.x*dataDims.y);
            subVolumePos = ((j+vol->getBorderLLF().y)*internalDimension.x) + ((i+vol->getBorderLLF().z)*internalDimension.x*internalDimension.y) + vol->getBorderLLF().x;
            //memcpy((data_ + volumePos + initialStartPos), (data + subVolumePos), dataSize);
        }
    }
}

typedef VolumeRAMPrecision<DataUINT8::type>     VolumeRAMuint8;
typedef VolumeRAMPrecision<DataINT8::type>      VolumeRAMint8;
typedef VolumeRAMPrecision<DataUINT16::type>    VolumeRAMuint16;
typedef VolumeRAMPrecision<DataINT16::type>     VolumeRAMint16;
typedef VolumeRAMPrecision<DataUINT32::type>    VolumeRAMuint32;
typedef VolumeRAMPrecision<DataINT32::type>     VolumeRAMint32;
typedef VolumeRAMPrecision<DataFLOAT16::type>   VolumeRAMfloat16;
typedef VolumeRAMPrecision<DataFLOAT32::type>   VolumeRAMfloat32;
typedef VolumeRAMPrecision<DataFLOAT64::type>   VolumeRAMfloat64;

typedef VolumeRAMCustomPrecision<DataUINT12::type, DataUINT12::bits>    VolumeRAMuint12;
typedef VolumeRAMCustomPrecision<DataINT12::type, DataINT12::bits>      VolumeRAMint12;

} // namespace

#endif // IVW_VOLUMERAM_H
