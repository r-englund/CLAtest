#ifndef IVW_TYPE_TO_STRING_H
#define IVW_TYPE_TO_STRING_H

#include <inviwo/core/util/formats.h>

#include <string>

namespace inviwo {

/*! \brief Convert data type to string representation
 *
 * @return Uppercase short name of type followed by number of bits used, no space.
 */
template<typename T>
std::string typeToString() {
    return std::string("Error, type specialization not implemented");
}
/*template< struct DataType<glm::detail::float16, 2>* >
std::string typeToString() {
    return typeToString<DataType<glm::detail::float16, 2>::type>();
}
template< const DataFLOAT32& >
std::string typeToString() {
    return typeToString<DataFLOAT32::type>();
}
/*template<>
std::string typeToString<int8_t>() {
    return "INT8";
}
template< int16_t >
std::string typeToString() {
    return "INT16";
}
template< int32_t >
std::string typeToString() {
    return "INT32";
}
template< int64_t >
std::string typeToString() {
    return "INT64";
}
template< uint8_t >
std::string typeToString() {
    return "UINT8";
}
template< uint16_t >
std::string typeToString() {
    return "UINT16";
}
template< uint32_t >
std::string typeToString() {
    return "UINT32";
}
template< uint64_t >
std::string typeToString() {
    return "UINT64";
}
// Compiler does not allow float as template, use const reference instead.
template< const float& >
std::string typeToString() {
    return "FLOAT32";
}
template< const double& >
std::string typeToString() {
    return "FLOAT64";
}*/


}
#endif
