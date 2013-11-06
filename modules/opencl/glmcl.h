#ifndef __GLMCL_H__
#define __GLMCL_H__

#include <modules/opencl/cl.hpp>
#include <glm/glm.hpp>

#include <inviwo/core/util/logdistributor.h>
#include <iostream>

namespace cl {


// Kernel argument specializations for vec3 types:
// int, uint, float

// int types
template <>
cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::int8>& value);
template <>
cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::int16>& value);
template <>
cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::int32>& value);
template <>
cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::int64>& value);

// uint types
template <>
cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::uint8>& value);
template <>
cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::uint16>& value);
template <>
cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::uint32>& value);
template <>
cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::uint64>& value);


// float types
template <>
cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::float16>& value);
template <>
cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::float32>& value);

} // namespace cl
#endif
