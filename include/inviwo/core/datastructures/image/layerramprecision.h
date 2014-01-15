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
    LayerRAMPrecision(uvec2 dimensions = uvec2(256,256), 
                      LayerType type = COLOR_LAYER, 
                      const DataFormatBase* format = defaultformat());

    LayerRAMPrecision(T* data, 
                      uvec2 dimensions = uvec2(256,256),
                      LayerType type = COLOR_LAYER, 
                      const DataFormatBase* format = defaultformat());

    LayerRAMPrecision(const LayerRAMPrecision<T>& rhs)
        : LayerRAM(rhs) {
        initialize();
        memcpy(data_, rhs.getData(), dimensions_.x*dimensions_.y*sizeof(T));
    }
    LayerRAMPrecision<T>& operator=(const LayerRAMPrecision<T>& rhs) {
        if (this != &rhs) {
            LayerRAM::operator=(rhs);
            delete[] data_;
            initialize();
            memcpy(data_, rhs.getData(), dimensions_.x*dimensions_.y*sizeof(T));
        }
        return *this;
    };
    virtual LayerRAMPrecision<T>* clone() const {
        return new LayerRAMPrecision<T>(*this);
    }

    virtual ~LayerRAMPrecision() {
        deinitialize();
    };
    virtual void initialize();
    virtual void initialize(void*);
    virtual void deinitialize();
    virtual void resize(uvec2 dimensions);

    void setValueFromSingleFloat(const uvec2& pos, float val);
    void setValueFromVec2Float(const uvec2& pos, vec2 val);
    void setValueFromVec3Float(const uvec2& pos, vec3 val);
    void setValueFromVec4Float(const uvec2& pos, vec4 val);

    float getValueAsSingleFloat(const uvec2& pos) const;
    vec2 getValueAsVec2Float(const uvec2& pos) const;
    vec3 getValueAsVec3Float(const uvec2& pos) const;
    vec4 getValueAsVec4Float(const uvec2& pos) const;
    
private:
    static const DataFormatBase* defaultformat(){
        return GenericDataFormat(T)::get();
    }
};

template<typename T, size_t B>
class LayerRAMCustomPrecision : public LayerRAMPrecision<T> {
public:
    LayerRAMCustomPrecision(uvec2 dimensions = uvec2(256,256), 
                            LayerType type = COLOR_LAYER, 
                            const DataFormatBase* format = defaultformat())
        : LayerRAMPrecision<T>(dimensions, type, format) {
    }
    LayerRAMCustomPrecision(T* data, uvec2 dimensions = uvec2(256,256),
                            LayerType type = COLOR_LAYER, 
                            const DataFormatBase* format = defaultformat())
        : LayerRAMPrecision<T>(data, dimensions, type, format) {
    }
    virtual ~LayerRAMCustomPrecision() {};
    
private:
    static const DataFormatBase* defaultformat(){
        return  DataFormat<T, B>::get();
    }
};

template<typename T>
LayerRAMPrecision<T>::LayerRAMPrecision(uvec2 dimensions, LayerType type, const DataFormatBase* format) : LayerRAM(dimensions, type, format) {
    initialize();
}
template<typename T>
LayerRAMPrecision<T>::LayerRAMPrecision(T* data, uvec2 dimensions, LayerType type, const DataFormatBase* format) : LayerRAM(dimensions, type, format) {
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
}

template<typename T>
void LayerRAMPrecision<T>::resize(uvec2 dimensions) {
    dimensions_ = dimensions;

    //Delete and reallocate data_ to new size
    deinitialize();
    initialize();
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

#define DataFormatIdMacro(i) typedef LayerRAMCustomPrecision<Data##i::type, Data##i::bits> LayerRAM_##i;
#include <inviwo/core/util/formatsdefinefunc.h>

} // namespace

#endif // IVW_LAYERRAMPRECISION_H
