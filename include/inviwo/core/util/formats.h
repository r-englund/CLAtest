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

#ifndef IVW_FORMATS_H
#define IVW_FORMATS_H

#pragma warning(disable : 4723)
#pragma warning(disable : 4756)
#pragma warning(disable : 4244) // min/max to double.

#include <inviwo/core/common/inviwocoredefine.h>
#include <limits>
#include <string>

/*! \brief Defines general useful formats and new data types
 * Non-virtual, meaning no dynamic_cast as string comparison is as fast/faster
 */

#define BYTES_TO_BITS(bytes) (bytes*8)
#define BITS_TO_BYTES(bytes) (bytes/8)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#endif

namespace inviwo {

template <unsigned int N, typename T>
class Matrix {};
template <typename T>
class Matrix<4, T> : public glm::detail::tmat4x4<T, glm::defaultp> {
public:
    Matrix<4, T>() : glm::detail::tmat4x4<T, glm::defaultp>() {};
    Matrix<4, T>(const Matrix<4, T>& m) : glm::detail::tmat4x4<T, glm::defaultp>(
            m[0][0], m[0][1], m[0][2], m[0][3],
            m[1][0], m[1][1], m[1][2], m[1][3],
            m[2][0], m[2][1], m[2][2], m[2][3],
            m[3][0], m[3][1], m[3][2], m[3][3]) {};
    Matrix<4, T>(const glm::detail::tmat4x4<T, glm::defaultp>& m) : glm::detail::tmat4x4<T, glm::defaultp>(m) {};
    Matrix<4, T>(T m) : glm::detail::tmat4x4<T, glm::defaultp>(m) {};
    Matrix<4, T>(T x1, T y1, T z1, T w1,
                 T x2, T y2, T z2, T w2,
                 T x3, T y3, T z3, T w3,
                 T x4, T y4, T z4, T w4) :
        glm::detail::tmat4x4<T, glm::defaultp>(x1, y1, z1, w1,
                                               x2, y2, z2, w2,
                                               x3, y3, z3, w3,
                                               x4, y4, z4, w4) {};
    glm::detail::tmat4x4<T, glm::defaultp> getGLM() const {
        return *this;
    };
};
template <typename T>
class Matrix<3, T> : public glm::detail::tmat3x3<T, glm::defaultp> {
public:
    Matrix<3, T>() : glm::detail::tmat3x3<T, glm::defaultp>() {};
    Matrix<3, T>(const Matrix<3, T>& m) : glm::detail::tmat3x3<T, glm::defaultp>(
            m[0][0], m[0][1], m[0][2],
            m[1][0], m[1][1], m[1][2],
            m[2][0], m[2][1], m[2][2]) {};
    Matrix<3, T>(const glm::detail::tmat3x3<T, glm::defaultp>& m) : glm::detail::tmat3x3<T, glm::defaultp>(m) {};
    Matrix<3, T>(T m) : glm::detail::tmat3x3<T, glm::defaultp>(m) {};
    Matrix<3, T>(T x1, T y1, T z1,
                 T x2, T y2, T z2,
                 T x3, T y3, T z3) :
        glm::detail::tmat3x3<T, glm::defaultp>(x1, y1, z1,
                                               x2, y2, z2,
                                               x3, y3, z3) {};
    glm::detail::tmat3x3<T, glm::defaultp> getGLM() const {
        return *this;
    };
};
template <typename T>
class Matrix<2, T> : public glm::detail::tmat2x2<T, glm::defaultp> {
public:
    Matrix<2, T>() : glm::detail::tmat2x2<T, glm::defaultp>() {};
    Matrix<2, T>(const Matrix<2, T>& m) : glm::detail::tmat2x2<T, glm::defaultp>(
            m[0][0], m[0][1],
            m[1][0], m[1][1]) {};
    Matrix<2, T>(const glm::detail::tmat2x2<T, glm::defaultp>& m) : glm::detail::tmat2x2<T, glm::defaultp>(m) {};
    Matrix<2, T>(T m) : glm::detail::tmat2x2<T, glm::defaultp>(m) {};
    Matrix<2, T>(T x1, T y1,
                 T x2, T y2) :
        glm::detail::tmat2x2<T, glm::defaultp>(x1, y1,
                                               x2, y2) {};
    glm::detail::tmat2x2<T, glm::defaultp> getGLM() const {
        return *this;
    };
};
template <unsigned int N, typename T>
class Vector {};
template <typename T>
class Vector<4, T> : public glm::detail::tvec4<T, glm::defaultp> {
public:
    Vector<4, T>() : glm::detail::tvec4<T, glm::defaultp>() {};
    Vector<4, T>(const Vector<4, T>& v) : glm::detail::tvec4<T, glm::defaultp>(v.x, v.y, v.z, v.w) {};
    Vector<4, T>(const glm::detail::tvec4<T, glm::defaultp>& v) : glm::detail::tvec4<T, glm::defaultp>(v) {};
    Vector<4, T>(T v) : glm::detail::tvec4<T, glm::defaultp>(v) {};
    Vector<4, T>(T v1, T v2, T v3, T v4) : glm::detail::tvec2<T, glm::defaultp>(v1, v2, v3, v4) {};
    glm::detail::tvec4<T, glm::defaultp> getGLM() const { return *this; };
};
template <typename T>
class Vector<3, T> : public glm::detail::tvec3<T, glm::defaultp> {
public:
    Vector<3, T>() : glm::detail::tvec3<T, glm::defaultp>() {};
    Vector<3, T>(const Vector<3, T>& v) : glm::detail::tvec3<T, glm::defaultp>(v.x, v.y, v.z) {};
    Vector<3, T>(const glm::detail::tvec3<T, glm::defaultp>& v) : glm::detail::tvec3<T, glm::defaultp>(v) {};
    Vector<3, T>(T v) : glm::detail::tvec3<T, glm::defaultp>(v) {};
    Vector<3, T>(T v1, T v2, T v3) : glm::detail::tvec3<T, glm::defaultp>(v1, v2, v3) {};
    glm::detail::tvec3<T, glm::defaultp> getGLM() const { return *this; };
};
template <typename T>
class Vector<2, T> : public glm::detail::tvec2<T, glm::defaultp> {
public:
    Vector<2, T>() : glm::detail::tvec2<T, glm::defaultp>() {};
    Vector<2, T>(const Vector<2, T>& v) : glm::detail::tvec2<T, glm::defaultp>(v.x, v.y) {};
    Vector<2, T>(const glm::detail::tvec2<T, glm::defaultp>& v) : glm::detail::tvec2<T, glm::defaultp>(v) {};
    Vector<2, T>(T v) : glm::detail::tvec2<T, glm::defaultp>(v) {};
    Vector<2, T>(T v1, T v2) : glm::detail::tvec2<T, glm::defaultp>(v1, v2) {};
    glm::detail::tvec2<T, glm::defaultp> getGLM() const { return *this; };
};

namespace DataFormatEnums {

//Do not set enums specifically, as NUMBER_OF_FORMATS is used to count the number of enums
enum Id {
    NOT_SPECIALIZED,
    FLOAT16,
    FLOAT32,
    FLOAT64,
    INT8,
    INT12,
    INT16,
    INT32,
    INT64,
    UINT8,
    UINT12,
    UINT16,
    UINT32,
    UINT64,
    Vec2FLOAT16,
    Vec2FLOAT32,
    Vec2FLOAT64,
    Vec2INT8,
    Vec2INT12,
    Vec2INT16,
    Vec2INT32,
    Vec2INT64,
    Vec2UINT8,
    Vec2UINT12,
    Vec2UINT16,
    Vec2UINT32,
    Vec2UINT64,
    Vec3FLOAT16,
    Vec3FLOAT32,
    Vec3FLOAT64,
    Vec3INT8,
    Vec3INT12,
    Vec3INT16,
    Vec3INT32,
    Vec3INT64,
    Vec3UINT8,
    Vec3UINT12,
    Vec3UINT16,
    Vec3UINT32,
    Vec3UINT64,
    Vec4FLOAT16,
    Vec4FLOAT32,
    Vec4FLOAT64,
    Vec4INT8,
    Vec4INT12,
    Vec4INT16,
    Vec4INT32,
    Vec4INT64,
    Vec4UINT8,
    Vec4UINT12,
    Vec4UINT16,
    Vec4UINT32,
    Vec4UINT64,
    NUMBER_OF_FORMATS
};

enum NumericType {
    NOT_SPECIALIZED_TYPE,
    FLOAT_TYPE,
    UNSIGNED_INTEGER_TYPE,
    SIGNED_INTEGER_TYPE
};

}

class IVW_CORE_API DataFormatBase {
public:
    DataFormatBase();
    DataFormatBase(DataFormatEnums::Id t, size_t bA, size_t bS, int c, double max, double min, DataFormatEnums::NumericType nt, std::string s);
    virtual ~DataFormatBase();

    static const DataFormatBase* get() {
        if (!instance_[DataFormatEnums::NOT_SPECIALIZED])
            instance_[DataFormatEnums::NOT_SPECIALIZED] = new DataFormatBase();

        return instance_[DataFormatEnums::NOT_SPECIALIZED];
    }

    static void cleanDataFormatBases();

    static const DataFormatBase* get(std::string name);

    static size_t bitsAllocated() { return 0; }
    static size_t bitsStored() { return 0; }
    static int components() { return 0; }
    static DataFormatEnums::NumericType numericType() { return DataFormatEnums::NOT_SPECIALIZED_TYPE; }
    static std::string str() { return "Error, type specialization not implemented"; }
    static DataFormatEnums::Id id() { return DataFormatEnums::NOT_SPECIALIZED; }

    virtual float valueToNormalizedFloat(void*) const;
    virtual vec2 valueToNormalizedVec2Float(void*) const;
    virtual vec3 valueToNormalizedVec3Float(void*) const;
    virtual vec4 valueToNormalizedVec4Float(void*) const;

    virtual void floatToValue(float, void*) const;
    virtual void vec2ToValue(vec2, void*) const;
    virtual void vec3ToValue(vec3, void*) const;
    virtual void vec4ToValue(vec4, void*) const;

    size_t getBitsAllocated() const;
    size_t getBitsStored() const;
    size_t getBytesAllocated() const;
    size_t getBytesStored() const;
    int getComponents() const;
    DataFormatEnums::NumericType getNumericType() const;
    double getMax() const;
    double getMin() const;
    const char* getString() const;
    DataFormatEnums::Id getId() const;


protected:
    static DataFormatBase* instance_[DataFormatEnums::NUMBER_OF_FORMATS];

    static DataFormatBase* getNonConst(DataFormatEnums::Id id) {
        return instance_[id];
    }

    DataFormatEnums::Id formatId_;
    size_t bitsAllocated_;
    size_t bitsStored_;
    int components_;
    DataFormatEnums::NumericType numericType_;
    double max_;
    double min_;
    std::string* formatStr_;

};

template<typename T, size_t B>
class IVW_CORE_API DataFormat : public DataFormatBase {
public:
    typedef T type;
    static const size_t bits = B;

    DataFormat() : DataFormatBase(id(),
                                      bitsAllocated(),
                                      bitsStored(),
                                      components(),
                                      maxToDouble(),
                                      minToDouble(),
                                      numericType(),
                                      str()) {
    }

    virtual ~DataFormat() {}

    static const DataFormat<T, B>* get() {
        DataFormatBase* d = DataFormatBase::getNonConst(id());

        if (!d) {
            d = new DataFormat<T, B>();
            instance_[id()] = d;
        }

        return static_cast<DataFormat<T, B>*>(d);
    }

    static size_t bitsAllocated() { return B; }
    static size_t bitsStored() { return B; }
    static int components() { return 1; }
    static DataFormatEnums::NumericType numericType() { return DataFormatEnums::NOT_SPECIALIZED_TYPE; }

    static T max() { return std::numeric_limits<T>::max(); }
    static T min() { return std::numeric_limits<T>::min(); }

    static double maxToDouble() { return static_cast<double>(max()); }
    static double minToDouble() { return static_cast<double>(min()); }

    static std::string str() { return DataFormatBase::str(); }
    static DataFormatEnums::Id id() { return DataFormatBase::id(); }

    inline float valueToNormalizedFloat(void* val) const { return DataFormatBase::valueToNormalizedFloat(val); }
    inline vec2 valueToNormalizedVec2Float(void* val) const { return DataFormatBase::valueToNormalizedVec2Float(val); }
    inline vec3 valueToNormalizedVec3Float(void* val) const { return DataFormatBase::valueToNormalizedVec3Float(val); }
    inline vec4 valueToNormalizedVec4Float(void* val) const { return DataFormatBase::valueToNormalizedVec4Float(val); }

    inline void floatToValue(float val, void* loc) const { DataFormatBase::floatToValue(val, loc); }
    inline void vec2ToValue(vec2 val, void* loc) const { DataFormatBase::vec2ToValue(val, loc); }
    inline void vec3ToValue(vec3 val, void* loc) const { DataFormatBase::vec3ToValue(val, loc); }
    inline void vec4ToValue(vec4 val, void* loc) const { DataFormatBase::vec4ToValue(val, loc); }

    //typename D = dest, typename S = src

    template<typename D, typename S>
    inline D normalizeSigned(S val) const {
        return (static_cast<D>(val) - static_cast<D>(DataFormat<S, B>::min())) /
               (static_cast<D>(DataFormat<S, B>::max()) - static_cast<D>(DataFormat<S, B>::min()));
    }

    template<typename D, typename S>
    inline D normalizeUnsigned(S val) const {
        return static_cast<D>(val) / static_cast<D>(DataFormat<S, B>::max());
    }

    template<typename D, typename S>
    inline D normalizeSignedSingle(void* val) const {
        S valT = *static_cast<S*>(val);
        return normalizeSigned<D, S>(valT);
    }

    template<typename D, typename S>
    inline D normalizeUnsignedSingle(void* val) const {
        S valT = *static_cast<S*>(val);
        return normalizeUnsigned<D, S>(valT);
    }

    template<typename D, typename S>
    inline glm::detail::tvec2<D, glm::defaultp> normalizeSignedVec2(void* val) const {
        glm::detail::tvec2<S, glm::defaultp> valT = *static_cast<glm::detail::tvec2<S, glm::defaultp>*>(val);
        glm::detail::tvec2<D, glm::defaultp> result;
        result.x = normalizeSigned<D, S>(valT.x);
        result.y = normalizeSigned<D, S>(valT.y);
        return result;
    }

    template<typename D, typename S>
    inline glm::detail::tvec2<D, glm::defaultp> normalizeUnsignedVec2(void* val) const {
        glm::detail::tvec2<S, glm::defaultp> valT = *static_cast<glm::detail::tvec2<S, glm::defaultp>*>(val);
        glm::detail::tvec2<D, glm::defaultp> result;
        result.x = normalizeUnsigned<D, S>(valT.x);
        result.y = normalizeUnsigned<D, S>(valT.y);
        return result;
    }

    template<typename D, typename S>
    inline glm::detail::tvec3<D, glm::defaultp> normalizeSignedVec3(void* val) const {
        glm::detail::tvec3<S, glm::defaultp> valT = *static_cast<glm::detail::tvec3<S, glm::defaultp>*>(val);
        glm::detail::tvec3<D, glm::defaultp> result;
        result.x = normalizeSigned<D, S>(valT.x);
        result.y = normalizeSigned<D, S>(valT.y);
        result.y = normalizeSigned<D, S>(valT.y);
        return result;
    }

    template<typename D, typename S>
    inline glm::detail::tvec3<D, glm::defaultp> normalizeUnsignedVec3(void* val) const {
        glm::detail::tvec3<S, glm::defaultp> valT = *static_cast<glm::detail::tvec3<S, glm::defaultp>*>(val);
        glm::detail::tvec3<D, glm::defaultp> result;
        result.x = normalizeUnsigned<D, S>(valT.x);
        result.y = normalizeUnsigned<D, S>(valT.y);
        result.z = normalizeUnsigned<D, S>(valT.z);
        return result;
    }

    template<typename D, typename S>
    inline glm::detail::tvec4<D, glm::defaultp> normalizeSignedVec4(void* val) const {
        glm::detail::tvec4<S, glm::defaultp> valT = *static_cast<glm::detail::tvec4<S, glm::defaultp>*>(val);
        glm::detail::tvec4<D, glm::defaultp> result;
        result.x = normalizeSigned<D, S>(valT.x);
        result.y = normalizeSigned<D, S>(valT.y);
        result.y = normalizeSigned<D, S>(valT.y);
        result.w = normalizeSigned<D, S>(valT.w);
        return result;
    }

    template<typename D, typename S>
    inline glm::detail::tvec4<D, glm::defaultp> normalizeUnsignedVec4(void* val) const {
        glm::detail::tvec4<S, glm::defaultp> valT = *static_cast<glm::detail::tvec4<S, glm::defaultp>*>(val);
        glm::detail::tvec4<D, glm::defaultp> result;
        result.x = normalizeUnsigned<D, S>(valT.x);
        result.y = normalizeUnsigned<D, S>(valT.y);
        result.z = normalizeUnsigned<D, S>(valT.z);
        result.w = normalizeUnsigned<D, S>(valT.w);
        return result;
    }

};

#define GenericDataBits(T) BYTES_TO_BITS(sizeof(T))

#define GenericDataFormat(T) DataFormat<T, GenericDataBits(T)>

/*---------------Single Value Formats------------------*/

// Floats
typedef DataFormat<glm::f32, 16>  DataFLOAT16;
typedef GenericDataFormat(glm::f32) DataFLOAT32;
typedef GenericDataFormat(glm::f64) DataFLOAT64;

// Integers
typedef GenericDataFormat(glm::i8)   DataINT8;
typedef DataFormat<glm::i16, 12>     DataINT12;
typedef GenericDataFormat(glm::i16)  DataINT16;
typedef GenericDataFormat(glm::i32)  DataINT32;
typedef GenericDataFormat(glm::i64)  DataINT64;

// Unsigned Integers
typedef GenericDataFormat(glm::u8)   DataUINT8;
typedef DataFormat<glm::u16, 12>     DataUINT12;
typedef GenericDataFormat(glm::u16)  DataUINT16;
typedef GenericDataFormat(glm::u32)  DataUINT32;
typedef GenericDataFormat(glm::u64)  DataUINT64;

/*---------------Vec2 Formats--------------------*/

// Floats
typedef DataFormat<glm::f32vec2, 32>  DataVec2FLOAT16;
typedef GenericDataFormat(glm::f32vec2) DataVec2FLOAT32;
typedef GenericDataFormat(glm::f64vec2) DataVec2FLOAT64;

// Integers
typedef GenericDataFormat(glm::i8vec2)  DataVec2INT8;
typedef DataFormat<glm::i16vec2, 24>    DataVec2INT12;
typedef GenericDataFormat(glm::i16vec2) DataVec2INT16;
typedef GenericDataFormat(glm::i32vec2) DataVec2INT32;
typedef GenericDataFormat(glm::i64vec2) DataVec2INT64;

// Unsigned Integers
typedef GenericDataFormat(glm::u8vec2)  DataVec2UINT8;
typedef DataFormat<glm::u16vec2, 24>    DataVec2UINT12;
typedef GenericDataFormat(glm::u16vec2) DataVec2UINT16;
typedef GenericDataFormat(glm::u32vec2) DataVec2UINT32;
typedef GenericDataFormat(glm::u64vec2) DataVec2UINT64;

/*---------------Vec3 Formats--------------------*/

// Floats
typedef DataFormat<glm::f32vec3, 48>  DataVec3FLOAT16;
typedef GenericDataFormat(glm::f32vec3) DataVec3FLOAT32;
typedef GenericDataFormat(glm::f64vec3) DataVec3FLOAT64;

// Integers
typedef GenericDataFormat(glm::i8vec3)  DataVec3INT8;
typedef DataFormat<glm::i16vec3, 36>    DataVec3INT12;
typedef GenericDataFormat(glm::i16vec3) DataVec3INT16;
typedef GenericDataFormat(glm::i32vec3) DataVec3INT32;
typedef GenericDataFormat(glm::i64vec3) DataVec3INT64;

// Unsigned Integers
typedef GenericDataFormat(glm::u8vec3)  DataVec3UINT8;
typedef DataFormat<glm::u16vec3, 36>    DataVec3UINT12;
typedef GenericDataFormat(glm::u16vec3) DataVec3UINT16;
typedef GenericDataFormat(glm::u32vec3) DataVec3UINT32;
typedef GenericDataFormat(glm::u64vec3) DataVec3UINT64;

/*---------------Vec4 Value Formats------------------*/

// Floats
typedef DataFormat<glm::f32vec4, 64>  DataVec4FLOAT16;
typedef GenericDataFormat(glm::f32vec4) DataVec4FLOAT32;
typedef GenericDataFormat(glm::f64vec4) DataVec4FLOAT64;

// Integers
typedef GenericDataFormat(glm::i8vec4)  DataVec4INT8;
typedef DataFormat<glm::i16vec4, 48>    DataVec4INT12;
typedef GenericDataFormat(glm::i16vec4) DataVec4INT16;
typedef GenericDataFormat(glm::i32vec4) DataVec4INT32;
typedef GenericDataFormat(glm::i64vec4) DataVec4INT64;

// Unsigned Integers
typedef GenericDataFormat(glm::u8vec4)  DataVec4UINT8;
typedef DataFormat<glm::u16vec4, 48>    DataVec4UINT12;
typedef GenericDataFormat(glm::u16vec4) DataVec4UINT16;
typedef GenericDataFormat(glm::u32vec4) DataVec4UINT32;
typedef GenericDataFormat(glm::u64vec4) DataVec4UINT64;

/*--------------- Conversions------------------*/

template<typename T>
inline glm::detail::tvec2<T, glm::defaultp> singleToVec2(T val) {
    return glm::detail::tvec2<T, glm::defaultp>(val);
}

template<typename T>
inline glm::detail::tvec3<T, glm::defaultp> singleToVec3(T val) {
    return glm::detail::tvec3<T, glm::defaultp>(val);
}

template<typename T>
inline glm::detail::tvec4<T, glm::defaultp> singleToVec4(T val) {
    return glm::detail::tvec4<T, glm::defaultp>(val);
}

template<typename T>
glm::detail::tvec3<T, glm::defaultp> vec2ToVec3(glm::detail::tvec2<T, glm::defaultp> val) {
    glm::detail::tvec3<T, glm::defaultp> result = glm::detail::tvec3<T, glm::defaultp>(0.f);
    result.xy() = val;
    return result;
}

template<typename T>
glm::detail::tvec4<T, glm::defaultp> vec2ToVec4(glm::detail::tvec2<T, glm::defaultp> val) {
    glm::detail::tvec4<T, glm::defaultp> result = glm::detail::tvec4<T, glm::defaultp>(0.f);
    result.xy() = val;
    return result;
}

template<typename T>
inline glm::detail::tvec4<T, glm::defaultp> vec3ToVec4(glm::detail::tvec3<T, glm::defaultp> val) {
    glm::detail::tvec4<T, glm::defaultp> result = glm::detail::tvec4<T, glm::defaultp>(0.f);
    result.xyz() = val;
    return result;
}

/*---------------Single Value Formats------------------*/

// Bit Specializations
template<> inline size_t DataFLOAT16::bitsAllocated() { return DataFLOAT32::bitsAllocated(); }
template<> inline size_t DataINT12::bitsAllocated() { return DataINT16::bitsAllocated(); }
template<> inline size_t DataUINT12::bitsAllocated() { return DataUINT16::bitsAllocated(); }

// Min/Max Specializations
template<> inline DataFLOAT16::type DataFLOAT16::max() { return DataFLOAT16::type(65504.f); }
template<> inline DataFLOAT16::type DataFLOAT16::min() { return DataFLOAT16::type(1.f/16384.f); }

template<> inline DataINT12::type DataINT12::max() { return static_cast<DataINT12::type>(2047); }
template<> inline DataINT12::type DataINT12::min() { return static_cast<DataINT12::type>(-2047); }

template<> inline DataUINT12::type DataUINT12::max() { return static_cast<DataUINT12::type>(4095); }
template<> inline DataUINT12::type DataUINT12::min() { return static_cast<DataUINT12::type>(0); }

// Type Function Specializations
template<> inline DataFormatEnums::Id DataFLOAT16::id() { return DataFormatEnums::FLOAT16; }
template<> inline DataFormatEnums::Id DataFLOAT32::id() { return DataFormatEnums::FLOAT32; }
template<> inline DataFormatEnums::Id DataFLOAT64::id() { return DataFormatEnums::FLOAT64; }

template<> inline DataFormatEnums::Id DataINT8::id() { return DataFormatEnums::INT8; }
template<> inline DataFormatEnums::Id DataINT12::id() { return DataFormatEnums::INT12; }
template<> inline DataFormatEnums::Id DataINT16::id() { return DataFormatEnums::INT16; }
template<> inline DataFormatEnums::Id DataINT32::id() { return DataFormatEnums::INT32; }
template<> inline DataFormatEnums::Id DataINT64::id() { return DataFormatEnums::INT64; }

template<> inline DataFormatEnums::Id DataUINT8::id() { return DataFormatEnums::UINT8; }
template<> inline DataFormatEnums::Id DataUINT12::id() { return DataFormatEnums::UINT12; }
template<> inline DataFormatEnums::Id DataUINT16::id() { return DataFormatEnums::UINT16; }
template<> inline DataFormatEnums::Id DataUINT32::id() { return DataFormatEnums::UINT32; }
template<> inline DataFormatEnums::Id DataUINT64::id() { return DataFormatEnums::UINT64; }

// String Function Specializations
template<> inline std::string DataFLOAT16::str() { return "FLOAT16"; }
template<> inline std::string DataFLOAT32::str() { return "FLOAT32"; }
template<> inline std::string DataFLOAT64::str() { return "FLOAT64"; }

template<> inline std::string DataINT8::str() { return "INT8"; }
template<> inline std::string DataINT12::str() { return "INT12"; }
template<> inline std::string DataINT16::str() { return "INT16"; }
template<> inline std::string DataINT32::str() { return "INT32"; }
template<> inline std::string DataINT64::str() { return "INT64"; }

template<> inline std::string DataUINT8::str() { return "UINT8"; }
template<> inline std::string DataUINT12::str() { return "UINT12"; }
template<> inline std::string DataUINT16::str() { return "UINT16"; }
template<> inline std::string DataUINT32::str() { return "UINT32"; }
template<> inline std::string DataUINT64::str() { return "UINT64"; }

// Type Conversion Specializations
#define DatatoFloat(F) \
    template<> inline void F::floatToValue(float val, void* loc) const { *static_cast<F::type*>(loc) = static_cast<F::type>(val); } \
    template<> inline void F::vec2ToValue(vec2 val, void* loc) const { *static_cast<F::type*>(loc) = static_cast<F::type>(val.x); } \
    template<> inline void F::vec3ToValue(vec3 val, void* loc) const { *static_cast<F::type*>(loc) = static_cast<F::type>(val.x); } \
    template<> inline void F::vec4ToValue(vec4 val, void* loc) const { *static_cast<F::type*>(loc) = static_cast<F::type>(val.x); }

#define DataUnchanged(F) \
    template<> inline DataFormatEnums::NumericType F::numericType() { return DataFormatEnums::FLOAT_TYPE; } \
    template<> inline float F::valueToNormalizedFloat(void* val) const { return *static_cast<float*>(val); } \
    template<> inline vec2 F::valueToNormalizedVec2Float(void* val) const { return singleToVec2<float>(*static_cast<float*>(val)); } \
    template<> inline vec3 F::valueToNormalizedVec3Float(void* val) const { return singleToVec3<float>(*static_cast<float*>(val)); } \
    template<> inline vec4 F::valueToNormalizedVec4Float(void* val) const { return singleToVec4<float>(*static_cast<float*>(val)); } \
    DatatoFloat(F)

#define DataNormalizedSignedSingle(F) \
    template<> inline DataFormatEnums::NumericType F::numericType() { return DataFormatEnums::SIGNED_INTEGER_TYPE; } \
    template<> inline float F::valueToNormalizedFloat(void* val) const { return normalizeSignedSingle<float, F::type>(val); } \
    template<> inline vec2 F::valueToNormalizedVec2Float(void* val) const { return singleToVec2<float>(normalizeSignedSingle<float, F::type>(val)); } \
    template<> inline vec3 F::valueToNormalizedVec3Float(void* val) const { return singleToVec3<float>(normalizeSignedSingle<float, F::type>(val)); } \
    template<> inline vec4 F::valueToNormalizedVec4Float(void* val) const { return singleToVec4<float>(normalizeSignedSingle<float, F::type>(val)); } \
    DatatoFloat(F)

#define DataNormalizedUnsignedSingle(F) \
    template<> inline DataFormatEnums::NumericType F::numericType() { return DataFormatEnums::UNSIGNED_INTEGER_TYPE; } \
    template<> inline float F::valueToNormalizedFloat(void* val) const { return normalizeUnsignedSingle<float, F::type>(val); } \
    template<> inline vec2 F::valueToNormalizedVec2Float(void* val) const { return singleToVec2<float>(normalizeUnsignedSingle<float, F::type>(val)); } \
    template<> inline vec3 F::valueToNormalizedVec3Float(void* val) const { return singleToVec3<float>(normalizeUnsignedSingle<float, F::type>(val)); } \
    template<> inline vec4 F::valueToNormalizedVec4Float(void* val) const { return singleToVec4<float>(normalizeUnsignedSingle<float, F::type>(val)); } \
    DatatoFloat(F)

DataUnchanged(DataFLOAT16)
DataUnchanged(DataFLOAT32)
DataUnchanged(DataFLOAT64)

DataNormalizedSignedSingle(DataINT8)
DataNormalizedSignedSingle(DataINT12)
DataNormalizedSignedSingle(DataINT16)
DataNormalizedSignedSingle(DataINT32)
DataNormalizedSignedSingle(DataINT64)

DataNormalizedUnsignedSingle(DataUINT8)
DataNormalizedUnsignedSingle(DataUINT12)
DataNormalizedUnsignedSingle(DataUINT16)
DataNormalizedUnsignedSingle(DataUINT32)
DataNormalizedUnsignedSingle(DataUINT64)

/*---------------Vec2 Formats--------------------*/

// Bit Specializations
template<> inline size_t DataVec2FLOAT16::bitsAllocated() { return DataVec2FLOAT32::bitsAllocated(); }
template<> inline size_t DataVec2INT12::bitsAllocated() { return DataVec2INT16::bitsAllocated(); }
template<> inline size_t DataVec2UINT12::bitsAllocated() { return DataVec2UINT16::bitsAllocated(); }

// Min/Max Specializations
template<> inline DataVec2FLOAT16::type DataVec2FLOAT16::max() { return DataVec2FLOAT16::type(DataFLOAT16::max()); }
template<> inline DataVec2FLOAT16::type DataVec2FLOAT16::min() { return DataVec2FLOAT16::type(DataFLOAT16::min()); }
template<> inline DataVec2FLOAT32::type DataVec2FLOAT32::max() { return DataVec2FLOAT32::type(DataFLOAT32::max()); }
template<> inline DataVec2FLOAT32::type DataVec2FLOAT32::min() { return DataVec2FLOAT32::type(DataFLOAT32::min()); }
template<> inline DataVec2FLOAT64::type DataVec2FLOAT64::max() { return DataVec2FLOAT64::type(DataFLOAT64::max()); }
template<> inline DataVec2FLOAT64::type DataVec2FLOAT64::min() { return DataVec2FLOAT64::type(DataFLOAT64::min()); }

template<> inline DataVec2INT8::type DataVec2INT8::max() { return DataVec2INT8::type(DataINT8::max()); }
template<> inline DataVec2INT8::type DataVec2INT8::min() { return DataVec2INT8::type(DataINT8::min()); }
template<> inline DataVec2INT12::type DataVec2INT12::max() { return DataVec2INT12::type(DataINT12::max()); }
template<> inline DataVec2INT12::type DataVec2INT12::min() { return DataVec2INT12::type(DataINT12::min()); }
template<> inline DataVec2INT16::type DataVec2INT16::max() { return DataVec2INT16::type(DataINT16::max()); }
template<> inline DataVec2INT16::type DataVec2INT16::min() { return DataVec2INT16::type(DataINT16::min()); }
template<> inline DataVec2INT32::type DataVec2INT32::max() { return DataVec2INT32::type(DataINT32::max()); }
template<> inline DataVec2INT32::type DataVec2INT32::min() { return DataVec2INT32::type(DataINT32::min()); }
template<> inline DataVec2INT64::type DataVec2INT64::max() { return DataVec2INT64::type(DataINT64::max()); }
template<> inline DataVec2INT64::type DataVec2INT64::min() { return DataVec2INT64::type(DataINT64::min()); }

template<> inline DataVec2UINT8::type DataVec2UINT8::max() { return DataVec2UINT8::type(DataUINT8::max()); }
template<> inline DataVec2UINT8::type DataVec2UINT8::min() { return DataVec2UINT8::type(DataUINT8::min()); }
template<> inline DataVec2UINT12::type DataVec2UINT12::max() { return DataVec2UINT12::type(DataUINT12::max()); }
template<> inline DataVec2UINT12::type DataVec2UINT12::min() { return DataVec2UINT12::type(DataUINT12::min()); }
template<> inline DataVec2UINT16::type DataVec2UINT16::max() { return DataVec2UINT16::type(DataUINT16::max()); }
template<> inline DataVec2UINT16::type DataVec2UINT16::min() { return DataVec2UINT16::type(DataUINT16::min()); }
template<> inline DataVec2UINT32::type DataVec2UINT32::max() { return DataVec2UINT32::type(DataUINT32::max()); }
template<> inline DataVec2UINT32::type DataVec2UINT32::min() { return DataVec2UINT32::type(DataUINT32::min()); }
template<> inline DataVec2UINT64::type DataVec2UINT64::max() { return DataVec2UINT64::type(DataUINT64::max()); }
template<> inline DataVec2UINT64::type DataVec2UINT64::min() { return DataVec2UINT64::type(DataUINT64::min()); }

// Type Function Specializations
template<> inline DataFormatEnums::Id DataVec2FLOAT16::id() { return DataFormatEnums::Vec2FLOAT16; }
template<> inline DataFormatEnums::Id DataVec2FLOAT32::id() { return DataFormatEnums::Vec2FLOAT32; }
template<> inline DataFormatEnums::Id DataVec2FLOAT64::id() { return DataFormatEnums::Vec2FLOAT64; }

template<> inline DataFormatEnums::Id DataVec2INT8::id() { return DataFormatEnums::Vec2INT8; }
template<> inline DataFormatEnums::Id DataVec2INT12::id() { return DataFormatEnums::Vec2INT12; }
template<> inline DataFormatEnums::Id DataVec2INT16::id() { return DataFormatEnums::Vec2INT16; }
template<> inline DataFormatEnums::Id DataVec2INT32::id() { return DataFormatEnums::Vec2INT32; }
template<> inline DataFormatEnums::Id DataVec2INT64::id() { return DataFormatEnums::Vec2INT64; }

template<> inline DataFormatEnums::Id DataVec2UINT8::id() { return DataFormatEnums::Vec2UINT8; }
template<> inline DataFormatEnums::Id DataVec2UINT12::id() { return DataFormatEnums::Vec2UINT12; }
template<> inline DataFormatEnums::Id DataVec2UINT16::id() { return DataFormatEnums::Vec2UINT16; }
template<> inline DataFormatEnums::Id DataVec2UINT32::id() { return DataFormatEnums::Vec2UINT32; }
template<> inline DataFormatEnums::Id DataVec2UINT64::id() { return DataFormatEnums::Vec2UINT64; }

// String Function Specializations
template<> inline std::string DataVec2FLOAT16::str() { return "Vec2FLOAT16"; }
template<> inline std::string DataVec2FLOAT32::str() { return "Vec2FLOAT32"; }
template<> inline std::string DataVec2FLOAT64::str() { return "Vec2FLOAT64"; }

template<> inline std::string DataVec2INT8::str() { return "Vec2INT8"; }
template<> inline std::string DataVec2INT12::str() { return "Vec2INT12"; }
template<> inline std::string DataVec2INT16::str() { return "Vec2INT16"; }
template<> inline std::string DataVec2INT32::str() { return "Vec2INT32"; }
template<> inline std::string DataVec2INT64::str() { return "Vec2INT64"; }

template<> inline std::string DataVec2UINT8::str() { return "Vec2UINT8"; }
template<> inline std::string DataVec2UINT12::str() { return "Vec2UINT12"; }
template<> inline std::string DataVec2UINT16::str() { return "Vec2UINT16"; }
template<> inline std::string DataVec2UINT32::str() { return "Vec2UINT32"; }
template<> inline std::string DataVec2UINT64::str() { return "Vec2UINT64"; }

//Vec to Single
#define DataVecToSingle(F, G) \
    template<> inline double F::maxToDouble() { return max().x; } \
    template<> inline double F::minToDouble() { return min().x; }

// Type Conversion Specializations
#define DatatoVec2t(F, G) \
    DataVecToSingle(F, G) \
    template<> inline int F::components() { return 2; } \
    template<> inline void F::floatToValue(float val, void* loc) const { *static_cast<F::type*>(loc) = singleToVec2<G::type>(static_cast<G::type>(val)); } \
    template<> inline void F::vec2ToValue(vec2 val, void* loc) const { *static_cast<F::type*>(loc) = F::type(static_cast<G::type>(val.x), static_cast<G::type>(val.y)); } \
    template<> inline void F::vec3ToValue(vec3 val, void* loc) const { *static_cast<F::type*>(loc) = F::type(static_cast<G::type>(val.x), static_cast<G::type>(val.y)); } \
    template<> inline void F::vec4ToValue(vec4 val, void* loc) const { *static_cast<F::type*>(loc) = F::type(static_cast<G::type>(val.x), static_cast<G::type>(val.y)); }

#define DataUnchangedVec2(F, G) \
    template<> inline DataFormatEnums::NumericType F::numericType() { return DataFormatEnums::FLOAT_TYPE; } \
    template<> inline float F::valueToNormalizedFloat(void* val) const { return static_cast<float>(static_cast<F::type*>(val)->x); } \
    template<> inline vec2 F::valueToNormalizedVec2Float(void* val) const { return vec2(*static_cast<F::type*>(val)); } \
    template<> inline vec3 F::valueToNormalizedVec3Float(void* val) const { return vec2ToVec3<float>(vec2(*static_cast<F::type*>(val))); } \
    template<> inline vec4 F::valueToNormalizedVec4Float(void* val) const { return vec2ToVec4<float>(vec2(*static_cast<F::type*>(val))); } \
    DatatoVec2t(F, G)

#define DataNormalizedSignedVec2(F, G) \
    template<> inline DataFormatEnums::NumericType F::numericType() { return DataFormatEnums::SIGNED_INTEGER_TYPE; } \
    template<> inline float F::valueToNormalizedFloat(void* val) const { return normalizeSignedVec2<float, G::type>(val).x; } \
    template<> inline vec2 F::valueToNormalizedVec2Float(void* val) const { return normalizeSignedVec2<float, G::type>(val); } \
    template<> inline vec3 F::valueToNormalizedVec3Float(void* val) const { return vec2ToVec3<float>(normalizeSignedVec2<float, G::type>(val)); } \
    template<> inline vec4 F::valueToNormalizedVec4Float(void* val) const { return vec2ToVec4<float>(normalizeSignedVec2<float, G::type>(val)); } \
    DatatoVec2t(F, G)

#define DataNormalizedUnsignedVec2(F, G) \
    template<> inline DataFormatEnums::NumericType F::numericType() { return DataFormatEnums::UNSIGNED_INTEGER_TYPE; } \
    template<> inline float F::valueToNormalizedFloat(void* val) const { return normalizeUnsignedVec2<float, G::type>(val).x; } \
    template<> inline vec2 F::valueToNormalizedVec2Float(void* val) const { return normalizeUnsignedVec2<float, G::type>(val); } \
    template<> inline vec3 F::valueToNormalizedVec3Float(void* val) const { return vec2ToVec3<float>(normalizeUnsignedVec2<float, G::type>(val)); } \
    template<> inline vec4 F::valueToNormalizedVec4Float(void* val) const { return vec2ToVec4<float>(normalizeUnsignedVec2<float, G::type>(val)); } \
    DatatoVec2t(F, G)

DataUnchangedVec2(DataVec2FLOAT16, DataFLOAT16)
DataUnchangedVec2(DataVec2FLOAT32, DataFLOAT32)
DataUnchangedVec2(DataVec2FLOAT64, DataFLOAT64)

DataNormalizedSignedVec2(DataVec2INT8, DataINT8)
DataNormalizedSignedVec2(DataVec2INT12, DataINT12)
DataNormalizedSignedVec2(DataVec2INT16, DataINT16)
DataNormalizedSignedVec2(DataVec2INT32, DataINT32)
DataNormalizedSignedVec2(DataVec2INT64, DataINT64)

DataNormalizedUnsignedVec2(DataVec2UINT8, DataUINT8)
DataNormalizedUnsignedVec2(DataVec2UINT12, DataUINT12)
DataNormalizedUnsignedVec2(DataVec2UINT16, DataUINT16)
DataNormalizedUnsignedVec2(DataVec2UINT32, DataUINT32)
DataNormalizedUnsignedVec2(DataVec2UINT64, DataUINT64)

/*---------------Vec3 Formats--------------------*/

// Bit Specializations
template<> inline size_t DataVec3FLOAT16::bitsAllocated() { return DataVec3FLOAT32::bitsAllocated(); }
template<> inline size_t DataVec3INT12::bitsAllocated() { return DataVec3INT16::bitsAllocated(); }
template<> inline size_t DataVec3UINT12::bitsAllocated() { return DataVec3UINT16::bitsAllocated(); }

// Min/Max Specializations
template<> inline DataVec3FLOAT16::type DataVec3FLOAT16::max() { return DataVec3FLOAT16::type(DataFLOAT16::max()); }
template<> inline DataVec3FLOAT16::type DataVec3FLOAT16::min() { return DataVec3FLOAT16::type(DataFLOAT16::min()); }
template<> inline DataVec3FLOAT32::type DataVec3FLOAT32::max() { return DataVec3FLOAT32::type(DataFLOAT32::max()); }
template<> inline DataVec3FLOAT32::type DataVec3FLOAT32::min() { return DataVec3FLOAT32::type(DataFLOAT32::min()); }
template<> inline DataVec3FLOAT64::type DataVec3FLOAT64::max() { return DataVec3FLOAT64::type(DataFLOAT64::max()); }
template<> inline DataVec3FLOAT64::type DataVec3FLOAT64::min() { return DataVec3FLOAT64::type(DataFLOAT64::min()); }

template<> inline DataVec3INT8::type DataVec3INT8::max() { return DataVec3INT8::type(DataINT8::max()); }
template<> inline DataVec3INT8::type DataVec3INT8::min() { return DataVec3INT8::type(DataINT8::min()); }
template<> inline DataVec3INT12::type DataVec3INT12::max() { return DataVec3INT12::type(DataINT12::max()); }
template<> inline DataVec3INT12::type DataVec3INT12::min() { return DataVec3INT12::type(DataINT12::min()); }
template<> inline DataVec3INT16::type DataVec3INT16::max() { return DataVec3INT16::type(DataINT16::max()); }
template<> inline DataVec3INT16::type DataVec3INT16::min() { return DataVec3INT16::type(DataINT16::min()); }
template<> inline DataVec3INT32::type DataVec3INT32::max() { return DataVec3INT32::type(DataINT32::max()); }
template<> inline DataVec3INT32::type DataVec3INT32::min() { return DataVec3INT32::type(DataINT32::min()); }
template<> inline DataVec3INT64::type DataVec3INT64::max() { return DataVec3INT64::type(DataINT64::max()); }
template<> inline DataVec3INT64::type DataVec3INT64::min() { return DataVec3INT64::type(DataINT64::min()); }

template<> inline DataVec3UINT8::type DataVec3UINT8::max() { return DataVec3UINT8::type(DataUINT8::max()); }
template<> inline DataVec3UINT8::type DataVec3UINT8::min() { return DataVec3UINT8::type(DataUINT8::min()); }
template<> inline DataVec3UINT12::type DataVec3UINT12::max() { return DataVec3UINT12::type(DataUINT12::max()); }
template<> inline DataVec3UINT12::type DataVec3UINT12::min() { return DataVec3UINT12::type(DataUINT12::min()); }
template<> inline DataVec3UINT16::type DataVec3UINT16::max() { return DataVec3UINT16::type(DataUINT16::max()); }
template<> inline DataVec3UINT16::type DataVec3UINT16::min() { return DataVec3UINT16::type(DataUINT16::min()); }
template<> inline DataVec3UINT32::type DataVec3UINT32::max() { return DataVec3UINT32::type(DataUINT32::max()); }
template<> inline DataVec3UINT32::type DataVec3UINT32::min() { return DataVec3UINT32::type(DataUINT32::min()); }
template<> inline DataVec3UINT64::type DataVec3UINT64::max() { return DataVec3UINT64::type(DataUINT64::max()); }
template<> inline DataVec3UINT64::type DataVec3UINT64::min() { return DataVec3UINT64::type(DataUINT64::min()); }

// Type Function Specializations
template<> inline DataFormatEnums::Id DataVec3FLOAT16::id() { return DataFormatEnums::Vec3FLOAT16; }
template<> inline DataFormatEnums::Id DataVec3FLOAT32::id() { return DataFormatEnums::Vec3FLOAT32; }
template<> inline DataFormatEnums::Id DataVec3FLOAT64::id() { return DataFormatEnums::Vec3FLOAT64; }

template<> inline DataFormatEnums::Id DataVec3INT8::id() { return DataFormatEnums::Vec3INT8; }
template<> inline DataFormatEnums::Id DataVec3INT12::id() { return DataFormatEnums::Vec3INT12; }
template<> inline DataFormatEnums::Id DataVec3INT16::id() { return DataFormatEnums::Vec3INT16; }
template<> inline DataFormatEnums::Id DataVec3INT32::id() { return DataFormatEnums::Vec3INT32; }
template<> inline DataFormatEnums::Id DataVec3INT64::id() { return DataFormatEnums::Vec3INT64; }

template<> inline DataFormatEnums::Id DataVec3UINT8::id() { return DataFormatEnums::Vec3UINT8; }
template<> inline DataFormatEnums::Id DataVec3UINT12::id() { return DataFormatEnums::Vec3UINT12; }
template<> inline DataFormatEnums::Id DataVec3UINT16::id() { return DataFormatEnums::Vec3UINT16; }
template<> inline DataFormatEnums::Id DataVec3UINT32::id() { return DataFormatEnums::Vec3UINT32; }
template<> inline DataFormatEnums::Id DataVec3UINT64::id() { return DataFormatEnums::Vec3UINT64; }

// String Function Specializations
template<> inline std::string DataVec3FLOAT16::str() { return "Vec3FLOAT16"; }
template<> inline std::string DataVec3FLOAT32::str() { return "Vec3FLOAT32"; }
template<> inline std::string DataVec3FLOAT64::str() { return "Vec3FLOAT64"; }

template<> inline std::string DataVec3INT8::str() { return "Vec3INT8"; }
template<> inline std::string DataVec3INT12::str() { return "Vec3INT12"; }
template<> inline std::string DataVec3INT16::str() { return "Vec3INT16"; }
template<> inline std::string DataVec3INT32::str() { return "Vec3INT32"; }
template<> inline std::string DataVec3INT64::str() { return "Vec3INT64"; }

template<> inline std::string DataVec3UINT8::str() { return "Vec3UINT8"; }
template<> inline std::string DataVec3UINT12::str() { return "Vec3UINT12"; }
template<> inline std::string DataVec3UINT16::str() { return "Vec3UINT16"; }
template<> inline std::string DataVec3UINT32::str() { return "Vec3UINT32"; }
template<> inline std::string DataVec3UINT64::str() { return "Vec3UINT64"; }

// Type Conversion Specializations
#define DataToVec3t(F, G) \
    DataVecToSingle(F, G) \
    template<> inline int F::components() { return 3; } \
    template<> inline void F::floatToValue(float val, void* loc) const { *static_cast<F::type*>(loc) = singleToVec3<G::type>(static_cast<G::type>(val)); } \
    template<> inline void F::vec2ToValue(vec2 val, void* loc) const { *static_cast<F::type*>(loc) = F::type(static_cast<G::type>(val.x), static_cast<G::type>(val.y), 0.f); } \
    template<> inline void F::vec3ToValue(vec3 val, void* loc) const { *static_cast<F::type*>(loc) = F::type(static_cast<G::type>(val.x), static_cast<G::type>(val.y), static_cast<G::type>(val.z)); } \
    template<> inline void F::vec4ToValue(vec4 val, void* loc) const { *static_cast<F::type*>(loc) = F::type(static_cast<G::type>(val.x), static_cast<G::type>(val.y), static_cast<G::type>(val.z)); }

#define DataUnchangedVec3(F, G) \
    template<> inline DataFormatEnums::NumericType F::numericType() { return DataFormatEnums::FLOAT_TYPE; } \
    template<> inline float F::valueToNormalizedFloat(void* val) const { return static_cast<float>(static_cast<F::type*>(val)->x); } \
    template<> inline vec2 F::valueToNormalizedVec2Float(void* val) const { return vec2(static_cast<F::type*>(val)->x, static_cast<F::type*>(val)->y); } \
    template<> inline vec3 F::valueToNormalizedVec3Float(void* val) const { return vec3(static_cast<F::type*>(val)->x, static_cast<F::type*>(val)->y, static_cast<F::type*>(val)->z); } \
    template<> inline vec4 F::valueToNormalizedVec4Float(void* val) const { return vec3ToVec4<float>(vec3(*static_cast<F::type*>(val))); } \
    DataToVec3t(F, G)

#define DataNormalizedSignedVec3(F, G) \
    template<> inline DataFormatEnums::NumericType F::numericType() { return DataFormatEnums::SIGNED_INTEGER_TYPE; } \
    template<> inline float F::valueToNormalizedFloat(void* val) const { return normalizeSignedVec3<float, G::type>(val).x; } \
    template<> inline vec2 F::valueToNormalizedVec2Float(void* val) const { return normalizeSignedVec2<float, G::type>(val).xy(); } \
    template<> inline vec3 F::valueToNormalizedVec3Float(void* val) const { return normalizeSignedVec3<float, G::type>(val); } \
    template<> inline vec4 F::valueToNormalizedVec4Float(void* val) const { return vec3ToVec4<float>(normalizeSignedVec3<float, G::type>(val)); } \
    DataToVec3t(F, G)

#define DataNormalizedUnsignedVec3(F, G) \
    template<> inline DataFormatEnums::NumericType F::numericType() { return DataFormatEnums::UNSIGNED_INTEGER_TYPE; } \
    template<> inline float F::valueToNormalizedFloat(void* val) const { return normalizeUnsignedVec3<float, G::type>(val).x; } \
    template<> inline vec2 F::valueToNormalizedVec2Float(void* val) const { return normalizeUnsignedVec3<float, G::type>(val).xy(); } \
    template<> inline vec3 F::valueToNormalizedVec3Float(void* val) const { return normalizeUnsignedVec3<float, G::type>(val); } \
    template<> inline vec4 F::valueToNormalizedVec4Float(void* val) const { return vec3ToVec4<float>(normalizeUnsignedVec3<float, G::type>(val)); } \
    DataToVec3t(F, G)

DataUnchangedVec3(DataVec3FLOAT16, DataFLOAT16)
DataUnchangedVec3(DataVec3FLOAT32, DataFLOAT32)
DataUnchangedVec3(DataVec3FLOAT64, DataFLOAT64)

DataNormalizedSignedVec3(DataVec3INT8, DataINT8)
DataNormalizedSignedVec3(DataVec3INT12, DataINT12)
DataNormalizedSignedVec3(DataVec3INT16, DataINT16)
DataNormalizedSignedVec3(DataVec3INT32, DataINT32)
DataNormalizedSignedVec3(DataVec3INT64, DataINT64)

DataNormalizedUnsignedVec3(DataVec3UINT8, DataUINT8)
DataNormalizedUnsignedVec3(DataVec3UINT12, DataUINT12)
DataNormalizedUnsignedVec3(DataVec3UINT16, DataUINT16)
DataNormalizedUnsignedVec3(DataVec3UINT32, DataUINT32)
DataNormalizedUnsignedVec3(DataVec3UINT64, DataUINT64)

/*---------------Vec4 Formats--------------------*/

// Bit Specializations
template<> inline size_t DataVec4FLOAT16::bitsAllocated() { return DataVec4FLOAT32::bitsAllocated(); }
template<> inline size_t DataVec4INT12::bitsAllocated() { return DataVec4INT16::bitsAllocated(); }
template<> inline size_t DataVec4UINT12::bitsAllocated() { return DataVec4UINT16::bitsAllocated(); }

// Min/Max Specializations
template<> inline DataVec4FLOAT16::type DataVec4FLOAT16::max() { return DataVec4FLOAT16::type(DataFLOAT16::max()); }
template<> inline DataVec4FLOAT16::type DataVec4FLOAT16::min() { return DataVec4FLOAT16::type(DataFLOAT16::min()); }
template<> inline DataVec4FLOAT32::type DataVec4FLOAT32::max() { return DataVec4FLOAT32::type(DataFLOAT32::max()); }
template<> inline DataVec4FLOAT32::type DataVec4FLOAT32::min() { return DataVec4FLOAT32::type(DataFLOAT32::min()); }
template<> inline DataVec4FLOAT64::type DataVec4FLOAT64::max() { return DataVec4FLOAT64::type(DataFLOAT64::max()); }
template<> inline DataVec4FLOAT64::type DataVec4FLOAT64::min() { return DataVec4FLOAT64::type(DataFLOAT64::min()); }

template<> inline DataVec4INT8::type DataVec4INT8::max() { return DataVec4INT8::type(DataINT8::max()); }
template<> inline DataVec4INT8::type DataVec4INT8::min() { return DataVec4INT8::type(DataINT8::min()); }
template<> inline DataVec4INT12::type DataVec4INT12::max() { return DataVec4INT12::type(DataINT12::max()); }
template<> inline DataVec4INT12::type DataVec4INT12::min() { return DataVec4INT12::type(DataINT12::min()); }
template<> inline DataVec4INT16::type DataVec4INT16::max() { return DataVec4INT16::type(DataINT16::max()); }
template<> inline DataVec4INT16::type DataVec4INT16::min() { return DataVec4INT16::type(DataINT16::min()); }
template<> inline DataVec4INT32::type DataVec4INT32::max() { return DataVec4INT32::type(DataINT32::max()); }
template<> inline DataVec4INT32::type DataVec4INT32::min() { return DataVec4INT32::type(DataINT32::min()); }
template<> inline DataVec4INT64::type DataVec4INT64::max() { return DataVec4INT64::type(DataINT64::max()); }
template<> inline DataVec4INT64::type DataVec4INT64::min() { return DataVec4INT64::type(DataINT64::min()); }

template<> inline DataVec4UINT8::type DataVec4UINT8::max() { return DataVec4UINT8::type(DataUINT8::max()); }
template<> inline DataVec4UINT8::type DataVec4UINT8::min() { return DataVec4UINT8::type(DataUINT8::min()); }
template<> inline DataVec4UINT12::type DataVec4UINT12::max() { return DataVec4UINT12::type(DataUINT12::max()); }
template<> inline DataVec4UINT12::type DataVec4UINT12::min() { return DataVec4UINT12::type(DataUINT12::min()); }
template<> inline DataVec4UINT16::type DataVec4UINT16::max() { return DataVec4UINT16::type(DataUINT16::max()); }
template<> inline DataVec4UINT16::type DataVec4UINT16::min() { return DataVec4UINT16::type(DataUINT16::min()); }
template<> inline DataVec4UINT32::type DataVec4UINT32::max() { return DataVec4UINT32::type(DataUINT32::max()); }
template<> inline DataVec4UINT32::type DataVec4UINT32::min() { return DataVec4UINT32::type(DataUINT32::min()); }
template<> inline DataVec4UINT64::type DataVec4UINT64::max() { return DataVec4UINT64::type(DataUINT64::max()); }
template<> inline DataVec4UINT64::type DataVec4UINT64::min() { return DataVec4UINT64::type(DataUINT64::min()); }

// Type Function Specializations
template<> inline DataFormatEnums::Id DataVec4FLOAT16::id() { return DataFormatEnums::Vec4FLOAT16; }
template<> inline DataFormatEnums::Id DataVec4FLOAT32::id() { return DataFormatEnums::Vec4FLOAT32; }
template<> inline DataFormatEnums::Id DataVec4FLOAT64::id() { return DataFormatEnums::Vec4FLOAT64; }

template<> inline DataFormatEnums::Id DataVec4INT8::id() { return DataFormatEnums::Vec4INT8; }
template<> inline DataFormatEnums::Id DataVec4INT12::id() { return DataFormatEnums::Vec4INT12; }
template<> inline DataFormatEnums::Id DataVec4INT16::id() { return DataFormatEnums::Vec4INT16; }
template<> inline DataFormatEnums::Id DataVec4INT32::id() { return DataFormatEnums::Vec4INT32; }
template<> inline DataFormatEnums::Id DataVec4INT64::id() { return DataFormatEnums::Vec4INT64; }

template<> inline DataFormatEnums::Id DataVec4UINT8::id() { return DataFormatEnums::Vec4UINT8; }
template<> inline DataFormatEnums::Id DataVec4UINT12::id() { return DataFormatEnums::Vec4UINT12; }
template<> inline DataFormatEnums::Id DataVec4UINT16::id() { return DataFormatEnums::Vec4UINT16; }
template<> inline DataFormatEnums::Id DataVec4UINT32::id() { return DataFormatEnums::Vec4UINT32; }
template<> inline DataFormatEnums::Id DataVec4UINT64::id() { return DataFormatEnums::Vec4UINT64; }

// String Function Specializations
template<> inline std::string DataVec4FLOAT16::str() { return "Vec4FLOAT16"; }
template<> inline std::string DataVec4FLOAT32::str() { return "Vec4FLOAT32"; }
template<> inline std::string DataVec4FLOAT64::str() { return "Vec4FLOAT64"; }

template<> inline std::string DataVec4INT8::str() { return "Vec4INT8"; }
template<> inline std::string DataVec4INT12::str() { return "Vec4INT12"; }
template<> inline std::string DataVec4INT16::str() { return "Vec4INT16"; }
template<> inline std::string DataVec4INT32::str() { return "Vec4INT32"; }
template<> inline std::string DataVec4INT64::str() { return "Vec4INT64"; }

template<> inline std::string DataVec4UINT8::str() { return "Vec4UINT8"; }
template<> inline std::string DataVec4UINT12::str() { return "Vec4UINT12"; }
template<> inline std::string DataVec4UINT16::str() { return "Vec4UINT16"; }
template<> inline std::string DataVec4UINT32::str() { return "Vec4UINT32"; }
template<> inline std::string DataVec4UINT64::str() { return "Vec4UINT64"; }

// Type Conversion Specializations
#define DataToVec4t(F, G) \
    DataVecToSingle(F, G) \
    template<> inline int F::components() { return 4; } \
    template<> inline void F::floatToValue(float val, void* loc) const { *static_cast<F::type*>(loc) = singleToVec4<G::type>(static_cast<G::type>(val)); } \
    template<> inline void F::vec2ToValue(vec2 val, void* loc) const { *static_cast<F::type*>(loc) = F::type(static_cast<G::type>(val.x), static_cast<G::type>(val.y), 0.f, 1.f); } \
    template<> inline void F::vec3ToValue(vec3 val, void* loc) const { *static_cast<F::type*>(loc) = F::type(static_cast<G::type>(val.x), static_cast<G::type>(val.y), static_cast<G::type>(val.z), 1.f); } \
    template<> inline void F::vec4ToValue(vec4 val, void* loc) const { *static_cast<F::type*>(loc) = F::type(static_cast<G::type>(val.x), static_cast<G::type>(val.y), static_cast<G::type>(val.z), static_cast<G::type>(val.w)); }

#define DataUnchangedVec4(F, G) \
    template<> inline DataFormatEnums::NumericType F::numericType() { return DataFormatEnums::FLOAT_TYPE; } \
    template<> inline float F::valueToNormalizedFloat(void* val) const { return static_cast<float>((*static_cast<F::type*>(val)).x); } \
    template<> inline vec2 F::valueToNormalizedVec2Float(void* val) const { return vec2(static_cast<F::type*>(val)->x, static_cast<F::type*>(val)->y); } \
    template<> inline vec3 F::valueToNormalizedVec3Float(void* val) const { return vec3(static_cast<F::type*>(val)->x, static_cast<F::type*>(val)->y, static_cast<F::type*>(val)->z); } \
    template<> inline vec4 F::valueToNormalizedVec4Float(void* val) const { return vec4(*static_cast<F::type*>(val)); } \
    DataToVec4t(F, G)

#define DataNormalizedSignedVec4(F, G) \
    template<> inline DataFormatEnums::NumericType F::numericType() { return DataFormatEnums::SIGNED_INTEGER_TYPE; } \
    template<> inline float F::valueToNormalizedFloat(void* val) const { return normalizeSignedVec4<float, G::type>(val).x; } \
    template<> inline vec2 F::valueToNormalizedVec2Float(void* val) const { return normalizeSignedVec4<float, G::type>(val).xy(); } \
    template<> inline vec3 F::valueToNormalizedVec3Float(void* val) const { return normalizeSignedVec4<float, G::type>(val).xyz(); } \
    template<> inline vec4 F::valueToNormalizedVec4Float(void* val) const { return normalizeSignedVec4<float, G::type>(val); } \
    DataToVec4t(F, G)

#define DataNormalizedUnsignedVec4(F, G) \
    template<> inline DataFormatEnums::NumericType F::numericType() { return DataFormatEnums::UNSIGNED_INTEGER_TYPE; } \
    template<> inline float F::valueToNormalizedFloat(void* val) const { return normalizeUnsignedVec4<float, G::type>(val).x; } \
    template<> inline vec2 F::valueToNormalizedVec2Float(void* val) const { return normalizeUnsignedVec4<float, G::type>(val).xy(); } \
    template<> inline vec3 F::valueToNormalizedVec3Float(void* val) const { return normalizeUnsignedVec4<float, G::type>(val).xyz(); } \
    template<> inline vec4 F::valueToNormalizedVec4Float(void* val) const { return normalizeUnsignedVec4<float, G::type>(val); } \
    DataToVec4t(F, G)

DataUnchangedVec4(DataVec4FLOAT16, DataFLOAT16)
DataUnchangedVec4(DataVec4FLOAT32, DataFLOAT32)
DataUnchangedVec4(DataVec4FLOAT64, DataFLOAT64)

DataNormalizedSignedVec4(DataVec4INT8, DataINT8)
DataNormalizedSignedVec4(DataVec4INT12, DataINT12)
DataNormalizedSignedVec4(DataVec4INT16, DataINT16)
DataNormalizedSignedVec4(DataVec4INT32, DataINT32)
DataNormalizedSignedVec4(DataVec4INT64, DataINT64)

DataNormalizedUnsignedVec4(DataVec4UINT8, DataUINT8)
DataNormalizedUnsignedVec4(DataVec4UINT12, DataUINT12)
DataNormalizedUnsignedVec4(DataVec4UINT16, DataUINT16)
DataNormalizedUnsignedVec4(DataVec4UINT32, DataUINT32)
DataNormalizedUnsignedVec4(DataVec4UINT64, DataUINT64)




#define CallFunctionWithTemplateArgsForType(fun, id) \
    switch (id) {\
    case DataFormatEnums::FLOAT16: fun<DataFLOAT16::type, DataFLOAT16::bits>(); break; \
    case DataFormatEnums::FLOAT32: fun<DataFLOAT32::type, DataFLOAT32::bits>(); break; \
    case DataFormatEnums::FLOAT64: fun<DataFLOAT64::type, DataFLOAT64::bits>(); break; \
    case DataFormatEnums::INT8: fun<DataINT8::type, DataINT8::bits>(); break; \
    case DataFormatEnums::INT12: fun<DataINT12::type, DataINT12::bits>(); break; \
    case DataFormatEnums::INT16: fun<DataINT16::type, DataINT16::bits>(); break; \
    case DataFormatEnums::INT32: fun<DataINT32::type, DataINT32::bits>(); break; \
    case DataFormatEnums::INT64: fun<DataINT64::type, DataINT64::bits>(); break; \
    case DataFormatEnums::UINT8: fun<DataUINT8::type, DataUINT8::bits>(); break; \
    case DataFormatEnums::UINT12: fun<DataUINT12::type, DataUINT12::bits>(); break; \
    case DataFormatEnums::UINT16: fun<DataUINT16::type, DataUINT16::bits>(); break; \
    case DataFormatEnums::UINT32: fun<DataUINT32::type, DataUINT32::bits>(); break; \
    case DataFormatEnums::UINT64: fun<DataUINT64::type, DataUINT64::bits>(); break; \
    case DataFormatEnums::Vec2FLOAT16: fun<DataVec2FLOAT16::type, DataVec2FLOAT16::bits>(); break; \
    case DataFormatEnums::Vec2FLOAT32: fun<DataVec2FLOAT32::type, DataVec2FLOAT32::bits>(); break; \
    case DataFormatEnums::Vec2FLOAT64: fun<DataVec2FLOAT64::type, DataVec2FLOAT64::bits>(); break; \
    case DataFormatEnums::Vec2INT8: fun<DataVec2INT8::type, DataVec2INT8::bits>(); break; \
    case DataFormatEnums::Vec2INT12: fun<DataVec2INT12::type, DataVec2INT12::bits>(); break; \
    case DataFormatEnums::Vec2INT16: fun<DataVec2INT16::type, DataVec2INT16::bits>(); break; \
    case DataFormatEnums::Vec2INT32: fun<DataVec2INT32::type, DataVec2INT32::bits>(); break; \
    case DataFormatEnums::Vec2INT64: fun<DataVec2INT64::type, DataVec2INT64::bits>(); break; \
    case DataFormatEnums::Vec2UINT8: fun<DataVec2UINT8::type, DataVec2UINT8::bits>(); break; \
    case DataFormatEnums::Vec2UINT12: fun<DataVec2UINT12::type, DataVec2UINT12::bits>(); break; \
    case DataFormatEnums::Vec2UINT16: fun<DataVec2UINT16::type, DataVec2UINT16::bits>(); break; \
    case DataFormatEnums::Vec2UINT32: fun<DataVec2UINT32::type, DataVec2UINT32::bits>(); break; \
    case DataFormatEnums::Vec2UINT64: fun<DataVec2UINT64::type, DataVec2UINT64::bits>(); break; \
    case DataFormatEnums::Vec3FLOAT16: fun<DataVec3FLOAT16::type, DataVec3FLOAT16::bits>(); break; \
    case DataFormatEnums::Vec3FLOAT32: fun<DataVec3FLOAT32::type, DataVec3FLOAT32::bits>(); break; \
    case DataFormatEnums::Vec3FLOAT64: fun<DataVec3FLOAT64::type, DataVec3FLOAT64::bits>(); break; \
    case DataFormatEnums::Vec3INT8: fun<DataVec3INT8::type, DataVec3INT8::bits>(); break; \
    case DataFormatEnums::Vec3INT12: fun<DataVec3INT12::type, DataVec3INT12::bits>(); break; \
    case DataFormatEnums::Vec3INT16: fun<DataVec3INT16::type, DataVec3INT16::bits>(); break; \
    case DataFormatEnums::Vec3INT32: fun<DataVec3INT32::type, DataVec3INT32::bits>(); break; \
    case DataFormatEnums::Vec3INT64: fun<DataVec3INT64::type, DataVec3INT64::bits>(); break; \
    case DataFormatEnums::Vec3UINT8: fun<DataVec3UINT8::type, DataVec3UINT8::bits>(); break; \
    case DataFormatEnums::Vec3UINT12: fun<DataVec3UINT12::type, DataVec3UINT12::bits>(); break; \
    case DataFormatEnums::Vec3UINT16: fun<DataVec3UINT16::type, DataVec3UINT16::bits>(); break; \
    case DataFormatEnums::Vec3UINT32: fun<DataVec3UINT32::type, DataVec3UINT32::bits>(); break; \
    case DataFormatEnums::Vec3UINT64: fun<DataVec3UINT64::type, DataVec3UINT64::bits>(); break; \
    case DataFormatEnums::Vec4FLOAT16: fun<DataVec4FLOAT16::type, DataVec4FLOAT16::bits>(); break; \
    case DataFormatEnums::Vec4FLOAT32: fun<DataVec4FLOAT32::type, DataVec4FLOAT32::bits>(); break; \
    case DataFormatEnums::Vec4FLOAT64: fun<DataVec4FLOAT64::type, DataVec4FLOAT64::bits>(); break; \
    case DataFormatEnums::Vec4INT8: fun<DataVec4INT8::type, DataVec4INT8::bits>(); break; \
    case DataFormatEnums::Vec4INT12: fun<DataVec4INT12::type, DataVec4INT12::bits>(); break; \
    case DataFormatEnums::Vec4INT16: fun<DataVec4INT16::type, DataVec4INT16::bits>(); break; \
    case DataFormatEnums::Vec4INT32: fun<DataVec4INT32::type, DataVec4INT32::bits>(); break; \
    case DataFormatEnums::Vec4INT64: fun<DataVec4INT64::type, DataVec4INT64::bits>(); break; \
    case DataFormatEnums::Vec4UINT8: fun<DataVec4UINT8::type, DataVec4UINT8::bits>(); break; \
    case DataFormatEnums::Vec4UINT12: fun<DataVec4UINT12::type, DataVec4UINT12::bits>(); break; \
    case DataFormatEnums::Vec4UINT16: fun<DataVec4UINT16::type, DataVec4UINT16::bits>(); break; \
    case DataFormatEnums::Vec4UINT32: fun<DataVec4UINT32::type, DataVec4UINT32::bits>(); break; \
    case DataFormatEnums::Vec4UINT64: fun<DataVec4UINT64::type, DataVec4UINT64::bits>(); break; \
    default: break; \
}


}

#endif
