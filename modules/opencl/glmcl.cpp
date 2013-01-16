

#include "glmcl.h"


namespace cl {

// Kernel argument specializations for vec3 types:
// int, uint, float

// int types
template <>
cl_int Kernel::setArg(cl_uint index, glm::detail::tvec3<glm::detail::int8> value)
{
    return setArg(index, glm::detail::tvec4<glm::detail::int8>(value, 0));
}
template <>
cl_int Kernel::setArg(cl_uint index, glm::detail::tvec3<glm::detail::int16> value)
{
    return setArg(index, glm::detail::tvec4<glm::detail::int16>(value, 0));
}
template <>
cl_int Kernel::setArg(cl_uint index, glm::detail::tvec3<glm::detail::int32> value)
{
    return setArg(index, glm::detail::tvec4<glm::detail::int32>(value, 0));
}
template <>
cl_int Kernel::setArg(cl_uint index, glm::detail::tvec3<glm::detail::int64> value)
{
    return setArg(index, glm::detail::tvec4<glm::detail::int16>(value, 0));
}

// uint types
template <>
cl_int Kernel::setArg(cl_uint index, glm::detail::tvec3<glm::detail::uint8> value)
{
    return setArg(index, glm::detail::tvec4<glm::detail::uint8>(value, 0));
}
template <>
cl_int Kernel::setArg(cl_uint index, glm::detail::tvec3<glm::detail::uint16> value)
{
    return setArg(index, glm::detail::tvec4<glm::detail::uint16>(value, 0));
}
template <>
cl_int Kernel::setArg(cl_uint index, glm::detail::tvec3<glm::detail::uint32> value)
{
    return setArg(index, glm::detail::tvec4<glm::detail::uint32>(value, 0));
}
template <>
cl_int Kernel::setArg(cl_uint index, glm::detail::tvec3<glm::detail::uint64> value)
{
    return setArg(index, glm::detail::tvec4<glm::detail::uint64>(value, 0));
}


// float types
template <>
cl_int Kernel::setArg(cl_uint index, glm::detail::tvec3<glm::detail::float16> value)
{
    return setArg(index, glm::detail::tvec4<glm::detail::float16>(value, 0));
}
template <>
cl_int Kernel::setArg(cl_uint index, glm::detail::tvec3<glm::detail::float32> value)
{
    return setArg(index, glm::detail::tvec4<glm::detail::float32>(value, 0));
}

} // namespace cl
