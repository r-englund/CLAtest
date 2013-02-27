#ifndef IVW_FORMATS_H
#define IVW_FORMATS_H

#include <inviwo/core/util/pstdint.h>
#include <glm/glm.hpp>
#include <limits>
#include <string>

/*! \brief Defines general useful formats and new data types
 * Non-virtual, meaning no dynamic_cast as string comparison is as fast/faster
 */

#define BYTES_TO_BITS(bytes) (bytes*8)
#define BITS_TO_BYTES(bytes) (bytes/8)

namespace inviwo {

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
        UINT64
    };

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

#define GenericDataFormat(T) DataFormat<T, BYTES_TO_BITS(sizeof(T))>

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

}

#endif
