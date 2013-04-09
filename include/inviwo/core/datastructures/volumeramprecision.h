#ifndef IVW_VOLUMERAMPRECISION_H
#define IVW_VOLUMERAMPRECISION_H

#include <inviwo/core/datastructures/volumeram.h>
#include <inviwo/core/datastructures/volumeramoperationexecuter.h>

namespace inviwo {

template<typename T>
class IVW_CORE_API VolumeRAMPrecision : public VolumeRAM {
public:
    VolumeRAMPrecision(uvec3 dimensions = uvec3(128,128,128), VolumeRepresentation::VolumeBorders border = VolumeRepresentation::VolumeBorders(), DataFormatBase format = GenericDataFormat(T)());
    VolumeRAMPrecision(T* data, uvec3 dimensions = uvec3(128,128,128), VolumeRepresentation::VolumeBorders border = VolumeRepresentation::VolumeBorders(), DataFormatBase format = GenericDataFormat(T)());
    virtual ~VolumeRAMPrecision() {};
    virtual void applyOperation(DataOperation* dop);
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
    void applyOperation(DataOperation*);
};

/*template class VolumeRAMPrecision<DataUINT8::type>;
template class VolumeRAMPrecision<DataINT8::type>;
template class VolumeRAMPrecision<DataUINT16::type>;
template class VolumeRAMPrecision<DataINT16::type>;
template class VolumeRAMPrecision<DataUINT32::type>;
template class VolumeRAMPrecision<DataINT32::type>;
template class VolumeRAMPrecision<DataFLOAT16::type>;
template class VolumeRAMPrecision<DataFLOAT32::type>;
template class VolumeRAMPrecision<DataFLOAT64::type>;

template class VolumeRAMCustomPrecision<DataUINT12::type, DataUINT12::bits>;
template class VolumeRAMCustomPrecision<DataINT12::type, DataINT12::bits>;*/

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

template<typename T>
void VolumeRAMPrecision<T>::applyOperation(DataOperation* dop){ 
    executeOperationOnVolumeRAMPrecision<T, GenericDataBits(T)>(dop);
}

template<typename T, size_t B>
void VolumeRAMCustomPrecision<T,B>::applyOperation(DataOperation* dop){ 
    executeOperationOnVolumeRAMPrecision<T, B>(dop); 
}

} // namespace

#endif // IVW_VOLUMERAMPRECISION_H
