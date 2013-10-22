#ifndef IVW_BUFFERRAMPRECISION_H
#define IVW_BUFFERRAMPRECISION_H

#include <inviwo/core/datastructures/buffer/bufferram.h>

namespace inviwo {

template<typename T>
class IVW_CORE_API BufferRAMPrecision : public BufferRAM {
public:
    BufferRAMPrecision(size_t size = 16, const DataFormatBase* format = defaultformat());
    BufferRAMPrecision(T* data, size_t size, const DataFormatBase* format = defaultformat());
    BufferRAMPrecision(const BufferRAMPrecision<T>& rhs): BufferRAM(rhs.getSize(), rhs.getDataFormat()) {
        *this = rhs;
    }
    BufferRAMPrecision<T>& operator=(const BufferRAMPrecision<T>& rhs) {
        if (this != &rhs) {
            delete[] data_;
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
    virtual DataRepresentation* clone() const;

    void setValueFromSingleFloat(size_t index, float val);
    void setValueFromVec2Float(size_t index, vec2 val);
    void setValueFromVec3Float(size_t index, vec3 val);
    void setValueFromVec4Float(size_t index, vec4 val);

    float getValueAsSingleFloat(size_t index) const;
    vec2 getValueAsVec2Float(size_t index) const;
    vec3 getValueAsVec3Float(size_t index) const;
    vec4 getValueAsVec4Float(size_t index) const;
    
private:
    static const DataFormatBase* defaultformat(){
        return GenericDataFormat(T)::get();
    }
};

template<typename T, size_t B>
class IVW_CORE_API BufferRAMCustomPrecision : public BufferRAMPrecision<T> {
public:
    BufferRAMCustomPrecision(size_t size = 16, const DataFormatBase* format = defaultformat())
        : BufferRAMPrecision<T>(size, format) {}
    virtual ~BufferRAMCustomPrecision() {};
    
private:
    static const DataFormatBase* defaultformat(){
        return  DataFormat<T, B>::get();
    }
};

template<typename T>
BufferRAMPrecision<T>::BufferRAMPrecision(size_t size, const DataFormatBase* format) : BufferRAM(size, format) {
    initialize();
}
template<typename T>
BufferRAMPrecision<T>::BufferRAMPrecision(T* data, size_t size, const DataFormatBase* format) : BufferRAM(size, format) {
    initialize(data);
}

template<typename T>
void BufferRAMPrecision<T>::initialize() {
    data_ = new T[size_];
}

template<typename T>
void BufferRAMPrecision<T>::initialize(void* data) {
    if (data == NULL) {
        data_ = new T[size_];
    } else {
        data_ = data;
    }
}

template<typename T>
void inviwo::BufferRAMPrecision<T>::deinitialize()
{
    if(data_) {
        delete[] static_cast<T*>(data_);
        data_ = NULL;
    }
}

template<typename T>
DataRepresentation* BufferRAMPrecision<T>::clone() const {
    BufferRAMPrecision* newBufferRAM = new BufferRAMPrecision<T>(getSize(), getDataFormat());
    return newBufferRAM;
}

template<typename T>
void BufferRAMPrecision<T>::setValueFromSingleFloat(size_t index, float val){
    T* data = static_cast<T*>(data_);
    getDataFormat()->floatToValue(val, &(data[index]));
}

template<typename T>
void BufferRAMPrecision<T>::setValueFromVec2Float(size_t index, vec2 val){
    T* data = static_cast<T*>(data_);
    getDataFormat()->vec2ToValue(val, &(data[index]));
}

template<typename T>
void BufferRAMPrecision<T>::setValueFromVec3Float(size_t index, vec3 val){
    T* data = static_cast<T*>(data_);
    getDataFormat()->vec3ToValue(val, &(data[index]));
}

template<typename T>
void BufferRAMPrecision<T>::setValueFromVec4Float(size_t index, vec4 val){
    T* data = static_cast<T*>(data_);
    getDataFormat()->vec4ToValue(val, &(data[index]));
}

template<typename T>
float BufferRAMPrecision<T>::getValueAsSingleFloat(size_t index) const{
    float result;
    T* data = static_cast<T*>(data_);
    T val = data[index];
    result = getDataFormat()->valueToNormalizedFloat(&val);
    return result;
}

template<typename T>
vec2 BufferRAMPrecision<T>::getValueAsVec2Float(size_t index) const{
    vec2 result;
    T* data = static_cast<T*>(data_);
    T val = data[index];
    result = getDataFormat()->valueToNormalizedVec2Float(&val);
    return result;
}

template<typename T>
vec3 BufferRAMPrecision<T>::getValueAsVec3Float(size_t index) const{
    vec3 result;
    T* data = static_cast<T*>(data_);
    T val = data[index];
    result = getDataFormat()->valueToNormalizedVec3Float(&val);
    return result;
}

template<typename T>
vec4 BufferRAMPrecision<T>::getValueAsVec4Float(size_t index) const{
    vec4 result;
    T* data = static_cast<T*>(data_);
    T val = data[index];
    result = getDataFormat()->valueToNormalizedVec4Float(&val);
    return result;
}

#define DataFormatIdMacro(i) typedef BufferRAMCustomPrecision<Data##i::type, Data##i::bits> BufferRAM_##i;
#include <inviwo/core/util/formatsdefinefunc.h>

} // namespace

#endif // IVW_BUFFERRAMPRECISION_H
