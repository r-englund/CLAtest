#ifndef IVW_VOLUMERAM_H
#define IVW_VOLUMERAM_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>
#include <inviwo/core/datastructures/volumerepresentation.h>
#include <inviwo/core/datastructures/volumeramoperationexecuter.h>

namespace inviwo {

struct VolumeRepresentation::VolumeBorders;

class IVW_CORE_API VolumeRAM : public VolumeRepresentation {

public:
    VolumeRAM(uvec3 dimension = uvec3(128,128,128), VolumeRepresentation::VolumeBorders border = VolumeRepresentation::VolumeBorders(), DataFormatBase format = DataFormatBase());
    virtual ~VolumeRAM();
    virtual void applyOperation(DataOperation*) = 0;
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
    VolumeRAMPrecision(uvec3 dimensions = uvec3(128,128,128), VolumeRepresentation::VolumeBorders border = VolumeRepresentation::VolumeBorders(), DataFormatBase format = GenericDataFormat(T)());
    VolumeRAMPrecision(T* data, uvec3 dimensions = uvec3(128,128,128), VolumeRepresentation::VolumeBorders border = VolumeRepresentation::VolumeBorders(), DataFormatBase format = GenericDataFormat(T)());
    virtual ~VolumeRAMPrecision() {};
    virtual void applyOperation(DataOperation* dop){ executeOperationOnVolumeRAMPrecision<T, GenericDataBits(T)>(dop); }
    using VolumeRAM::initialize;
    virtual void initialize(void*);
    virtual void deinitialize();
    virtual DataRepresentation* clone();
};

template<typename T, size_t B>
class IVW_CORE_API VolumeRAMCustomPrecision : public VolumeRAMPrecision<T> {
public:
    VolumeRAMCustomPrecision(uvec3 dimensions = uvec3(128,128,128), VolumeRepresentation::VolumeBorders border = VolumeRepresentation::VolumeBorders(), DataFormatBase format = DataFormat<T, B>()) : VolumeRAMPrecision<T>(dimensions, border, format) {};
    VolumeRAMCustomPrecision(T* data, uvec3 dimensions = uvec3(128,128,128), VolumeRepresentation::VolumeBorders border = VolumeRepresentation::VolumeBorders(), DataFormatBase format = DataFormat<T, B>()) : VolumeRAMPrecision<T>(data, dimensions, border, format) {};
    virtual ~VolumeRAMCustomPrecision() {};
    virtual void applyOperation(DataOperation* dop){ executeOperationOnVolumeRAMPrecision<T, B>(dop); }
};

template<typename T>
VolumeRAMPrecision<T>::VolumeRAMPrecision(uvec3 dimensions, VolumeRepresentation::VolumeBorders border, DataFormatBase format) : VolumeRAM(dimensions, border, format) {
    initialize(0);
}

template<typename T>
VolumeRAMPrecision<T>::VolumeRAMPrecision(T* data, uvec3 dimensions, VolumeRepresentation::VolumeBorders border, DataFormatBase format) : VolumeRAM(dimensions, border, format) { 
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
