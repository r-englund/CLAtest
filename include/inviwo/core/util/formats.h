#ifndef IVW_FORMATS_H
#define IVW_FORMATS_H

#include <inviwo/core/util/pstdint.h>
#include <glm/glm.hpp>
#include <limits>
#include <inviwo/core/util/stringconversion.h>

/*! \brief Defines general useful formats and new data types
 * Non-virtual, meaning no dynamic_cast as string comparison is as fast/faster
 */

#define BYTES_TO_BITS(bytes) (bytes*8)
#define BITS_TO_BYTES(bytes) (bytes/8)

namespace inviwo {

    //Do not set enums specifically, as NUMBER_OF_FORMATS is used to count the number of enums
    enum DataFormatId{
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

/*---------------Data Format Classes Defines-----------------------*/

class DataFormatBase
{
public:
    DataFormatBase() : formatId_(id()), bitsAllocated_(bitsAllocated()), bitsStored_(bitsStored()), formatStr_(str()){}
    DataFormatBase(DataFormatId t, size_t bA, size_t bS, std::string s) : formatId_(t), bitsAllocated_(bA), bitsStored_(bS), formatStr_(s){}

    static size_t bitsAllocated() { return 0; }
    static size_t bitsStored() { return 0; }
    static std::string str() { return "Error, type specialization not implemented"; }
    static DataFormatId id() { return NOT_SPECIALIZED; }

    size_t getBitsAllocated() const { return bitsAllocated_; }
    size_t getBitsStored() const { return bitsStored_; }
    size_t getBytesAllocated() const { return static_cast<size_t>(glm::ceil(BITS_TO_BYTES(static_cast<float>(getBitsAllocated())))); }
    size_t getBytesStored() const { return static_cast<size_t>(glm::ceil(BITS_TO_BYTES(static_cast<float>(getBitsStored())))); }
    std::string getString() const { return formatStr_; }
    DataFormatId getId() const { return formatId_; }
protected:
    DataFormatId formatId_;
    size_t bitsAllocated_;
    size_t bitsStored_;
    std::string formatStr_;
};

template<typename T, size_t B>
class DataFormat : public DataFormatBase
{
public:
    typedef T type;
    static const size_t bits = B;

    DataFormat() : DataFormatBase(id(), bitsAllocated(), bitsStored(), str()){}

    static size_t bitsAllocated() { return B; }
    static size_t bitsStored() { return B; }

    static T max() { return std::numeric_limits<T>::max(); }
    static T min() { return std::numeric_limits<T>::min(); }

    static std::string str() { return DataFormatBase::str(); }
    static DataFormatId id() { return DataFormatBase::id(); }
};

template<typename T, size_t B, size_t C>
class DataVecFormat : public DataFormatBase
{
public:
    static const size_t bits = DataFormat<T, B>::bits*C;

    DataVecFormat() : DataFormatBase(id(), bitsAllocated(), bitsStored(), str()){}

    static size_t bitsAllocated() { return DataFormat<T, B>::bitsAllocated()*C; }
    static size_t bitsStored() { return DataFormat<T, B>::bitsStored()*C; }

    static T max() { return DataFormat<T, B>::max(); }
    static T min() { return DataFormat<T, B>::min(); }

    static std::string str() { return "Vec"+toString<size_t>(C)+DataFormat<T, B>::str(); }
    static DataFormatId id() { return DataFormat<T, B>::id(); }
};

template<typename T, size_t B>
class DataVec2Format : public DataVecFormat<T, B, 2>
{
public:
    typedef glm::detail::tvec2<T> type;
    DataVec2Format() : DataVecFormat(){}
};

template<typename T, size_t B>
class DataVec3Format : public DataVecFormat<T, B, 3>
{
public:
    typedef glm::detail::tvec3<T> type;
    DataVec3Format() : DataVecFormat(){}
};

template<typename T, size_t B>
class DataVec4Format : public DataVecFormat<T, B, 4>
{
public:
    typedef glm::detail::tvec4<T> type;
    DataVec4Format() : DataVecFormat(){}
};

/*---------------Generic Defines-----------------------*/

#define GenericDataBits(T) BYTES_TO_BITS(sizeof(T))

#define GenericDataFormat(T) DataFormat<T, GenericDataBits(T)>

#define GenericDataVec2Format(T) DataVec2Format<T, GenericDataBits(T)>
#define GenericDataVec3Format(T) DataVec3Format<T, GenericDataBits(T)>
#define GenericDataVec4Format(T) DataVec4Format<T, GenericDataBits(T)>

/*---------------Single Value Formats------------------*/

// Floats
typedef GenericDataFormat(glm::detail::float16) DataFLOAT16;
typedef GenericDataFormat(glm::detail::float32) DataFLOAT32;
typedef GenericDataFormat(glm::detail::float64) DataFLOAT64;

// Integers
typedef GenericDataFormat(int8_t)         DataINT8;
typedef DataFormat<int16_t, 12>           DataINT12;
typedef GenericDataFormat(int16_t)        DataINT16;
typedef GenericDataFormat(int32_t)        DataINT32;
typedef GenericDataFormat(int64_t)        DataINT64;

// Unsigned Integers
typedef GenericDataFormat(uint8_t)        DataUINT8;
typedef DataFormat<uint16_t, 12>          DataUINT12;
typedef GenericDataFormat(uint16_t)       DataUINT16;
typedef GenericDataFormat(uint32_t)       DataUINT32;
typedef GenericDataFormat(uint64_t)       DataUINT64;

// Bit Specializations
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
template<> inline DataFormatId DataFLOAT16::id() { return FLOAT16; }
template<> inline DataFormatId DataFLOAT32::id() { return FLOAT32; }
template<> inline DataFormatId DataFLOAT64::id() { return FLOAT64; }

template<> inline DataFormatId DataINT8::id() { return INT8; }
template<> inline DataFormatId DataINT12::id() { return INT12; }
template<> inline DataFormatId DataINT16::id() { return INT16; }
template<> inline DataFormatId DataINT32::id() { return INT32; }
template<> inline DataFormatId DataINT64::id() { return INT64; }

template<> inline DataFormatId DataUINT8::id() { return UINT8; }
template<> inline DataFormatId DataUINT12::id() { return UINT12; }
template<> inline DataFormatId DataUINT16::id() { return UINT16; }
template<> inline DataFormatId DataUINT32::id() { return UINT32; }
template<> inline DataFormatId DataUINT64::id() { return UINT64; }

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

/*---------------Vec2 Formats------------------*/

// Floats
typedef GenericDataVec2Format(glm::detail::float16) DataVec2FLOAT16;
typedef GenericDataVec2Format(glm::detail::float32) DataVec2FLOAT32;
typedef GenericDataVec2Format(glm::detail::float64) DataVec2FLOAT64;

// Integers
typedef GenericDataVec2Format(int8_t)         DataVec2INT8;
typedef DataVec2Format<int16_t, 12>           DataVec2INT12;
typedef GenericDataVec2Format(int16_t)        DataVec2INT16;
typedef GenericDataVec2Format(int32_t)        DataVec2INT32;
typedef GenericDataVec2Format(int64_t)        DataVec2INT64;

// Unsigned Integers
typedef GenericDataVec2Format(uint8_t)        DataVec2UINT8;
typedef DataVec2Format<uint16_t, 12>          DataVec2UINT12;
typedef GenericDataVec2Format(uint16_t)       DataVec2UINT16;
typedef GenericDataVec2Format(uint32_t)       DataVec2UINT32;
typedef GenericDataVec2Format(uint64_t)       DataVec2UINT64;

// Type Function Specializations
/*template<> inline DataFormatId DataVec2FLOAT16::id() { return Vec2FLOAT16; }
template<> inline DataFormatId DataVec2FLOAT32::id() { return Vec2FLOAT32; }
template<> inline DataFormatId DataVec2FLOAT64::id() { return Vec2FLOAT64; }

template<> inline DataFormatId DataVec2INT8::id() { return Vec2INT8; }
template<> inline DataFormatId DataVec2INT12::id() { return Vec2INT12; }
template<> inline DataFormatId DataVec2INT16::id() { return Vec2INT16; }
template<> inline DataFormatId DataVec2INT32::id() { return Vec2INT32; }
template<> inline DataFormatId DataVec2INT64::id() { return Vec2INT64; }

template<> inline DataFormatId DataVec2UINT8::id() { return Vec2UINT8; }
template<> inline DataFormatId DataVec2UINT12::id() { return Vec2UINT12; }
template<> inline DataFormatId DataVec2UINT16::id() { return Vec2UINT16; }
template<> inline DataFormatId DataVec2UINT32::id() { return Vec2UINT32; }
template<> inline DataFormatId DataVec2UINT64::id() { return Vec2UINT64; }*/

/*---------------Vec3 Formats------------------*/

// Floats
typedef GenericDataVec3Format(glm::detail::float16) DataVec3FLOAT16;
typedef GenericDataVec3Format(glm::detail::float32) DataVec3FLOAT32;
typedef GenericDataVec3Format(glm::detail::float64) DataVec3FLOAT64;

// Integers
typedef GenericDataVec3Format(int8_t)         DataVec3INT8;
typedef DataVec3Format<int16_t, 12>           DataVec3INT12;
typedef GenericDataVec3Format(int16_t)        DataVec3INT16;
typedef GenericDataVec3Format(int32_t)        DataVec3INT32;
typedef GenericDataVec3Format(int64_t)        DataVec3INT64;

// Unsigned Integers
typedef GenericDataVec3Format(uint8_t)        DataVec3UINT8;
typedef DataVec3Format<uint16_t, 12>          DataVec3UINT12;
typedef GenericDataVec3Format(uint16_t)       DataVec3UINT16;
typedef GenericDataVec3Format(uint32_t)       DataVec3UINT32;
typedef GenericDataVec3Format(uint64_t)       DataVec3UINT64;

// Type Function Specializations
/*template<> inline DataFormatId DataVec3FLOAT16::id() { return Vec3FLOAT16; }
template<> inline DataFormatId DataVec3FLOAT32::id() { return Vec3FLOAT32; }
template<> inline DataFormatId DataVec3FLOAT64::id() { return Vec3FLOAT64; }

template<> inline DataFormatId DataVec3INT8::id() { return Vec3INT8; }
template<> inline DataFormatId DataVec3INT12::id() { return Vec3INT12; }
template<> inline DataFormatId DataVec3INT16::id() { return Vec3INT16; }
template<> inline DataFormatId DataVec3INT32::id() { return Vec3INT32; }
template<> inline DataFormatId DataVec3INT64::id() { return Vec3INT64; }

template<> inline DataFormatId DataVec3UINT8::id() { return Vec3UINT8; }
template<> inline DataFormatId DataVec3UINT12::id() { return Vec3UINT12; }
template<> inline DataFormatId DataVec3UINT16::id() { return Vec3UINT16; }
template<> inline DataFormatId DataVec3UINT32::id() { return Vec3UINT32; }
template<> inline DataFormatId DataVec3UINT64::id() { return Vec3UINT64; }*/

/*---------------Vec4 Formats------------------*/

// Floats
typedef GenericDataVec4Format(glm::detail::float16) DataVec4FLOAT16;
typedef GenericDataVec4Format(glm::detail::float32) DataVec4FLOAT32;
typedef GenericDataVec4Format(glm::detail::float64) DataVec4FLOAT64;

// Integers
typedef GenericDataVec4Format(int8_t)         DataVec4INT8;
typedef DataVec4Format<int16_t, 12>           DataVec4INT12;
typedef GenericDataVec4Format(int16_t)        DataVec4INT16;
typedef GenericDataVec4Format(int32_t)        DataVec4INT32;
typedef GenericDataVec4Format(int64_t)        DataVec4INT64;

// Unsigned Integers
typedef GenericDataVec4Format(uint8_t)        DataVec4UINT8;
typedef DataVec4Format<uint16_t, 12>          DataVec4UINT12;
typedef GenericDataVec4Format(uint16_t)       DataVec4UINT16;
typedef GenericDataVec4Format(uint32_t)       DataVec4UINT32;
typedef GenericDataVec4Format(uint64_t)       DataVec4UINT64;

// Type Function Specializations
/*template<> inline DataFormatId DataVec4FLOAT16::id() { return Vec4FLOAT16; }
template<> inline DataFormatId DataVec4FLOAT32::id() { return Vec4FLOAT32; }
template<> inline DataFormatId DataVec4FLOAT64::id() { return Vec4FLOAT64; }

template<> inline DataFormatId DataVec4INT8::id() { return Vec4INT8; }
template<> inline DataFormatId DataVec4INT12::id() { return Vec4INT12; }
template<> inline DataFormatId DataVec4INT16::id() { return Vec4INT16; }
template<> inline DataFormatId DataVec4INT32::id() { return Vec4INT32; }
template<> inline DataFormatId DataVec4INT64::id() { return Vec4INT64; }

template<> inline DataFormatId DataVec4UINT8::id() { return Vec4UINT8; }
template<> inline DataFormatId DataVec4UINT12::id() { return Vec4UINT12; }
template<> inline DataFormatId DataVec4UINT16::id() { return Vec4UINT16; }
template<> inline DataFormatId DataVec4UINT32::id() { return Vec4UINT32; }
template<> inline DataFormatId DataVec4UINT64::id() { return Vec4UINT64; }*/

}

#endif
