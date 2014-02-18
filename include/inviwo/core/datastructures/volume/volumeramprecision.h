/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

#ifndef IVW_VOLUMERAMPRECISION_H
#define IVW_VOLUMERAMPRECISION_H

#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeramoperationexecuter.h>

namespace inviwo {

template<typename T>
class VolumeRAMPrecision : public VolumeRAM {
public:
    VolumeRAMPrecision(uvec3 dimensions = uvec3(128,128,128),
                       VolumeRepresentation::VolumeBorders border = VolumeRepresentation::VolumeBorders(), const DataFormatBase* format = defaultformat());
    VolumeRAMPrecision(T* data, uvec3 dimensions = uvec3(128,128,128),
                       VolumeRepresentation::VolumeBorders border = VolumeRepresentation::VolumeBorders(), const DataFormatBase* format = defaultformat());
    VolumeRAMPrecision(const VolumeRAMPrecision<T>& rhs)
        : VolumeRAM(rhs) {
        initialize(0);
        memcpy(data_, rhs.getData(), dimensions_.x*dimensions_.y*dimensions_.z*sizeof(T));
    }

    VolumeRAMPrecision<T>& operator=(const VolumeRAMPrecision<T>& rhs) {
        if (this != &rhs) {
            VolumeRAM::operator=(rhs);
            delete[] data_;
            dimensions_ = rhs.getDimension();
            initialize(0);
            memcpy(data_, rhs.getData(), dimensions_.x*dimensions_.y*dimensions_.z*sizeof(T));
        }

        return *this;
    };
    virtual ~VolumeRAMPrecision() {
        deinitialize();
    };
    virtual void performOperation(DataOperation* dop) const;
    using VolumeRAM::initialize;
    virtual void initialize(void*);
    virtual void deinitialize();
    virtual VolumeRAMPrecision<T>* clone() const;

    virtual void setDimension(uvec3 dimensions) { dimensions_ = dimensions; deinitialize(); initialize(); }

    void setValueFromSingleFloat(const uvec3& pos, float val);
    void setValueFromVec2Float(const uvec3& pos, vec2 val);
    void setValueFromVec3Float(const uvec3& pos, vec3 val);
    void setValueFromVec4Float(const uvec3& pos, vec4 val);

    float getValueAsSingleFloat(const uvec3& pos) const;
    vec2 getValueAsVec2Float(const uvec3& pos) const;
    vec3 getValueAsVec3Float(const uvec3& pos) const;
    vec4 getValueAsVec4Float(const uvec3& pos) const;

private:
    static const DataFormatBase* defaultformat() {
        return GenericDataFormat(T)::get();
    }
};

template<typename T, size_t B>
class VolumeRAMCustomPrecision : public VolumeRAMPrecision<T> {
public:
    VolumeRAMCustomPrecision(uvec3 dimensions = uvec3(128,128,128),
                             VolumeRepresentation::VolumeBorders border = VolumeRepresentation::VolumeBorders(),
                             const DataFormatBase* format =  defaultformat()) : VolumeRAMPrecision<T>(dimensions, border, format) {};
    VolumeRAMCustomPrecision(T* data, uvec3 dimensions = uvec3(128,128,128),
                             VolumeRepresentation::VolumeBorders border = VolumeRepresentation::VolumeBorders(),
                             const DataFormatBase* format = defaultformat()) : VolumeRAMPrecision<T>(data, dimensions, border, format) {};
    virtual ~VolumeRAMCustomPrecision() {};
    void performOperation(DataOperation*) const;

private:
    static const DataFormatBase* defaultformat() {
        return DataFormat<T, B>::get();
    }
};

template<typename T>
VolumeRAMPrecision<T>::VolumeRAMPrecision(uvec3 dimensions, VolumeRepresentation::VolumeBorders border,
        const DataFormatBase* format) : VolumeRAM(dimensions, border, format) {
    initialize(0);
}

template<typename T>
VolumeRAMPrecision<T>::VolumeRAMPrecision(T* data, uvec3 dimensions, VolumeRepresentation::VolumeBorders border,
        const DataFormatBase* format) : VolumeRAM(dimensions, border, format) {
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
void inviwo::VolumeRAMPrecision<T>::deinitialize() {
    if (data_) {
        delete[] static_cast<T*>(data_);
        data_ = NULL;
    }
}

template<typename T>
VolumeRAMPrecision<T>* VolumeRAMPrecision<T>::clone() const {
    return new VolumeRAMPrecision<T>(*this);
}

template<typename T>
void VolumeRAMPrecision<T>::performOperation(DataOperation* dop) const {
    executeOperationOnVolumeRAMPrecision<T, GenericDataBits(T)>(dop);
}

template<typename T, size_t B>
void VolumeRAMCustomPrecision<T,B>::performOperation(DataOperation* dop) const {
    executeOperationOnVolumeRAMPrecision<T, B>(dop);
}

template<typename T>
void VolumeRAMPrecision<T>::setValueFromSingleFloat(const uvec3& pos, float val) {
    T* data = static_cast<T*>(data_);
    getDataFormat()->floatToValue(val, &(data[posToIndex(pos, dimensions_)]));
}

template<typename T>
void VolumeRAMPrecision<T>::setValueFromVec2Float(const uvec3& pos, vec2 val) {
    T* data = static_cast<T*>(data_);
    getDataFormat()->vec2ToValue(val, &(data[posToIndex(pos, dimensions_)]));
}

template<typename T>
void VolumeRAMPrecision<T>::setValueFromVec3Float(const uvec3& pos, vec3 val) {
    T* data = static_cast<T*>(data_);
    getDataFormat()->vec3ToValue(val, &(data[posToIndex(pos, dimensions_)]));
}

template<typename T>
void VolumeRAMPrecision<T>::setValueFromVec4Float(const uvec3& pos, vec4 val) {
    T* data = static_cast<T*>(data_);
    getDataFormat()->vec4ToValue(val, &(data[posToIndex(pos, dimensions_)]));
}

template<typename T>
float VolumeRAMPrecision<T>::getValueAsSingleFloat(const uvec3& pos) const {
    float result;
    T* data = static_cast<T*>(data_);
    T val = data[posToIndex(pos, dimensions_)];
    result = getDataFormat()->valueToNormalizedFloat(&val);
    return result;
}

template<typename T>
vec2 VolumeRAMPrecision<T>::getValueAsVec2Float(const uvec3& pos) const {
    vec2 result;
    T* data = static_cast<T*>(data_);
    T val = data[posToIndex(pos, dimensions_)];
    result = getDataFormat()->valueToNormalizedVec2Float(&val);
    return result;
}

template<typename T>
vec3 VolumeRAMPrecision<T>::getValueAsVec3Float(const uvec3& pos) const {
    vec3 result;
    T* data = static_cast<T*>(data_);
    T val = data[posToIndex(pos, dimensions_)];
    result = getDataFormat()->valueToNormalizedVec3Float(&val);
    return result;
}

template<typename T>
vec4 VolumeRAMPrecision<T>::getValueAsVec4Float(const uvec3& pos) const {
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
