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
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#ifndef IVW_BUFFERRAMPRECISION_H
#define IVW_BUFFERRAMPRECISION_H

#include <inviwo/core/datastructures/buffer/bufferram.h>

namespace inviwo {

template<typename T>
class BufferRAMPrecision : public BufferRAM {
public:
    BufferRAMPrecision(size_t size = 0, const DataFormatBase* format = DataFormatBase::get(), BufferType type = POSITION_ATTRIB, BufferUsage usage = STATIC);
    BufferRAMPrecision(T* data, size_t size, const DataFormatBase* format = DataFormatBase::get(), BufferType type = POSITION_ATTRIB, BufferUsage usage = STATIC);
    BufferRAMPrecision(const BufferRAMPrecision<T>& rhs);
    BufferRAMPrecision<T>& operator=(const BufferRAMPrecision<T>& rhs) {
        if (this != &rhs) {
            
            size_ = rhs.getSize();

            initialize();
            memcpy(data_, rhs.getData(), size_*sizeof(T));
        }
        return *this;
    };
    virtual ~BufferRAMPrecision() {};
    virtual void initialize();
    virtual void initialize(void*);
    virtual void deinitialize();
    virtual BufferRAMPrecision* clone() const;

    virtual void* getData() { return (data_->empty() ? NULL : &data_->front()); } 
    virtual const void* getData() const {return (data_->empty() ? NULL : &data_->front()); } 

    const std::vector<T>* getDataContainer() const { return data_; } 

    void setValueFromSingleFloat(size_t index, float val);
    void setValueFromVec2Float(size_t index, vec2 val);
    void setValueFromVec3Float(size_t index, vec3 val);
    void setValueFromVec4Float(size_t index, vec4 val);

    float getValueAsSingleFloat(size_t index) const;
    vec2 getValueAsVec2Float(size_t index) const;
    vec3 getValueAsVec3Float(size_t index) const;
    vec4 getValueAsVec4Float(size_t index) const;

    void add(const T& item);

    void clear();
    
private:
    static const DataFormatBase* defaultformat(){
        return GenericDataFormat(T)::get();
    }
    std::vector<T>* data_;
};



template<typename T, size_t B>
class BufferRAMCustomPrecision : public BufferRAMPrecision<T> {
public:
    BufferRAMCustomPrecision(size_t size = 0, const DataFormatBase* format = DataFormatBase::get(), BufferType type = POSITION_ATTRIB, BufferUsage usage = STATIC)
        : BufferRAMPrecision<T>(size, format, type, usage) {}
    virtual ~BufferRAMCustomPrecision() {};
    
private:
    static const DataFormatBase* defaultformat(){
        return  DataFormat<T, B>::get();
    }
};

template<typename T>
BufferRAMPrecision<T>::BufferRAMPrecision(size_t size, const DataFormatBase* format, BufferType type, BufferUsage usage) : 
    BufferRAM(size, format, type, usage) {
    initialize();
}
template<typename T>
BufferRAMPrecision<T>::BufferRAMPrecision(T* data, size_t size, const DataFormatBase* format, BufferType type, BufferUsage usage) : 
    BufferRAM(size, format, type, usage) {
    initialize(data);
}

template<typename T>
BufferRAMPrecision<T>::BufferRAMPrecision(const BufferRAMPrecision<T>& rhs) : BufferRAM(rhs) {
    initialize(rhs.data_->data());
}

template<typename T>
void BufferRAMPrecision<T>::initialize() {
    initialize(NULL);
    //data_->resize(size_);
    //data_ = new T[size_];
}

template<typename T>
void BufferRAMPrecision<T>::initialize(void* data) {
    if (data == NULL) {
        data_ = new std::vector<T>(size_);
        //data_ = new T[size_];
    } else {
        data_ = new std::vector<T>(static_cast<T*>(data), static_cast<T*>(data)+size_);
        //data_ = data;
    }
}

template<typename T>
void inviwo::BufferRAMPrecision<T>::deinitialize()
{
    if(data_) {
        delete data_;
        //delete[] static_cast<T*>(data_);
        data_ = NULL;
    }
}

template<typename T>
BufferRAMPrecision<T>* BufferRAMPrecision<T>::clone() const {
    BufferRAMPrecision<T>* newBufferRAM = new BufferRAMPrecision<T>(getSize(), getDataFormat(), getBufferType(), getBufferUsage());
    return newBufferRAM;
}

template<typename T>
void BufferRAMPrecision<T>::setValueFromSingleFloat(size_t index, float val){
    T* data = static_cast<T*>(&data_->front());
    getDataFormat()->floatToValue(val, &(data[index]));
}

template<typename T>
void BufferRAMPrecision<T>::setValueFromVec2Float(size_t index, vec2 val){
    T* data = static_cast<T*>(&data_->front());
    getDataFormat()->vec2ToValue(val, &(data[index]));
}

template<typename T>
void BufferRAMPrecision<T>::setValueFromVec3Float(size_t index, vec3 val){
    T* data = static_cast<T*>(&data_->front());
    getDataFormat()->vec3ToValue(val, &(data[index]));
}

template<typename T>
void BufferRAMPrecision<T>::setValueFromVec4Float(size_t index, vec4 val){
    T* data = static_cast<T*>(&data_->front());
    getDataFormat()->vec4ToValue(val, &(data[index]));
}

template<typename T>
float BufferRAMPrecision<T>::getValueAsSingleFloat(size_t index) const{
    float result;
    T* data = static_cast<T*>(&data_->front());
    T val = data[index];
    result = getDataFormat()->valueToNormalizedFloat(&val);
    return result;
}

template<typename T>
vec2 BufferRAMPrecision<T>::getValueAsVec2Float(size_t index) const{
    vec2 result;
    T* data = static_cast<T*>(&data_->front());
    T val = data[index];
    result = getDataFormat()->valueToNormalizedVec2Float(&val);
    return result;
}

template<typename T>
vec3 BufferRAMPrecision<T>::getValueAsVec3Float(size_t index) const{
    vec3 result;
    T* data = static_cast<T*>(&data_->front());
    T val = data[index];
    result = getDataFormat()->valueToNormalizedVec3Float(&val);
    return result;
}

template<typename T>
vec4 BufferRAMPrecision<T>::getValueAsVec4Float(size_t index) const{
    vec4 result;
    T* data = static_cast<T*>(&data_->front());
    T val = data[index];
    result = getDataFormat()->valueToNormalizedVec4Float(&val);
    return result;
}

template<typename T>
void BufferRAMPrecision<T>::add( const T& item )
{
    data_->push_back(item);
    size_ = data_->size();
    //setSize(data_->size());
}

template<typename T>
void BufferRAMPrecision<T>::clear()
{
    data_->clear();
    size_ = 0;
}

#define DataFormatIdMacro(i) typedef BufferRAMCustomPrecision<Data##i::type, Data##i::bits> BufferRAM_##i;
#include <inviwo/core/util/formatsdefinefunc.h>

typedef BufferRAM_Vec4FLOAT32 ColorBufferRAM;
typedef BufferRAM_FLOAT32 CurvatureBufferRAM;
typedef BufferRAM_UINT32 IndexBufferRAM;
typedef BufferRAM_Vec2FLOAT32 Position2dBufferRAM;
typedef BufferRAM_Vec2FLOAT32 TexCoord2dBufferRAM;
typedef BufferRAM_Vec3FLOAT32 Position3dBufferRAM;
typedef BufferRAM_Vec3FLOAT32 TexCoord3dBufferRAM;
typedef BufferRAM_Vec3FLOAT32 NormalBufferRAM;

} // namespace

#endif // IVW_BUFFERRAMPRECISION_H
