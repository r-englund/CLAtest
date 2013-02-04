#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/cl.hpp>
#include <modules/opencl/glmcl.h>
#include <inviwo/core/util/logdistributor.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <fstream>


#ifdef WIN32
#define OPEN_FILE(a,b,c) fopen_s(&a, b, c);
#else
#define OPEN_FILE(a,b,c) a = fopen(b, c);
#endif


namespace cl {
    void LogOpenCLError(cl_int err, const char* fileName, const char* functionName, int lineNumber) {
        if (err != CL_SUCCESS) {
            std::ostringstream errorMessage;
            errorMessage << "OpenCL Error " << err << ": " << cl::errorCodeToString(err);
            inviwo::LogCentral::instance()->log("OpenCL", inviwo::Error, fileName, functionName, lineNumber, (errorMessage.str()));
        }
    }
    std::string errorCodeToString(cl_int err) {
        std::string error; 
        switch (err) {
        case CL_SUCCESS: error = "CL_SUCCESS"; break; //                                                                         0
        case CL_DEVICE_NOT_FOUND: error = "CL_DEVICE_NOT_FOUND"; break; //                                                      -1
        case CL_DEVICE_NOT_AVAILABLE: error = "CL_DEVICE_NOT_AVAILABLE"; break; //                                              -2
        case CL_COMPILER_NOT_AVAILABLE: error = "CL_COMPILER_NOT_AVAILABLE"; break; //                                          -3
        case CL_MEM_OBJECT_ALLOCATION_FAILURE: error = "CL_MEM_OBJECT_ALLOCATION_FAILURE"; break; //                            -4
        case CL_OUT_OF_RESOURCES: error = "CL_OUT_OF_RESOURCES"; break; //                                                      -5
        case CL_OUT_OF_HOST_MEMORY: error = "CL_OUT_OF_HOST_MEMORY"; break; //                                                  -6
        case CL_PROFILING_INFO_NOT_AVAILABLE: error = "CL_PROFILING_INFO_NOT_AVAILABLE"; break; //                              -7
        case CL_MEM_COPY_OVERLAP: error = "CL_MEM_COPY_OVERLAP"; break; //                                                      -8
        case CL_IMAGE_FORMAT_MISMATCH: error = "CL_IMAGE_FORMAT_MISMATCH"; break; //                                            -9
        case CL_IMAGE_FORMAT_NOT_SUPPORTED: error = "CL_IMAGE_FORMAT_NOT_SUPPORTED"; break; //                                  -10
        case CL_BUILD_PROGRAM_FAILURE: error = "CL_BUILD_PROGRAM_FAILURE"; break; //                                            -11
        case CL_MAP_FAILURE: error = "CL_MAP_FAILURE"; break; //                                                                -12
        case CL_MISALIGNED_SUB_BUFFER_OFFSET: error = "CL_MISALIGNED_SUB_BUFFER_OFFSET"; break; //                              -13
        case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST: error = "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST"; break; //    -14

        case CL_INVALID_VALUE: error = "CL_INVALID_VALUE"; break; //                                                            -30
        case CL_INVALID_DEVICE_TYPE: error = "CL_INVALID_DEVICE_TYPE"; break; //                                                -31
        case CL_INVALID_PLATFORM: error = "CL_INVALID_PLATFORM"; break; //                                                      -32
        case CL_INVALID_DEVICE: error = "CL_INVALID_DEVICE"; break; //                                                          -33
        case CL_INVALID_CONTEXT: error = "CL_INVALID_CONTEXT"; break; //                                                        -34
        case CL_INVALID_QUEUE_PROPERTIES: error = "CL_INVALID_QUEUE_PROPERTIES"; break; //                                      -35
        case CL_INVALID_COMMAND_QUEUE: error = "CL_INVALID_COMMAND_QUEUE"; break; //                                            -36
        case CL_INVALID_HOST_PTR: error = "CL_INVALID_HOST_PTR"; break; //                                                      -37
        case CL_INVALID_MEM_OBJECT: error = "CL_INVALID_MEM_OBJECT"; break; //                                                  -38
        case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR: error = "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR"; break; //                        -39
        case CL_INVALID_IMAGE_SIZE: error = "CL_INVALID_IMAGE_SIZE"; break; //                                                  -40
        case CL_INVALID_SAMPLER: error = "CL_INVALID_SAMPLER"; break; //                                                        -41
        case CL_INVALID_BINARY: error = "CL_INVALID_BINARY"; break; //                                                          -42
        case CL_INVALID_BUILD_OPTIONS: error = "CL_INVALID_BUILD_OPTIONS"; break; //                                            -43
        case CL_INVALID_PROGRAM: error = "CL_INVALID_PROGRAM"; break; //                                                        -44
        case CL_INVALID_PROGRAM_EXECUTABLE: error = "CL_INVALID_PROGRAM_EXECUTABLE"; break; //                                  -45
        case CL_INVALID_KERNEL_NAME: error = "CL_INVALID_KERNEL_NAME"; break; //                                                -46
        case CL_INVALID_KERNEL_DEFINITION: error = "CL_INVALID_KERNEL_DEFINITION"; break; //                                    -47
        case CL_INVALID_KERNEL: error = "CL_INVALID_KERNEL"; break; //                                                          -48
        case CL_INVALID_ARG_INDEX: error = "CL_INVALID_ARG_INDEX"; break; //                                                    -49
        case CL_INVALID_ARG_VALUE: error = "CL_INVALID_ARG_VALUE"; break; //                                                    -50
        case CL_INVALID_ARG_SIZE: error = "CL_INVALID_ARG_SIZE"; break; //                                                      -51
        case CL_INVALID_KERNEL_ARGS: error = "CL_INVALID_KERNEL_ARGS"; break; //                                                -52
        case CL_INVALID_WORK_DIMENSION: error = "CL_INVALID_WORK_DIMENSION"; break; //                                          -53
        case CL_INVALID_WORK_GROUP_SIZE: error = "CL_INVALID_WORK_GROUP_SIZE"; break; //                                        -54
        case CL_INVALID_WORK_ITEM_SIZE: error = "CL_INVALID_WORK_ITEM_SIZE"; break; //                                          -55
        case CL_INVALID_GLOBAL_OFFSET: error = "CL_INVALID_GLOBAL_OFFSET"; break; //                                            -56
        case CL_INVALID_EVENT_WAIT_LIST: error = "CL_INVALID_EVENT_WAIT_LIST"; break; //                                        -57
        case CL_INVALID_EVENT: error = "CL_INVALID_EVENT"; break; //                                                            -58
        case CL_INVALID_OPERATION: error = "CL_INVALID_OPERATION"; break; //                                                    -59
        case CL_INVALID_GL_OBJECT: error = "CL_INVALID_GL_OBJECT"; break; //                                                    -60
        case CL_INVALID_BUFFER_SIZE: error = "CL_INVALID_BUFFER_SIZE"; break; //                                                -61
        case CL_INVALID_MIP_LEVEL: error = "CL_INVALID_MIP_LEVEL"; break; //                                                    -62
        case CL_INVALID_GLOBAL_WORK_SIZE: error = "CL_INVALID_GLOBAL_WORK_SIZE"; break; //                                      -63
        case CL_INVALID_PROPERTY: error = "CL_INVALID_PROPERTY"; break; //                                                      -64
        default: error = "unknown error"; break;
        }
        return error;
    }
}

// Copied macros __PARAM_NAME_INFO_X_X from cl.hpp 
// but modified them to only contain cl_device_info.
// Future versions just need to add the macro,
// make sure that template function deviceInfoToString 
// is updated, and call the macro in OpenCL::printDeviceInfo(const cl::Device& device)
#define STRING_CLASS std::string

#define __DEVICE_INFO_1_0(F) \
    F(cl_device_info, CL_DEVICE_TYPE, cl_device_type) \
    F(cl_device_info, CL_DEVICE_NAME, STRING_CLASS) \
    F(cl_device_info, CL_DEVICE_VENDOR, STRING_CLASS) \
    F(cl_device_info, CL_DRIVER_VERSION, STRING_CLASS) \
    F(cl_device_info, CL_DEVICE_PROFILE, STRING_CLASS) \
    F(cl_device_info, CL_DEVICE_VERSION, STRING_CLASS) \
    F(cl_device_info, CL_DEVICE_EXTENSIONS, STRING_CLASS) \
    F(cl_device_info, CL_DEVICE_MAX_COMPUTE_UNITS, cl_uint) \
    F(cl_device_info, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, cl_uint) \
    F(cl_device_info, CL_DEVICE_MAX_WORK_GROUP_SIZE, ::size_t) \
    F(cl_device_info, CL_DEVICE_MAX_WORK_ITEM_SIZES, VECTOR_CLASS< ::size_t>) \
    F(cl_device_info, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, cl_uint) \
    F(cl_device_info, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, cl_uint) \
    F(cl_device_info, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, cl_uint) \
    F(cl_device_info, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, cl_uint) \
    F(cl_device_info, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, cl_uint) \
    F(cl_device_info, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, cl_uint) \
    F(cl_device_info, CL_DEVICE_MAX_CLOCK_FREQUENCY, cl_uint) \
    F(cl_device_info, CL_DEVICE_ADDRESS_BITS, cl_uint) \
    F(cl_device_info, CL_DEVICE_MAX_READ_IMAGE_ARGS, cl_uint) \
    F(cl_device_info, CL_DEVICE_MAX_WRITE_IMAGE_ARGS, cl_uint) \
    F(cl_device_info, CL_DEVICE_MAX_MEM_ALLOC_SIZE, cl_ulong) \
    F(cl_device_info, CL_DEVICE_IMAGE2D_MAX_WIDTH, ::size_t) \
    F(cl_device_info, CL_DEVICE_IMAGE2D_MAX_HEIGHT, ::size_t) \
    F(cl_device_info, CL_DEVICE_IMAGE3D_MAX_WIDTH, ::size_t) \
    F(cl_device_info, CL_DEVICE_IMAGE3D_MAX_HEIGHT, ::size_t) \
    F(cl_device_info, CL_DEVICE_IMAGE3D_MAX_DEPTH, ::size_t) \
    F(cl_device_info, CL_DEVICE_IMAGE_SUPPORT, cl_bool) \
    F(cl_device_info, CL_DEVICE_MAX_PARAMETER_SIZE, ::size_t) \
    F(cl_device_info, CL_DEVICE_MAX_SAMPLERS, cl_uint) \
    F(cl_device_info, CL_DEVICE_MEM_BASE_ADDR_ALIGN, cl_uint) \
    F(cl_device_info, CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE, cl_uint) \
    F(cl_device_info, CL_DEVICE_SINGLE_FP_CONFIG, cl_device_fp_config) \
    F(cl_device_info, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, cl_device_mem_cache_type) \
    F(cl_device_info, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, cl_uint)\
    F(cl_device_info, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, cl_ulong) \
    F(cl_device_info, CL_DEVICE_GLOBAL_MEM_SIZE, cl_ulong) \
    F(cl_device_info, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, cl_ulong) \
    F(cl_device_info, CL_DEVICE_MAX_CONSTANT_ARGS, cl_uint) \
    F(cl_device_info, CL_DEVICE_LOCAL_MEM_TYPE, cl_device_local_mem_type) \
    F(cl_device_info, CL_DEVICE_LOCAL_MEM_SIZE, cl_ulong) \
    F(cl_device_info, CL_DEVICE_ERROR_CORRECTION_SUPPORT, cl_bool) \
    F(cl_device_info, CL_DEVICE_PROFILING_TIMER_RESOLUTION, ::size_t) \
    F(cl_device_info, CL_DEVICE_ENDIAN_LITTLE, cl_bool) \
    F(cl_device_info, CL_DEVICE_AVAILABLE, cl_bool) \
    F(cl_device_info, CL_DEVICE_COMPILER_AVAILABLE, cl_bool) \
    F(cl_device_info, CL_DEVICE_EXECUTION_CAPABILITIES, cl_device_exec_capabilities) \
    F(cl_device_info, CL_DEVICE_QUEUE_PROPERTIES, cl_command_queue_properties) \
    F(cl_device_info, CL_DEVICE_PLATFORM, cl_platform_id) 

#if defined(CL_VERSION_1_1)
#define __DEVICE_INFO_1_1(F) \
    F(cl_device_info, CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF, cl_uint) \
    F(cl_device_info, CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR, cl_uint) \
    F(cl_device_info, CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT, cl_uint) \
    F(cl_device_info, CL_DEVICE_NATIVE_VECTOR_WIDTH_INT, cl_uint) \
    F(cl_device_info, CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG, cl_uint) \
    F(cl_device_info, CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT, cl_uint) \
    F(cl_device_info, CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE, cl_uint) \
    F(cl_device_info, CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF, cl_uint) \
    F(cl_device_info, CL_DEVICE_DOUBLE_FP_CONFIG, cl_device_fp_config) \
    F(cl_device_info, CL_DEVICE_HALF_FP_CONFIG, cl_device_fp_config) \
    F(cl_device_info, CL_DEVICE_HOST_UNIFIED_MEMORY, cl_bool) \
    F(cl_device_info, CL_DEVICE_OPENCL_C_VERSION, STRING_CLASS) 
#endif
#if defined(CL_VERSION_1_2)
#define __DEVICE_INFO_1_2(F) \
    F(cl_device_info, CL_DEVICE_PARENT_DEVICE, cl_device_id) \
    F(cl_device_info, CL_DEVICE_PARTITION_PROPERTIES, VECTOR_CLASS<cl_device_partition_property>) \
    F(cl_device_info, CL_DEVICE_PARTITION_TYPE, VECTOR_CLASS<cl_device_partition_property>)  \
    F(cl_device_info, CL_DEVICE_REFERENCE_COUNT, cl_uint) \
    F(cl_device_info, CL_DEVICE_PREFERRED_INTEROP_USER_SYNC, cl_bool) \
    F(cl_device_info, CL_DEVICE_PARTITION_AFFINITY_DOMAIN, cl_device_affinity_domain) \
    F(cl_device_info, CL_DEVICE_BUILT_IN_KERNELS, STRING_CLASS)
#endif // #if defined(CL_VERSION_1_2)

 /** \brief Return string representation of  device info and corresponding value
  * @note Added 
  * @param cl_device_info info OpenCL device info identifier.
  * @param const T & value 
  * @param bool is_cl_bool Necessary since cl_bool is a cl_uint
  * @return string representation of device info and corresponding value
  */
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
template<>
std::string deviceInfoToString(cl_device_info, const std::string& value, bool) {
    return value;
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

#define __CL_PRINT_DEVICE_INFO(token, param_name, T) \
    if(std::string(#T).compare("cl_bool") == 0) { \
        LogInfoS("OpenCL", #param_name << ": " << deviceInfoToString(param_name, device.getInfo<param_name>(), true)) \
    } else {                                        \
        LogInfoS("OpenCL", #param_name << ": " << deviceInfoToString(param_name, device.getInfo<param_name>(), false)) \
    } 
        




namespace inviwo {

    OpenCL::OpenCL() {
        initialize(true);
    }

    void OpenCL::initialize(bool glSharing) {
        cl_int error = CL_SUCCESS;
        try {
            std::vector<cl::Platform> platforms;
            cl::Platform::get(&platforms);
            if (platforms.size() == 0) {
                std::cout << "No OpenCL platforms found" << std::endl;
                return;
            }

            cl::Platform platform;
            getBestGPUDevice(gpuDevice_, platform);
            std::vector<cl_context_properties> properties;
            if(glSharing) 
                properties = getGLSharingContextProperties();
            cl_context_properties platformProperties[] = {CL_CONTEXT_PLATFORM, (cl_context_properties)platform(), 0};
            properties.insert(properties.end(), platformProperties, platformProperties+ sizeof(platformProperties)/sizeof(cl_context_properties));
            try
            {
                gpuContext_ = cl::Context(gpuDevice_, &properties[0]);
            }
            catch (cl::Error&)
            {
                LogErrorS("OpenCL", "ERROR: Unable to create OpenCL context. Trying to create without openGL sharing... ");
                properties.clear();
                properties.insert(properties.end(), platformProperties, platformProperties+ sizeof(platformProperties)/sizeof(cl_context_properties));
                gpuContext_ = cl::Context(gpuDevice_, &properties[0]);
                LogErrorS("OpenCL", "Succeeded creating OpenCL without OpenGL sharing. ");

            }

            printDeviceInfo(gpuDevice_);

            cl_command_queue_properties queueProperties = 0;
            cl_command_queue_properties supportedQueueProperties;
            gpuDevice_.getInfo(CL_DEVICE_QUEUE_PROPERTIES, &supportedQueueProperties);
            #if IVW_PROFILING
                if ( supportedQueueProperties & CL_QUEUE_PROFILING_ENABLE)
                    queueProperties |= CL_QUEUE_PROFILING_ENABLE;
            #endif
            synchronosGPUQueue_ = cl::CommandQueue(gpuContext_, gpuDevice_, queueProperties);
            if ( supportedQueueProperties & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE)
                queueProperties |= CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
            asyncGPUQueue_ = cl::CommandQueue(gpuContext_, gpuDevice_, queueProperties);

        } catch (cl::Error& err) {
            
            LogErrorS("OpenCL", "ERROR: " << err.what() << "(" << err.err() << "), " << cl::errorCodeToString(err.err()) << std::endl);
            
        }
    }
    bool OpenCL::isValidImageFormat(const cl::Context& context, const cl::ImageFormat& format) {
        std::vector<cl::ImageFormat> formats;
        context.getSupportedImageFormats(CL_MEM_READ_WRITE, CL_MEM_OBJECT_IMAGE2D, &formats);
        for(::size_t i = 0; i < formats.size(); ++i) {
            if ( formats[i].image_channel_order == format.image_channel_order && formats[i].image_channel_data_type == format.image_channel_data_type) {
                return true;
            }
        }
        return false;
    }
    bool OpenCL::isValidVolumeFormat(const cl::Context& context, const cl::ImageFormat& format) {
        std::vector<cl::ImageFormat> formats;
        context.getSupportedImageFormats(CL_MEM_READ_WRITE, CL_MEM_OBJECT_IMAGE3D, &formats);
        for(::size_t i = 0; i < formats.size(); ++i) {
            if ( formats[i].image_channel_order == format.image_channel_order && formats[i].image_channel_data_type == format.image_channel_data_type) {
                return true;
            }
        }
        return false;
    }

    void OpenCL::printDeviceInfo(const cl::Device& device) {
        
        try
        {
            // Macros will print print supported device info
            __DEVICE_INFO_1_0(__CL_PRINT_DEVICE_INFO)
            #if defined(CL_VERSION_1_1)
                __DEVICE_INFO_1_1(__CL_PRINT_DEVICE_INFO) 
            #endif 
            #if defined(CL_VERSION_1_2)
                __DEVICE_INFO_1_2(__CL_PRINT_DEVICE_INFO) 
            #endif 
            #if defined(USE_CL_DEVICE_FISSION)
                __PARAM_NAME_DEVICE_FISSION(__CL_PRINT_DEVICE_INFO) 
            #endif
        }
        catch (cl::Error&)
        {
            LogInfoS("OpenCL", "Error while retrieving device info.");
        }        
    }
    /*! \brief Get the device that has most compute units.
     *  
     *  @param bestDevice Set to found device, if found.
     *  @param onPlatform Set to platform that device exist on, if found.
     *  \return True if any device found, false otherwise. 
     */
    bool OpenCL::getBestGPUDevice(cl::Device& bestDevice, cl::Platform& onPlatform) {
        bool foundDevice = false;
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);
        cl_uint maxComputeUnits = 0;
        // Search for best device
        for(::size_t i = 0; i < platforms.size(); ++i) {
            std::vector<cl::Device> devices;
            platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &devices);
            for(::size_t j = 0; j < devices.size(); ++j) {
                cl_uint tmpMaxComputeUnits;
                devices[j].getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &tmpMaxComputeUnits);
                if( maxComputeUnits < tmpMaxComputeUnits ) {
                    bestDevice = devices[j];
                    onPlatform = platforms[i];
                    maxComputeUnits = tmpMaxComputeUnits;
                    foundDevice = true;
                }
            }
        }
        return foundDevice;
    }
    void OpenCL::printBuildError(const std::vector<cl::Device>& devices, const cl::Program& program, const std::string& filename) {
        for(::size_t i = 0; i < devices.size(); ++i) {
             cl_build_status status = program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(devices[i]);
             // Houston, we have a problem
             if(status == CL_BUILD_ERROR) {
                 std::string buildLog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[i]);
                LogErrorS("OpenCL", filename << " build error:" << std::endl << buildLog);
             }
        }
    }
    void OpenCL::printBuildError(const cl::Device& device, const cl::Program& program, const std::string& filename) {
        printBuildError(std::vector<cl::Device>(1, device), program, filename);
    }

    std::vector<cl_context_properties> OpenCL::getGLSharingContextProperties() {
#if WIN32
        cl_context_properties props[] = {
            CL_GL_CONTEXT_KHR, (cl_context_properties) wglGetCurrentContext(), 
            CL_WGL_HDC_KHR, (cl_context_properties) wglGetCurrentDC()};
#elif __APPLE__
        CGLContextObj glContext = CGLGetCurrentContext();
        CGLShareGroupObj shareGroup = CGLGetShareGroup(glContext);
        cl_context_properties props[] = {
            CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)shareGroup};
#else // LINUX
        cl_context_properties props[] = {
            CL_GL_CONTEXT_KHR, (cl_context_properties) glXGetCurrentContext(),
            CL_GLX_DISPLAY_KHR, (cl_context_properties) glXGetCurrentDisplay()};
#endif
        return std::vector<cl_context_properties>(props, props + sizeof(props)/sizeof(cl_context_properties));
    }

    cl::Program OpenCL::buildProgram(const std::string& fileName, const std::string& defines, const cl::CommandQueue& queue) 
    {
        cl::Context context = queue.getInfo<CL_QUEUE_CONTEXT>();
        cl::Device device = queue.getInfo<CL_QUEUE_DEVICE>();
        // build the program from the source in the file
        std::ifstream file(fileName.c_str());
        std::string prog(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
        cl::Program::Sources source( 1, std::make_pair(prog.c_str(), prog.length()+1));
        cl::Program program(context, source);
        try {
            program.build(std::vector<cl::Device>(1, OpenCL::getInstance()->getDevice()));
            std::string buildLog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
            // Output log if it contains any info
            if(buildLog.size() > 1)
                LogInfoS("OpenCL", fileName << " build info:" << std::endl << buildLog);
        } catch (cl::Error& e) {
            OpenCL::printBuildError(std::vector<cl::Device>(1, OpenCL::getInstance()->getDevice()), program);
            throw e;
        }
        return program;
    }

    cl::Program OpenCL::buildProgram( const std::string& fileName, const std::string& defines /*= ""*/ )
    {
        return OpenCL::buildProgram(fileName, defines, OpenCL::getInstance()->getQueue());
    }


}
