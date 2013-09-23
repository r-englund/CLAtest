#ifndef IVW_VOLUMERAMPRECISION_H
#define IVW_VOLUMERAMPRECISION_H

#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeramoperationexecuter.h>

namespace inviwo {

template<typename T>
class IVW_CORE_API VolumeRAMPrecision : public VolumeRAM {
public:
    VolumeRAMPrecision(uvec3 dimensions = uvec3(128,128,128), VolumeRepresentation::VolumeBorders border = VolumeRepresentation::VolumeBorders(), const DataFormatBase* format = GenericDataFormat(T)::get());
    VolumeRAMPrecision(T* data, uvec3 dimensions = uvec3(128,128,128), VolumeRepresentation::VolumeBorders border = VolumeRepresentation::VolumeBorders(), const DataFormatBase* format = GenericDataFormat(T)::get());
    virtual ~VolumeRAMPrecision() {};
    VolumeRAMPrecision(const VolumeRAMPrecision<T>& rhs) {
        *this = rhs;
    }
    VolumeRAMPrecision<T>& operator=(const VolumeRAMPrecision<T>& rhs) {
        if (this != &rhs) {
            delete[] data_;
            dimensions_ = rhs.getDimension();
            initialize();
            std::copy(rhs.getData(), rhs.getData()+dimensions_.x*dimensions_.y*sizeof(T), data_);
        }
        return *this;
    };
    virtual void performOperation(DataOperation* dop) const;
    using VolumeRAM::initialize;
    virtual void initialize(void*);
    virtual void deinitialize();
    virtual DataRepresentation* clone() const;

    void setValueFromSingleFloat(const uvec3& pos, float val);
    void setValueFromVec2Float(const uvec3& pos, vec2 val);
    void setValueFromVec3Float(const uvec3& pos, vec3 val);
    void setValueFromVec4Float(const uvec3& pos, vec4 val);

    float getValueAsSingleFloat(const uvec3& pos) const;
    vec2 getValueAsVec2Float(const uvec3& pos) const;
    vec3 getValueAsVec3Float(const uvec3& pos) const;
    vec4 getValueAsVec4Float(const uvec3& pos) const;
};

template<typename T, size_t B>
class IVW_CORE_API VolumeRAMCustomPrecision : public VolumeRAMPrecision<T> {
public:
    VolumeRAMCustomPrecision(uvec3 dimensions = uvec3(128,128,128), VolumeRepresentation::VolumeBorders border = VolumeRepresentation::VolumeBorders(), const DataFormatBase* format =  DataFormat<T, B>::get()) : VolumeRAMPrecision<T>(dimensions, border, format) {};
    VolumeRAMCustomPrecision(T* data, uvec3 dimensions = uvec3(128,128,128), VolumeRepresentation::VolumeBorders border = VolumeRepresentation::VolumeBorders(), const DataFormatBase* format =  DataFormat<T, B>::get()) : VolumeRAMPrecision<T>(data, dimensions, border, format) {};
    virtual ~VolumeRAMCustomPrecision() {};
    void performOperation(DataOperation*) const;
};

template<typename T>
VolumeRAMPrecision<T>::VolumeRAMPrecision(uvec3 dimensions, VolumeRepresentation::VolumeBorders border, const DataFormatBase* format) : VolumeRAM(dimensions, border, format) {
    initialize(0);
}

template<typename T>
VolumeRAMPrecision<T>::VolumeRAMPrecision(T* data, uvec3 dimensions, VolumeRepresentation::VolumeBorders border, const DataFormatBase* format) : VolumeRAM(dimensions, border, format) { 
    initialize(data);
}

template<typename T>
void VolumeRAMPrecision<T>::initialize(void* data) {
    if (!data)
        data_ = new T[dimensions_.x*dimensions_.y*dimensions_.z];
    else
        data_ = data;
    VolumeRAM::initialize();
}

template<typename T>
void inviwo::VolumeRAMPrecision<T>::deinitialize()
{
    if(data_) {
        delete[] static_cast<T*>(data_);
        data_ = NULL;
    }
}

template<typename T>
DataRepresentation* VolumeRAMPrecision<T>::clone() const {
    VolumeRAMPrecision* newVolumeRAM = new VolumeRAMPrecision<T>(dimensions_);
    //TODO:: Copy volume textures if necessary
    return newVolumeRAM;
}

template<typename T>
void VolumeRAMPrecision<T>::performOperation(DataOperation* dop) const{ 
    executeOperationOnVolumeRAMPrecision<T, GenericDataBits(T)>(dop);
}

template<typename T, size_t B>
void VolumeRAMCustomPrecision<T,B>::performOperation(DataOperation* dop) const{ 
    executeOperationOnVolumeRAMPrecision<T, B>(dop); 
}

template<typename T>
void VolumeRAMPrecision<T>::setValueFromSingleFloat(const uvec3& pos, float val){
    T* data = static_cast<T*>(data_);
    getDataFormat()->floatToValue(val, &(data[posToIndex(pos, dimensions_)]));
}

template<typename T>
void VolumeRAMPrecision<T>::setValueFromVec2Float(const uvec3& pos, vec2 val){
    T* data = static_cast<T*>(data_);
    getDataFormat()->vec2ToValue(val, &(data[posToIndex(pos, dimensions_)]));
}

template<typename T>
void VolumeRAMPrecision<T>::setValueFromVec3Float(const uvec3& pos, vec3 val){
    T* data = static_cast<T*>(data_);
    getDataFormat()->vec3ToValue(val, &(data[posToIndex(pos, dimensions_)]));
}

template<typename T>
void VolumeRAMPrecision<T>::setValueFromVec4Float(const uvec3& pos, vec4 val){
    T* data = static_cast<T*>(data_);
    getDataFormat()->vec4ToValue(val, &(data[posToIndex(pos, dimensions_)]));
}

template<typename T>
float VolumeRAMPrecision<T>::getValueAsSingleFloat(const uvec3& pos) const{
    float result;
    T* data = static_cast<T*>(data_);
    T val = data[posToIndex(pos, dimensions_)];
    result = getDataFormat()->valueToNormalizedFloat(&val);
    return result;
}

template<typename T>
vec2 VolumeRAMPrecision<T>::getValueAsVec2Float(const uvec3& pos) const{
    vec2 result;
    T* data = static_cast<T*>(data_);
    T val = data[posToIndex(pos, dimensions_)];
    result = getDataFormat()->valueToNormalizedVec2Float(&val);
    return result;
}

template<typename T>
vec3 VolumeRAMPrecision<T>::getValueAsVec3Float(const uvec3& pos) const{
    vec3 result;
    T* data = static_cast<T*>(data_);
    T val = data[posToIndex(pos, dimensions_)];
    result = getDataFormat()->valueToNormalizedVec3Float(&val);
    return result;
}

template<typename T>
vec4 VolumeRAMPrecision<T>::getValueAsVec4Float(const uvec3& pos) const{
    vec4 result;
    T* data = static_cast<T*>(data_);
    T val = data[posToIndex(pos, dimensions_)];
    result = getDataFormat()->valueToNormalizedVec4Float(&val);
    return result;
}

#define DataFormatIdMacro(i) typedef VolumeRAMCustomPrecision<Data##i::type, Data##i::bits> VolumeRAM_##i;
#include <inviwo/core/util/formatsdefinefunc.h>

} // namespace

#endif // IVW_VOLUMERAMPRECISION_H
