#ifndef __GLMCL_H__
#define __GLMCL_H__

#include <modules/opencl/cl.hpp>
#include <glm/glm.hpp>
#include <inviwo/core/util/formats.h>
#include <inviwo/core/util/logdistributor.h>
#include <iostream>

namespace cl {

// Kernel argument specializations for vec3 types:
// int, uint, float

// int types
template <>
cl_int Kernel::setArg(cl_uint index, glm::detail::tvec3<glm::detail::int8> value);
template <>
cl_int Kernel::setArg(cl_uint index, glm::detail::tvec3<glm::detail::int16> value);
template <>
cl_int Kernel::setArg(cl_uint index, glm::detail::tvec3<glm::detail::int32> value);
template <>
cl_int Kernel::setArg(cl_uint index, glm::detail::tvec3<glm::detail::int64> value);

// uint types
template <>
cl_int Kernel::setArg(cl_uint index, glm::detail::tvec3<glm::detail::uint8> value);
template <>
cl_int Kernel::setArg(cl_uint index, glm::detail::tvec3<glm::detail::uint16> value);
template <>
cl_int Kernel::setArg(cl_uint index, glm::detail::tvec3<glm::detail::uint32> value);
template <>
cl_int Kernel::setArg(cl_uint index, glm::detail::tvec3<glm::detail::uint64> value);


// float types
template <>
cl_int Kernel::setArg(cl_uint index, glm::detail::tvec3<glm::detail::float16> value);
template <>
cl_int Kernel::setArg(cl_uint index, glm::detail::tvec3<glm::detail::float32> value);

//template <>
//cl_int Kernel::setArg(cl_uint index, const cl::Image2D& value);

// Image formats for OpenCL

template< typename T >
cl::ImageFormat typeToImageFormat() {
    //LogErrorS("OpenCL", "Image format for type " << inviwo::typeToString<T>() << " is not valid");
    return cl::ImageFormat(CL_INTENSITY, CL_UNORM_INT8);
}
template< int8_t >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_INTENSITY, CL_SNORM_INT8);
}
template< int16_t >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_INTENSITY, CL_SNORM_INT16);
}
template< int32_t >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_R, CL_SIGNED_INT32);
}

template< uint8_t >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_INTENSITY, CL_UNORM_INT8);
}
template< uint16_t >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_INTENSITY, CL_UNORM_INT16);
}
// Compiler does not allow float as template, use const reference instead.
template< const float& >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_INTENSITY, CL_FLOAT);
}
////////////////////////// vec2 types ////////////////////////////////////////////////
template< const glm::detail::tvec2<glm::detail::int8>& >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_RG, CL_SNORM_INT8);
}
template< const glm::detail::tvec2<glm::detail::int16>& >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_RG, CL_SNORM_INT16);
}
template< const glm::detail::tvec2<glm::detail::int32>& >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_RG, CL_SIGNED_INT32);
}

template< const glm::detail::tvec2<glm::detail::uint8>& >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_RG, CL_UNORM_INT8);
}
template< const glm::detail::tvec2<glm::detail::uint16>& >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_RG, CL_UNORM_INT16);
}
template< const glm::detail::tvec2<glm::detail::uint32>& >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_RG, CL_UNSIGNED_INT32);
}

template< const glm::detail::tvec2<glm::detail::float16>& >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_RG, CL_HALF_FLOAT);
}
template< const glm::detail::tvec2<glm::detail::float32>& >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_RG, CL_FLOAT);
}

//////////////////////// vec3 types ////////////////////////////////////////////////
// Not supported due because only compressed formats are possible 
// CL_RGB requires (CL_UNORM_SHORT_565, CL_UNORM_SHORT_555 or CL_UNORM_INT101010)

//////////////////////// vec4 types ////////////////////////////////////////////////
template< const glm::detail::tvec4<glm::detail::int8>& >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_RGBA, CL_SNORM_INT8);
}
template< const glm::detail::tvec4<glm::detail::int16>& >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_RGBA, CL_SNORM_INT16);
}
template< const glm::detail::tvec4<glm::detail::int32>& >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_RGBA, CL_SIGNED_INT32);
}

template< const glm::detail::tvec4<glm::detail::uint8>& >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_RGBA, CL_UNORM_INT8);
}
template< const glm::detail::tvec4<glm::detail::uint16>& >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_RGBA, CL_UNORM_INT16);
}
template< const glm::detail::tvec4<glm::detail::uint32>& >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_RGBA, CL_UNSIGNED_INT32);
}

template< const glm::detail::tvec4<glm::detail::float16>& >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_RGBA, CL_HALF_FLOAT);
}
template< const glm::detail::tvec4<glm::detail::float32>& >
cl::ImageFormat typeToImageFormat() {
    return cl::ImageFormat(CL_RGBA, CL_FLOAT);
}

} // namespace cl
#endif
