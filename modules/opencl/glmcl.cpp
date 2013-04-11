

#include <modules/opencl/glmcl.h>


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

cl::ImageFormat typeToImageFormat( inviwo::DataFormatId format )
{
    switch (format) {
        case inviwo::UINT8:
            return cl::ImageFormat(CL_INTENSITY, CL_UNORM_INT8); 
        case inviwo::UINT16:
            return cl::ImageFormat(CL_INTENSITY, CL_UNORM_INT16);
        case inviwo::UINT32:
            return cl::ImageFormat(CL_R, CL_UNSIGNED_INT32);
        case inviwo::INT8:
            return cl::ImageFormat(CL_INTENSITY, CL_SNORM_INT8);
        case inviwo::INT16:
            return cl::ImageFormat(CL_INTENSITY, CL_SNORM_INT16);
        case inviwo::INT32:
            return cl::ImageFormat(CL_R, CL_SIGNED_INT32);
        case inviwo::FLOAT32:
            return cl::ImageFormat(CL_INTENSITY, CL_FLOAT);
        case inviwo::Vec4UINT8:
            return cl::ImageFormat(CL_RGBA, CL_UNORM_INT8);
    }
    LogErrorS("cl::ImageFormat typeToImageFormat", "Format not implmented yet");
    return cl::ImageFormat(CL_INTENSITY, CL_UNORM_INT8);
}

//template <>
//cl_int Kernel::setArg(cl_uint index, const cl::Image2D& value)
//{
//    return setArg(index, value());
//}

} // namespace cl
