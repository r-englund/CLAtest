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
        
        std::ostringstream stream;
        stream << std::boolalpha; // Allow bool to print true/false
        try
        {
            stream << "OpenCL device info: " << std::endl;
            stream << "Name: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
            stream << "Vendor: " << device.getInfo<CL_DEVICE_VENDOR>() << std::endl;
            stream << "Profile: " << device.getInfo<CL_DEVICE_PROFILE>() << std::endl;
            stream << "Device version: " << device.getInfo<CL_DEVICE_VERSION>() << std::endl;
            stream << "Driver version: " << device.getInfo<CL_DRIVER_VERSION>() << std::endl;
            stream << "OpenCL C version: " << device.getInfo<CL_DEVICE_OPENCL_C_VERSION>() << std::endl;
            stream << "Extensions: " << device.getInfo<CL_DEVICE_EXTENSIONS>() << std::endl;
            stream << "Max compute units: " << device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << std::endl;
            cl::size_t<3> maxWorkItemSizes;
            device.getInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES, &maxWorkItemSizes);
            stream << "Max work item sizes: (";
            for(int i = 0; i < 2; ++i) {
                stream << maxWorkItemSizes[i] << ", ";
            }
            stream << maxWorkItemSizes[2] << ")" << std::endl;

            stream << "Preffered vector width (char): " << device.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR>() << std::endl;
            stream << "Preffered vector width (short): " << device.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT>() << std::endl;
            stream << "Preffered vector width (int): " << device.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT>() << std::endl;
            stream << "Preffered vector width (long): " << device.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG>() << std::endl;
            stream << "Preffered vector width (float): " << device.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT>() << std::endl;
            stream << "Preffered vector width (double): " << device.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE>() << std::endl;
            stream << "Preffered vector width (half): " << device.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF>() << std::endl;
            stream << "Preffered native vector width (char): " << device.getInfo<CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR>() << std::endl;
            stream << "Preffered native vector width (short): " << device.getInfo<CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT>() << std::endl;
            stream << "Preffered native vector width (int): " << device.getInfo<CL_DEVICE_NATIVE_VECTOR_WIDTH_INT>() << std::endl;
            stream << "Preffered native vector width (long): " << device.getInfo<CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG>() << std::endl;
            stream << "Preffered native vector width (float): " << device.getInfo<CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT>() << std::endl;
            stream << "Preffered native vector width (double): " << device.getInfo<CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE>() << std::endl;
            stream << "Preffered native vector width (half): " << device.getInfo<CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF>() << std::endl;
            stream << "Max clock frequency: " << device.getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>() << std::endl;
            stream << "Address bits: " << device.getInfo<CL_DEVICE_ADDRESS_BITS>() << std::endl;
            stream << "Max alloc size: " << device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>() << std::endl;
            stream << "Supports image: " << static_cast<bool>(device.getInfo<CL_DEVICE_IMAGE_SUPPORT>() != 0) << std::endl;
            stream << "Max number of simultaneous image objects that can be read by a kernel: " << device.getInfo<CL_DEVICE_MAX_READ_IMAGE_ARGS>() << std::endl;
            stream << "Max number of simultaneous image objects that can be written by a kernel: " << device.getInfo<CL_DEVICE_MAX_WRITE_IMAGE_ARGS>() << std::endl;
           
            stream << "Max 2D image (width, height): (" << device.getInfo<CL_DEVICE_IMAGE2D_MAX_WIDTH>() << ", " << device.getInfo<CL_DEVICE_IMAGE2D_MAX_HEIGHT>() << ")" << std::endl;
            stream << "Max 3D image (width, height, depth): (" << device.getInfo<CL_DEVICE_IMAGE3D_MAX_WIDTH>() << ", " << device.getInfo<CL_DEVICE_IMAGE3D_MAX_HEIGHT>() << ", " << device.getInfo<CL_DEVICE_IMAGE3D_MAX_DEPTH>() << ")" << std::endl;
            #if defined(CL_VERSION_1_2) 
            stream << "Max 1D image size: " << device.getInfo<CL_DEVICE_IMAGE_MAX_BUFFER_SIZE>() << std::endl;
            stream << "Max number of images in a 1D or 2D image array: " << device.getInfo<CL_DEVICE_IMAGE_MAX_ARRAY_SIZE>() << std::endl;
            #endif
            stream << "Max samplers: " << device.getInfo<CL_DEVICE_MAX_SAMPLERS>() << std::endl;
            stream << "Max kernel arguments size (bytes): " << device.getInfo<CL_DEVICE_MAX_PARAMETER_SIZE>() << std::endl;
            stream << "Memory base address align: " << device.getInfo<CL_DEVICE_MEM_BASE_ADDR_ALIGN>() << std::endl;
            // Floating point support
            cl_device_fp_config fpConfig;
            device.getInfo(CL_DEVICE_SINGLE_FP_CONFIG, &fpConfig);
            stream << "Supports float denorms: " << static_cast<bool>((fpConfig & CL_FP_DENORM) != 0) << std::endl;
            stream << "Supports float INF and quiet NaN: " << static_cast<bool>((fpConfig & CL_FP_INF_NAN) != 0) << std::endl;
            stream << "Supports float round to nearest: " << static_cast<bool>((fpConfig & CL_FP_ROUND_TO_NEAREST) != 0) << std::endl;
            stream << "Supports float round to zero: " << static_cast<bool>((fpConfig & CL_FP_ROUND_TO_ZERO) != 0) << std::endl;
            stream << "Supports float round to inf: " << static_cast<bool>((fpConfig & CL_FP_ROUND_TO_INF) != 0) << std::endl;
            stream << "Supports float IEEE754-2008 fused multiply-add: " << static_cast<bool>((fpConfig & CL_FP_FMA) != 0) << std::endl;
            //stream << "Supports float IEEE754 correct divide and sqrt round: " << static_cast<bool>(fpConfig & CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT!= 0) << std::endl;
            stream << "Software float implementation: " << static_cast<bool>((fpConfig & CL_FP_SOFT_FLOAT) != 0) << std::endl;
            
            device.getInfo(CL_DEVICE_DOUBLE_FP_CONFIG, &fpConfig);
            stream << "Supports double denorms: " << static_cast<bool>((fpConfig & CL_FP_DENORM) != 0) << std::endl;
            stream << "Supports double INF and quiet NaN: " << static_cast<bool>((fpConfig & CL_FP_INF_NAN) != 0) << std::endl;
            stream << "Supports double round to nearest: " << static_cast<bool>((fpConfig & CL_FP_ROUND_TO_NEAREST) != 0) << std::endl;
            stream << "Supports double round to zero: " << static_cast<bool>((fpConfig & CL_FP_ROUND_TO_ZERO) != 0) << std::endl;
            stream << "Supports double round to inf: " << static_cast<bool>((fpConfig & CL_FP_ROUND_TO_INF) != 0) << std::endl;
            stream << "Supports double IEEE754-2008 fused multiply-add: " << static_cast<bool>((fpConfig & CL_FP_FMA) != 0) << std::endl;
            //stream << "Supports double IEEE754 correct divide and sqrt round: " << static_cast<bool>(fpConfig & CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT != 0) << std::endl;
            stream << "Software double implementation: " << static_cast<bool>((fpConfig & CL_FP_SOFT_FLOAT) != 0) << std::endl;
            // Cache
            cl_device_mem_cache_type cacheType;
            device.getInfo(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, &cacheType);
            stream << "Supports read only cache: " << static_cast<bool>((cacheType & CL_READ_ONLY_CACHE) != 0) << std::endl;
            stream << "Supports read-write cache: " << static_cast<bool>((cacheType & CL_READ_WRITE_CACHE) != 0) << std::endl;
            stream << "Size of global memory cache line in bytes: " << device.getInfo<CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE>() << std::endl;
            stream << "Size of global memory cache in bytes: " << device.getInfo<CL_DEVICE_GLOBAL_MEM_CACHE_SIZE>() << std::endl;
            stream << "Size of global device memory in bytes: " << device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>() << std::endl;
            stream << "Max size in bytes of a constant buffer allocation: " << device.getInfo<CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE>() << std::endl;
            
            stream << "Max number of arguments declared with the __constant qualifier in a kernel: " << device.getInfo<CL_DEVICE_MAX_CONSTANT_ARGS>() << std::endl;
            
            // Local memory
            cl_device_local_mem_type lmemType;
            device.getInfo(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, &lmemType);
            stream << "Supports dedicated local memory: " << static_cast<bool>((lmemType & CL_LOCAL) != 0) << std::endl;
            stream << "Supports local memory: " << (lmemType != CL_NONE) << std::endl;
            stream << "Local memory size in bytes: " << device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>() << std::endl;


            stream << "Supports error correction for memory: " << static_cast<bool>(device.getInfo<CL_DEVICE_ERROR_CORRECTION_SUPPORT>() != 0) << std::endl;
            stream << "Unified memory subsystem: " << static_cast<bool>(device.getInfo<CL_DEVICE_HOST_UNIFIED_MEMORY>() != 0) << std::endl;
            stream << "Profiling timer resolution (nanoseconds): " << device.getInfo<CL_DEVICE_PROFILING_TIMER_RESOLUTION>() << std::endl;
            stream << "Little endian: " << static_cast<bool>(device.getInfo<CL_DEVICE_ENDIAN_LITTLE>() != 0) << std::endl;
            stream << "Device available: " << static_cast<bool>(device.getInfo<CL_DEVICE_AVAILABLE>() != 0) << std::endl;
            stream << "Compiler available: " << static_cast<bool>(device.getInfo<CL_DEVICE_COMPILER_AVAILABLE>() != 0) << std::endl;
            #if defined(CL_VERSION_1_2) 
            stream << "Linker available: " << static_cast<bool>(device.getInfo<CL_DEVICE_LINKER_AVAILABLE>() != 0) << std::endl;
            #endif
            // Device execution
            cl_device_exec_capabilities execCap;
            device.getInfo(CL_DEVICE_EXECUTION_CAPABILITIES, &execCap);
            stream << "Supports kernel execution: " << static_cast<bool>((execCap & CL_EXEC_KERNEL) != 0) << std::endl;
            stream << "Supports native kernel execution: " << static_cast<bool>((execCap & CL_EXEC_NATIVE_KERNEL) != 0) << std::endl;
           
            
            // Command queue
            cl_command_queue_properties commandQueueProperties;
            device.getInfo(CL_DEVICE_QUEUE_PROPERTIES, &commandQueueProperties);
            stream << "Supports out of order execution: " << static_cast<bool>((commandQueueProperties & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE) != 0) << std::endl;
            stream << "Supports profiling: " << static_cast<bool>((commandQueueProperties & CL_QUEUE_PROFILING_ENABLE) != 0) << std::endl;
            
            #if defined(CL_VERSION_1_2) 
            stream << "Built-in kernels: " << device.getInfo<CL_DEVICE_BUILT_IN_KERNELS>() << std::endl;
            
            stream << "Max printf buffer size: " << device.getInfo<CL_DEVICE_PRINTF_BUFFER_SIZE>() << std::endl;
            stream << "Preferred user GL/DirectX sync responsibility: " << static_cast<bool>(device.getInfo<CL_DEVICE_PREFERRED_INTEROP_USER_SYNC>() != 0) << std::endl;
            #endif
        }
        catch (cl::Error&)
        {
            LogInfoS("OpenCL", "Error while retrieving device info.");
        }
        LogInfoS("OpenCL", stream.str());
        
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
