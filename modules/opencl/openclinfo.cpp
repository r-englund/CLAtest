#include "openclinfo.h"
#include <inviwo/core/util/logdistributor.h>
#include <modules/opencl/inviwoopencl.h>

namespace inviwo {

    std::string imageFormatToString(const cl::ImageFormat& format) {
        std::string order;
        switch (format.image_channel_order) {
            case CL_R: order = "CL_R"; break;
            case CL_Rx: order = "CL_Rx"; break;
            case CL_INTENSITY: order = "CL_INTENSITY"; break;
            case CL_LUMINANCE: order = "CL_LUMINANCE"; break;    
            case CL_RG: order = "CL_RG"; break;   
            case CL_RGx: order = "CL_RGx"; break;
            case CL_RA: order = "CL_RA"; break;
            case CL_RGB: order = "CL_RGB"; break;
            case CL_RGBx: order = "CL_RGBx"; break;
            case CL_RGBA: order = "CL_RGBA"; break;
            case CL_BGRA: order = "CL_BGRA"; break;
            case CL_ARGB: order = "CL_ARGB"; break; 
            default: order = "unkown channel order"; break;
        }
        std::string type;
        switch (format.image_channel_order) {
            case CL_SNORM_INT8: type = "CL_SNORM_INT8"; break;
            case CL_SNORM_INT16: type = "CL_SNORM_INT16"; break;
            case CL_UNORM_INT8: type = "CL_UNORM_INT8"; break;
            case CL_UNORM_INT16: type = "CL_UNORM_INT16"; break;    
            case CL_UNORM_SHORT_565: type = "CL_UNORM_SHORT_565"; break;   
            case CL_UNORM_SHORT_555: type = "CL_UNORM_SHORT_555"; break;
            case CL_UNORM_INT_101010: type = "CL_UNORM_INT_101010"; break;
            case CL_SIGNED_INT8: type = "CL_SIGNED_INT8"; break;
            case CL_SIGNED_INT16: type = "CL_SIGNED_INT16"; break;
            case CL_SIGNED_INT32: type = "CL_SIGNED_INT32"; break;
            case CL_UNSIGNED_INT8: type = "CL_UNSIGNED_INT8"; break;
            case CL_UNSIGNED_INT16: type = "CL_UNSIGNED_INT16"; break; 
            case CL_UNSIGNED_INT32: type = "CL_UNSIGNED_INT32"; break; 
            case CL_HALF_FLOAT: type = "CL_HALF_FLOAT"; break; 
            case CL_FLOAT: type = "CL_FLOAT"; break;                 
            default: type = "unkown data type"; break;
        }     
        
        std::ostringstream stream;
        stream << "(" << order << ", " << type << ")";
        return stream.str();
        
    }
    const std::string OpenCLInfo::logSource_ = "OpenCL Info";

    OpenCLInfo::OpenCLInfo() {}

    OpenCLInfo::~OpenCLInfo() {}

    void OpenCLInfo::retrieveStaticInfo(){
        
    }

    void OpenCLInfo::retrieveDynamicInfo(){
        
    }

    void OpenCLInfo::printInfo(){
        OpenCL::printDeviceInfo(OpenCL::getInstance()->getDevice());
        
        // Supported image 2D formats
        std::vector<cl::ImageFormat> formats;
        OpenCL::getInstance()->getContext().getSupportedImageFormats(CL_MEM_READ_WRITE, CL_MEM_OBJECT_IMAGE2D, &formats);
        std::ostringstream stream;
        stream << "Supported 2D READ_WRITE formats: " << imageFormatToString(formats[0]);
        for(::size_t i = 1; i < formats.size(); ++i) {
            stream << ", " << imageFormatToString(formats[i]);
        }
        LogInfoS("OpenCL", stream.str())
        formats.clear();
        OpenCL::getInstance()->getContext().getSupportedImageFormats(CL_MEM_READ_WRITE, CL_MEM_OBJECT_IMAGE3D, &formats);
        stream.clear();
        stream << "Supported 3D READ_WRITE formats: " << imageFormatToString(formats[0]);
        for(::size_t i = 1; i < formats.size(); ++i) {
            stream << ", " << imageFormatToString(formats[i]);
        }
    }

} // namespace




template< typename T>
std::string deviceInfoToString(cl_device_info info, const T& value, bool is_cl_bool) { 
    std::ostringstream stream;
    stream << std::boolalpha; // bool will print true/false
    switch(info) {
        case CL_DEVICE_TYPE: 
            switch(value) {
                case CL_DEVICE_TYPE_CPU:  stream << "CPU";  break;
                case CL_DEVICE_TYPE_GPU:  stream << "GPU";  break;
                case CL_DEVICE_TYPE_ACCELERATOR:  stream << "accelerator";  break;
            }
            break;
        case CL_DEVICE_GLOBAL_MEM_CACHE_TYPE:
            stream << "read only cache: " << static_cast<bool>((value & CL_READ_ONLY_CACHE) != 0) << ", ";
            stream << "read-write cache: " << static_cast<bool>((value & CL_READ_WRITE_CACHE) != 0);
            break;
        case CL_DEVICE_LOCAL_MEM_TYPE:
            switch(value) {
                case CL_LOCAL: stream << "local"; break;
                case CL_GLOBAL: stream << "global"; break;
                case CL_NONE: stream << "none"; break;
            }
            break;
        case CL_DEVICE_EXECUTION_CAPABILITIES:
            stream << "kernel execution: " << static_cast<bool>((value & CL_EXEC_KERNEL) != 0) << ", ";
            stream << "native kernel execution: " << static_cast<bool>((value & CL_EXEC_NATIVE_KERNEL) != 0);
            break;
        case CL_DEVICE_QUEUE_PROPERTIES:
            stream << "out of order execution: " << static_cast<bool>((value & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE) != 0) << ", ";
            stream << "profiling: " << static_cast<bool>((value & CL_QUEUE_PROFILING_ENABLE) != 0);
            break;
#if defined(CL_VERSION_1_1)
        case CL_DEVICE_SINGLE_FP_CONFIG: 
        case CL_DEVICE_DOUBLE_FP_CONFIG:
            stream << "denorms: " << static_cast<bool>((value & CL_FP_DENORM) != 0) << ", ";
            stream << "INF and quiet NaN: " << static_cast<bool>((value & CL_FP_INF_NAN) != 0) << ", ";
            stream << "round to nearest: " << static_cast<bool>((value & CL_FP_ROUND_TO_NEAREST) != 0) << ", ";
            stream << "round to zero: " << static_cast<bool>((value & CL_FP_ROUND_TO_ZERO) != 0) << ", ";
            stream << "round to inf: " << static_cast<bool>((value & CL_FP_ROUND_TO_INF) != 0) << ", ";
            stream << "IEEE754-2008 fused multiply-add: " << static_cast<bool>((value & CL_FP_FMA) != 0) << ", ";
            //stream << "IEEE754 correct divide and sqrt round: " << static_cast<bool>(value & CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT!= 0) << ", ";
            stream << "software implementation: " << static_cast<bool>((value & CL_FP_SOFT_FLOAT) != 0);
            break;
#endif
#if defined(CL_VERSION_1_2)
        case CL_DEVICE_PARTITION_AFFINITY_DOMAIN: 
            stream << "numa: " << static_cast<bool>((value & CL_DEVICE_AFFINITY_DOMAIN_NUMA) != 0) << ", ";
            stream << "L1 cache: " << static_cast<bool>((value & CL_DEVICE_AFFINITY_DOMAIN_L1_CACHE) != 0) << ", ";
            stream << "L2 cache: " << static_cast<bool>((value & CL_DEVICE_AFFINITY_DOMAIN_L2_CACHE) != 0) << ", ";
            stream << "L3 cache: " << static_cast<bool>((value & CL_DEVICE_AFFINITY_DOMAIN_L3_CACHE) != 0) << ", ";
            stream << "L4 cache: " << static_cast<bool>((value & CL_DEVICE_AFFINITY_DOMAIN_L4_CACHE) != 0) << ", ";
            stream << "next partitionable: " << static_cast<bool>((value & CL_DEVICE_AFFINITY_DOMAIN_NEXT_PARTITIONABLE) != 0);
            break;
#endif
        default: 
            if(is_cl_bool) {
                stream << static_cast<bool>(value != 0);
            } else {
                stream << value;
            }
            
    }    
    return stream.str();
}

template<typename T>
std::string deviceInfoToString(cl_device_info info, const std::vector<T>& value, bool) { 
    std::ostringstream stream;
    if(info == CL_DEVICE_MAX_WORK_ITEM_SIZES) {
        stream << "(";
        for(size_t i = 0; i < value.size()-1; ++i) {
            stream << value[i] << ", ";
        }
        stream << "("<< value.back() << ")";
    }
#if defined(CL_VERSION_1_2)
    if(info == CL_DEVICE_PARTITION_PROPERTIES || info == CL_DEVICE_PARTITION_TYPE) {
        for(size_t i = 0; i < value.size(); ++i) {
            switch(value[i]) {
                case CL_DEVICE_PARTITION_EQUALLY: stream << "partition equally"; break;
                case CL_DEVICE_PARTITION_BY_COUNTS: stream << "partition by counts"; break;
                case CL_DEVICE_PARTITION_BY_COUNTS_LIST_END: stream << "partition by counts list end"; break;
                case CL_DEVICE_PARTITION_BY_AFFINITY_DOMAIN: stream << "partition by affinity domain"; break;
            }
            if(i < value.size()-1)       
                stream << ", ";
        }
    }
#endif
#if defined(USE_CL_DEVICE_FISSION)
    if(info == CL_DEVICE_AFFINITY_DOMAINS_EXT || info == CL_DEVICE_PARTITION_TYPES_EXT || Info == CL_DEVICE_PARTITION_STYLE_EXT) {
        for(size_t i = 0; i < value.size(); ++i) {
            switch(value[i]) {
                case CL_DEVICE_PARTITION_EQUALLY_EXT: stream << "partition equally"; break;
                case CL_DEVICE_PARTITION_BY_COUNTS_EXT: stream << "partition by counts"; break;
                case CL_DEVICE_PARTITION_BY_NAMES_EXT: stream << "partition by names"; break;
                case CL_DEVICE_PARTITION_BY_AFFINITY_DOMAIN_EXT: stream << "partition by affinity domain"; break;
                case CL_PROPERTIES_LIST_END_EXT: stream << "list end"; break;
                case CL_PARTITION_BY_COUNTS_LIST_END_EXT: stream << "partition by counts list end"; break;
                case CL_PARTITION_BY_NAMES_LIST_END_EXT: stream << "partition by names list end"; break;
            }
            if(i < value.size()-1)       
                stream << ", ";
        }
    }
#endif // USE_CL_DEVICE_FISSION
    
    return stream.str();
}
// We do not print platform info so far.
template<>
std::string deviceInfoToString(cl_device_info, const cl_platform_id& value, bool) { 
    return "";
}

