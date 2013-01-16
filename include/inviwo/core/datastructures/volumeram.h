#ifndef IVW_VOLUMERAM_H
#define IVW_VOLUMERAM_H

#include "inviwo/core/inviwocoredefine.h"
#include "inviwo/core/inviwo.h"
#include "inviwo/core/datastructures/volumerepresentation.h"

namespace inviwo {

class IVW_CORE_API VolumeRAM : public VolumeRepresentation {

public:
    VolumeRAM();
    VolumeRAM(ivec3 dimensions);
    virtual ~VolumeRAM();
    virtual void initialize();
    virtual void deinitialize();
    virtual DataRepresentation* clone()=0;
    virtual void* getData();
protected:
    void* data_;
};

template<typename T>
class IVW_CORE_API VolumeRAMPrecision : public VolumeRAM {
public:
    VolumeRAMPrecision();
    VolumeRAMPrecision(ivec3 dimensions);
    VolumeRAMPrecision(T* data, ivec3 dimensions);
    virtual ~VolumeRAMPrecision() {};
    virtual void initialize(void*);
    virtual void deinitialize();    
    virtual DataRepresentation* clone();
private:
    void setTypeAndFormat();
};

template<typename T>
IVW_CORE_API VolumeRAMPrecision<T>::VolumeRAMPrecision() : VolumeRAM() {
    VolumeRAMPrecision<T>::setTypeAndFormat();
    VolumeRAMPrecision<T>::initialize(0);
}

template<typename T>
IVW_CORE_API VolumeRAMPrecision<T>::VolumeRAMPrecision(ivec3 dimensions) : VolumeRAM(dimensions) {
    VolumeRAMPrecision<T>::setTypeAndFormat();
    VolumeRAMPrecision<T>::initialize(0);
}

template<typename T>
IVW_CORE_API VolumeRAMPrecision<T>::VolumeRAMPrecision(T* data, ivec3 dimensions) : VolumeRAM(dimensions) { 
    VolumeRAMPrecision<T>::setTypeAndFormat();
    VolumeRAMPrecision<T>::initialize(data);
}

template<typename T>
IVW_CORE_API void VolumeRAMPrecision<T>::initialize(void* data) {
    if (!data)
        data_ = new T[dimensions_.x*dimensions_.y*dimensions_.z*sizeof(T)];
    else
        data_ = data;
    VolumeRAM::initialize();
}


template<typename T>
IVW_CORE_API void VolumeRAMPrecision<T>::setTypeAndFormat() {
    if (dynamic_cast< VolumeRAMPrecision<uint8_t>* >(this)) {
        dataFormat_ = "UINT8";
        return;
    }
    else if (dynamic_cast< VolumeRAMPrecision<int8_t>* >(this)) {
        dataFormat_ = "INT8";
        return;
    }
    else if (dynamic_cast< VolumeRAMPrecision<uint16_t>* >(this)) {
        dataFormat_ = "UINT16";
        return;
    }
    else if (dynamic_cast< VolumeRAMPrecision<int16_t>* >(this)) {
        dataFormat_ = "INT16";            
        return;
    }
    else if (dynamic_cast< VolumeRAMPrecision<uint32_t>* >(this)) {
        dataFormat_ = "UINT32";            
        return;
    }
    else if (dynamic_cast< VolumeRAMPrecision<int32_t>* >(this)) {
        dataFormat_ = "INT32";
        return;
    }

    dataFormat_ = "UINT8";
}

template<typename T>
IVW_CORE_API DataRepresentation* VolumeRAMPrecision<T>::clone() {
    VolumeRAMPrecision* newVolumeRAM = new VolumeRAMPrecision<T>(dimensions_);
    //TODO:: Copy volume textures if necessary
    return newVolumeRAM;
}

template<typename T>
IVW_CORE_API void VolumeRAMPrecision<T>::deinitialize() {
    delete data_;
    data_ = 0;
    VolumeRAM::deinitialize();
}

typedef VolumeRAMPrecision<uint8_t> VolumeRAMuint8;
typedef VolumeRAMPrecision<int8_t> VolumeRAMint8;
typedef VolumeRAMPrecision<uint16_t> VolumeRAMuint16;
typedef VolumeRAMPrecision<int16_t> VolumeRAMint16;
typedef VolumeRAMPrecision<uint32_t> VolumeRAMuint32;
typedef VolumeRAMPrecision<int32_t> VolumeRAMint32;

} // namespace

#endif // IVW_VOLUMERAM_H
