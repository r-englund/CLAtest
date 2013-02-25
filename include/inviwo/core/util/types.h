#ifndef IVW_TYPES_H
#define IVW_TYPES_H

#include <inviwo/core/util/pstdint.h>
#include <glm/glm.hpp>
#include <limits>
#include <string>

/*! \brief Defines general useful types and new data types
 */

#define BYTES_TO_BITS(bytes) (bytes*8)

namespace inviwo {

class DataTypeBase
{
public:
    static size_t getBits() { return 0; }
    static std::string getString() { return "Error, type specialization not implemented"; }
};

template<typename T, size_t B>
class DataType : DataTypeBase
{
public:
    typedef T type;
    static size_t getBits() { return B; }
    static T getMax() { return std::numeric_limits<T>::max(); }
    static T getMin() { return std::numeric_limits<T>::min(); }
    static std::string getString() { return DataTypeBase::getString(); }
};

template <typename T>
struct GenericDataType
{
    typedef DataType<T, BYTES_TO_BITS(sizeof(T))> type;
};

// Floats
typedef GenericDataType<glm::detail::float16>::type DataFLOAT16;
typedef GenericDataType<glm::detail::float32>::type DataFLOAT32;
typedef GenericDataType<glm::detail::float64>::type DataFLOAT64;

// Integers
typedef GenericDataType<int8_t>::type   DataINT8;
typedef DataType<int16_t, 12>           DataINT12;
typedef GenericDataType<int16_t>::type  DataINT16;
typedef GenericDataType<int32_t>::type  DataINT32;
typedef GenericDataType<int64_t>::type  DataINT64;

// Unsigned Integers
typedef GenericDataType<uint8_t>::type  DataUINT8;
typedef DataType<uint16_t, 12>          DataUINT12;
typedef GenericDataType<uint16_t>::type DataUINT16;
typedef GenericDataType<uint32_t>::type DataUINT32;
typedef GenericDataType<uint64_t>::type DataUINT64;

// Min/Max Specializations
template<>
static glm::detail::float16 DataFLOAT16::getMax() { return static_cast<glm::detail::float16>(glm::detail::toFloat16(std::numeric_limits<float>::max())); }
template<>
static glm::detail::float16 DataFLOAT16::getMin() { return static_cast<glm::detail::float16>(glm::detail::toFloat16(std::numeric_limits<float>::min())); }

template<>
static int16_t DataINT12::getMax() { return static_cast<int16_t>(2047); }
template<>
static int16_t DataINT12::getMin() { return static_cast<int16_t>(-2047); }

template<>
static uint16_t DataUINT12::getMax() { return static_cast<uint16_t>(4095); }
template<>
static uint16_t DataUINT12::getMin() { return static_cast<uint16_t>(0); }

// String Function Specializations
template<>
static std::string DataFLOAT16::getString() { return "FLOAT16"; }
template<>
static std::string DataFLOAT32::getString() { return "FLOAT32"; }
template<>
static std::string DataFLOAT64::getString() { return "FLOAT64"; }

template<>
static std::string DataINT8::getString() { return "INT8"; }
template<>
static std::string DataINT12::getString() { return "INT12"; }
template<>
static std::string DataINT16::getString() { return "INT16"; }
template<>
static std::string DataINT32::getString() { return "INT32"; }
template<>
static std::string DataINT64::getString() { return "INT64"; }

template<>
static std::string DataUINT8::getString() { return "UINT8"; }
template<>
static std::string DataUINT12::getString() { return "UINT12"; }
template<>
static std::string DataUINT16::getString() { return "UINT16"; }
template<>
static std::string DataUINT32::getString() { return "UINT32"; }
template<>
static std::string DataUINT64::getString() { return "UINT64"; }

}

#endif
