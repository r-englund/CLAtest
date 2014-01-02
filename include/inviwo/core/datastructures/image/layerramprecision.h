/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_LAYERRAMPRECISION_H
#define IVW_LAYERRAMPRECISION_H

#include <inviwo/core/datastructures/image/layerram.h>

namespace inviwo {

template<typename T>
class LayerRAMPrecision : public LayerRAM {
public:
    LayerRAMPrecision(uvec2 dimensions = uvec2(128,128), const DataFormatBase* format = defaultformat());
    LayerRAMPrecision(T* data, uvec2 dimensions = uvec2(128,128), const DataFormatBase* format = defaultformat());
    LayerRAMPrecision(const LayerRAMPrecision<T>& rhs): LayerRAM(rhs.getDimensions(), rhs.getDataFormat()) {
        *this = rhs;
    }
    LayerRAMPrecision<T>& operator=(const LayerRAMPrecision<T>& rhs) {
        if (this != &rhs) {
            delete[] data_;
            dimensions_ = rhs.getDimensions();
            initialize();
            memcpy(data_, rhs.getData(), dimensions_.x*dimensions_.y*sizeof(T));
        }
        return *this;
    };
    virtual ~LayerRAMPrecision() {
        deinitialize();
    };
    virtual void initialize();
    virtual void initialize(void*);
    virtual void deinitialize();
    virtual DataRepresentation* clone() const;
    virtual void resize(uvec2 dimensions);

    void setValueFromSingleFloat(const uvec2& pos, float val);
    void setValueFromVec2Float(const uvec2& pos, vec2 val);
    void setValueFromVec3Float(const uvec2& pos, vec3 val);
    void setValueFromVec4Float(const uvec2& pos, vec4 val);

    float getValueAsSingleFloat(const uvec2& pos) const;
    vec2 getValueAsVec2Float(const uvec2& pos) const;
    vec3 getValueAsVec3Float(const uvec2& pos) const;
    vec4 getValueAsVec4Float(const uvec2& pos) const;

    vec4 getPickingValue(const uvec2& pos) const;

protected:
    void allocatePickingData();
    
private:
    static const DataFormatBase* defaultformat(){
        return GenericDataFormat(T)::get();
    }
};

template<typename T, size_t B>
class LayerRAMCustomPrecision : public LayerRAMPrecision<T> {
public:
    LayerRAMCustomPrecision(uvec2 dimensions = uvec2(128,128), const DataFormatBase* format = defaultformat())
        : LayerRAMPrecision<T>(dimensions, format) {}
    LayerRAMCustomPrecision(T* data, uvec2 dimensions = uvec2(128,128), const DataFormatBase* format = defaultformat())
        : LayerRAMPrecision<T>(data, dimensions, format) {}
    virtual ~LayerRAMCustomPrecision() {};
    
private:
    static const DataFormatBase* defaultformat(){
        return  DataFormat<T, B>::get();
    }
};

template<typename T>
LayerRAMPrecision<T>::LayerRAMPrecision(uvec2 dimensions, const DataFormatBase* format) : LayerRAM(dimensions, format) {
    initialize();
}
template<typename T>
LayerRAMPrecision<T>::LayerRAMPrecision(T* data, uvec2 dimensions, const DataFormatBase* format) : LayerRAM(dimensions, format) {
    initialize(data);
}

template<typename T>
void LayerRAMPrecision<T>::initialize() {
    data_ = new T[dimensions_.x*dimensions_.y];
}

template<typename T>
void LayerRAMPrecision<T>::initialize(void* data) {
    if (data == NULL) {
        data_ = new T[dimensions_.x*dimensions_.y];
    } else {
        data_ = data;
    }
}

template<typename T>
void LayerRAMPrecision<T>::deinitialize(){
    if(data_) {
        delete[] static_cast<T*>(data_);
        data_ = NULL;
    }
    if(pickingData_) {
        delete[] static_cast<T*>(pickingData_);
        pickingData_ = NULL;
    }
}

template<typename T>
void LayerRAMPrecision<T>::resize(uvec2 dimensions) {
    dimensions_ = dimensions;

    //Delete and reallocate data_ to new size
    LayerRAM::deinitialize();
    deinitialize();
    LayerRAM::initialize();
    initialize();
}

template<typename T>
DataRepresentation* LayerRAMPrecision<T>::clone() const {
    LayerRAMPrecision* newLayerRAM = new LayerRAMPrecision<T>(getDimensions());
    //*newLayerRAM = *this;
    return newLayerRAM;
}

template<typename T>
void LayerRAMPrecision<T>::setValueFromSingleFloat(const uvec2& pos, float val){
    T* data = static_cast<T*>(data_);
    getDataFormat()->floatToValue(val, &(data[posToIndex(pos, dimensions_)]));
}

template<typename T>
void LayerRAMPrecision<T>::setValueFromVec2Float(const uvec2& pos, vec2 val){
    T* data = static_cast<T*>(data_);
    getDataFormat()->vec2ToValue(val, &(data[posToIndex(pos, dimensions_)]));
}

template<typename T>
void LayerRAMPrecision<T>::setValueFromVec3Float(const uvec2& pos, vec3 val){
    T* data = static_cast<T*>(data_);
    getDataFormat()->vec3ToValue(val, &(data[posToIndex(pos, dimensions_)]));
}

template<typename T>
void LayerRAMPrecision<T>::setValueFromVec4Float(const uvec2& pos, vec4 val){
    T* data = static_cast<T*>(data_);
    getDataFormat()->vec4ToValue(val, &(data[posToIndex(pos, dimensions_)]));
}

template<typename T>
float LayerRAMPrecision<T>::getValueAsSingleFloat(const uvec2& pos) const{
    float result;
    T* data = static_cast<T*>(data_);
    T val = data[posToIndex(pos, dimensions_)];
    result = getDataFormat()->valueToNormalizedFloat(&val);
    return result;
}

template<typename T>
vec2 LayerRAMPrecision<T>::getValueAsVec2Float(const uvec2& pos) const{
    vec2 result;
    T* data = static_cast<T*>(data_);
    T val = data[posToIndex(pos, dimensions_)];
    result = getDataFormat()->valueToNormalizedVec2Float(&val);
    return result;
}

template<typename T>
vec3 LayerRAMPrecision<T>::getValueAsVec3Float(const uvec2& pos) const{
    vec3 result;
    T* data = static_cast<T*>(data_);
    T val = data[posToIndex(pos, dimensions_)];
    result = getDataFormat()->valueToNormalizedVec3Float(&val);
    return result;
}

template<typename T>
vec4 LayerRAMPrecision<T>::getValueAsVec4Float(const uvec2& pos) const{
    vec4 result;
    T* data = static_cast<T*>(data_);
    T val = data[posToIndex(pos, dimensions_)];
    result = getDataFormat()->valueToNormalizedVec4Float(&val);
    return result;
}

template<typename T>
vec4 LayerRAMPrecision<T>::getPickingValue(const uvec2& pos) const{
    vec4 result = vec4(0.f);
    if (pickingData_) {
        T* pickData = static_cast<T*>(pickingData_);
        T val = pickData[posToIndex(pos, dimensions_)];
        result = getDataFormat()->valueToNormalizedVec4Float(&val);
    }
    return result;
}

template<typename T>
void LayerRAMPrecision<T>::allocatePickingData(){
    pickingData_ = new T[dimensions_.x*dimensions_.y];
}

#define DataFormatIdMacro(i) typedef LayerRAMCustomPrecision<Data##i::type, Data##i::bits> LayerRAM_##i;
#include <inviwo/core/util/formatsdefinefunc.h>

} // namespace

#endif // IVW_LAYERRAMPRECISION_H
