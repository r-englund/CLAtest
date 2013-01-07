#ifndef IVW_VOLUMERAM_H
#define IVW_VOLUMERAM_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/datastructures/volumerepresentation.h"

namespace inviwo {

class VolumeRAM : public VolumeRepresentation {

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


template<class T=UINT8>
class VolumeRAMPrecision : public VolumeRAM {
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

template<class T>
VolumeRAMPrecision<T>::VolumeRAMPrecision() : VolumeRAM() {
    VolumeRAMPrecision<T>::setTypeAndFormat();
    VolumeRAMPrecision<T>::initialize(0);
}

template<class T>
VolumeRAMPrecision<T>::VolumeRAMPrecision(ivec3 dimensions) : VolumeRAM(dimensions) {
    VolumeRAMPrecision<T>::setTypeAndFormat();
    VolumeRAMPrecision<T>::initialize(0);
}

template<class T>
VolumeRAMPrecision<T>::VolumeRAMPrecision(T* data, ivec3 dimensions) : VolumeRAM(dimensions) { 
    VolumeRAMPrecision<T>::setTypeAndFormat();
    VolumeRAMPrecision<T>::initialize(data);
}

template<class T>
void VolumeRAMPrecision<T>::initialize(void* data) {
    if (!data)
        data_ = new T[dimensions_.x*dimensions_.y*dimensions_.z*sizeof(T)];
    else
        data_ = data;
    VolumeRAM::initialize();
}


template<class T>
void VolumeRAMPrecision<T>::setTypeAndFormat() {
    if (dynamic_cast< VolumeRAMPrecision<UINT8>* >(this)) {
        dataFormat_ = "UINT8";
        return;
    }
    else if (dynamic_cast< VolumeRAMPrecision<INT8>* >(this)) {
        dataFormat_ = "UINT8";
        return;
    }
    else if (dynamic_cast< VolumeRAMPrecision<UINT16>* >(this)) {
        dataFormat_ = "UINT16";
        return;
    }
    else if (dynamic_cast< VolumeRAMPrecision<INT16>* >(this)) {
        dataFormat_ = "INT16";            
        return;
    }
    else if (dynamic_cast< VolumeRAMPrecision<UINT32>* >(this)) {
        dataFormat_ = "UINT32";            
        return;
    }
    else if (dynamic_cast< VolumeRAMPrecision<INT32>* >(this)) {
        dataFormat_ = "INT32";
        return;
    }

    dataFormat_ = "UINT8";
}

template<class T>
DataRepresentation* VolumeRAMPrecision<T>::clone() {
    VolumeRAMPrecision* newVolumeRAM = new VolumeRAMPrecision<T>(dimensions_);
    //TODO:: Copy volume textures if necessary
    return newVolumeRAM;
}

template<class T>
void VolumeRAMPrecision<T>::deinitialize() {
    delete data_;
    data_ = 0;
    VolumeRAM::deinitialize();
}

typedef VolumeRAMPrecision<UINT8> VolumeRAMuint8;
typedef VolumeRAMPrecision<INT8> VolumeRAMint8;
typedef VolumeRAMPrecision<UINT16> VolumeRAMuint16;
typedef VolumeRAMPrecision<INT16> VolumeRAMint16;
typedef VolumeRAMPrecision<UINT32> VolumeRAMuint32;
typedef VolumeRAMPrecision<INT32> VolumeRAMint32;

} // namespace

#endif // IVW_VOLUMERAM_H
