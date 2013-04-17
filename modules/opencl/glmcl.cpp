

#include <modules/opencl/glmcl.h>
#include <modules/opencl/inviwoopencl.h>


namespace cl {

// Kernel argument specializations for vec3 types:
// int, uint, float

// int types
template <>
cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::int8>& value)
{
    return setArg(index, glm::detail::tvec4<glm::detail::int8>(value, 0));
}
template <>
cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::int16>& value)
{
    return setArg(index, glm::detail::tvec4<glm::detail::int16>(value, 0));
}
template <>
cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::int32>& value)
{
    return setArg(index, glm::detail::tvec4<glm::detail::int32>(value, 0));
}
template <>
cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::int64>& value)
{
    return setArg(index, glm::detail::tvec4<glm::detail::int16>(value, 0));
}

// uint types
template <>
cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::uint8>& value)
{
    return setArg(index, glm::detail::tvec4<glm::detail::uint8>(value, 0));
}
template <>
cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::uint16>& value)
{
    return setArg(index, glm::detail::tvec4<glm::detail::uint16>(value, 0));
}
template <>
cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::uint32>& value)
{
    return setArg(index, glm::detail::tvec4<glm::detail::uint32>(value, 0));
}
template <>
cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::uint64>& value)
{
    return setArg(index, glm::detail::tvec4<glm::detail::uint64>(value, 0));
}


// float types
template <>
cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::float16>& value)
{
    return setArg(index, glm::detail::tvec4<glm::detail::float16>(value, 0));
}
template <>
cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::float32>& value)
{
    return setArg(index, glm::detail::tvec4<glm::detail::float32>(value, 0));
}

cl::ImageFormat typeToImageFormat( inviwo::DataFormatId format )
{   
    cl::ImageFormat clFormat;
    switch (format) {
        case inviwo::UINT8:
            clFormat = cl::ImageFormat(CL_INTENSITY, CL_UNORM_INT8);  break;
        case inviwo::UINT16:
            clFormat = cl::ImageFormat(CL_INTENSITY, CL_UNORM_INT16); break;
        case inviwo::UINT32:
            clFormat = cl::ImageFormat(CL_R, CL_UNSIGNED_INT32); break;
        case inviwo::INT8:
            clFormat = cl::ImageFormat(CL_INTENSITY, CL_SNORM_INT8); break;
        case inviwo::INT16:
            clFormat = cl::ImageFormat(CL_INTENSITY, CL_SNORM_INT16); break;
        case inviwo::INT32:
            clFormat = cl::ImageFormat(CL_R, CL_SIGNED_INT32); break;
        case inviwo::FLOAT32:
            clFormat = cl::ImageFormat(CL_INTENSITY, CL_FLOAT); break;
        case inviwo::Vec4UINT8:
            clFormat = cl::ImageFormat(CL_RGBA, CL_UNORM_INT8); break; // TODO: Find out why CL_UNORM_INT8 does not work
        case inviwo::Vec4FLOAT32:
            clFormat = cl::ImageFormat(CL_RGBA, CL_FLOAT); break;
        default:
            LogErrorCustom("cl::ImageFormat typeToImageFormat", "Format not implmented yet"); break;

    }
#ifdef DEBUG
    if (!inviwo::OpenCL::isValidImageFormat(inviwo::OpenCL::getInstance()->getContext(), clFormat)) {
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "OpenCL device does not support format");
    };
#endif 
    
    return clFormat;
}

//template <>
//cl_int Kernel::setArg(cl_uint index, const cl::Image2D& value)
//{
//    return setArg(index, value());
//}

} // namespace cl
