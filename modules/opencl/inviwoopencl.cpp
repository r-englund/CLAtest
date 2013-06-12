#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/cl.hpp>
#include <modules/opencl/glmcl.h>
#include <modules/opencl/openclinfo.h> 
#include <inviwo/core/util/logdistributor.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <fstream>
#if !(WIN32 || __APPLE__) // LINUX
#include <GL/glx.h> // glXCurrentContext()
#endif

#ifdef WIN32
#define OPEN_FILE(a,b,c) fopen_s(&a, b, c);
#else
#define OPEN_FILE(a,b,c) a = fopen(b, c);
#endif


namespace cl {

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
            LogError("ERROR: Unable to create OpenCL context. Trying to create without openGL sharing... ");
            properties.clear();
            properties.insert(properties.end(), platformProperties, platformProperties+ sizeof(platformProperties)/sizeof(cl_context_properties));
            gpuContext_ = cl::Context(gpuDevice_, &properties[0]);
            LogError("Succeeded creating OpenCL without OpenGL sharing. ");

        }

        //OpenCLInfo::printDeviceInfo(gpuDevice_);

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
            
        LogError("ERROR: Failed to initialize OpenCL. " << err.what() << "(" << err.err() << "), " << errorCodeToString(err.err()) << std::endl);
            
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
            LogErrorCustom("OpenCL", filename << " build error:" << std::endl << buildLog);
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
            LogInfoCustom("OpenCL", fileName << " build info:" << std::endl << buildLog);
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


void LogOpenCLError(cl_int err, const char* message) {
    if (err != CL_SUCCESS) {
        std::ostringstream errorMessage;
        errorMessage << "OpenCL Error " << err << ": " << errorCodeToString(err) << std::endl << message;
        LogErrorCustom("OpenCL", errorMessage.str());
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
#if defined(CL_VERSION_1_1)
    case CL_MISALIGNED_SUB_BUFFER_OFFSET: error = "CL_MISALIGNED_SUB_BUFFER_OFFSET"; break; //                              -13
    case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST: error = "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST"; break; //    -14
#endif
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
#if defined(CL_VERSION_1_1)
    case CL_INVALID_PROPERTY: error = "CL_INVALID_PROPERTY"; break; //                                                      -64
#endif
    default: error = "unknown OpenCL error"; break;
    }
    return error;
}

cl::ImageFormat dataFormatToCLImageFormat( inviwo::DataFormatId format )
{   
    // Difference between SNORM/UNORM and SIGNED/UNSIGNED INT
    // SNORM/UNORM:
    // Each channel component is a normalized integer value
    // SIGNED/UNSIGNED:
    // Each channel component is an unnormalized integer value

    // From the specification (note that CL_Rx, CL_RGx and CL_RGBx are optional):
    // If the image channel order is CL_A, CL_INTENSITY, CL_Rx, CL_RA, CL_RGx, 
    // CL_RGBx, CL_ARGB, CL_BGRA, or CL_RGBA, the border color is (0.0f, 0.0f, 
    // 0.0f, 0.0f).
    // If the image channel order is CL_R, CL_RG, CL_RGB, or CL_LUMINANCE, the border 
    // color is (0.0f, 0.0f, 0.0f, 1.0f).

    // CL_INTENSITY maps single value I to (I, I, I, I)

    cl::ImageFormat clFormat;
    switch (format) {
    case NOT_SPECIALIZED:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case FLOAT16:
        clFormat = cl::ImageFormat(CL_INTENSITY, CL_HALF_FLOAT);  break;
    case FLOAT32:
        clFormat = cl::ImageFormat(CL_INTENSITY, CL_FLOAT); break;
    case FLOAT64:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case INT8:
        clFormat = cl::ImageFormat(CL_INTENSITY, CL_SNORM_INT8); break;
    case INT12:
        clFormat = cl::ImageFormat(CL_INTENSITY, CL_SNORM_INT16); break;
    case INT16:
        clFormat = cl::ImageFormat(CL_INTENSITY, CL_SNORM_INT16); break;
    case INT32:
        clFormat = cl::ImageFormat(CL_R, CL_SIGNED_INT32); break;
    case INT64:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case UINT8:
        clFormat = cl::ImageFormat(CL_INTENSITY, CL_UNORM_INT8);  break;
    case UINT12:
        clFormat = cl::ImageFormat(CL_INTENSITY, CL_UNORM_INT16); break;
    case UINT16:
        clFormat = cl::ImageFormat(CL_INTENSITY, CL_UNORM_INT16); break;
    case UINT32:
        clFormat = cl::ImageFormat(CL_R, CL_UNSIGNED_INT32); break;
    case UINT64:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case Vec2FLOAT16:
        clFormat = cl::ImageFormat(CL_RG, CL_HALF_FLOAT);  break;
    case Vec2FLOAT32:
        clFormat = cl::ImageFormat(CL_RG, CL_FLOAT);  break;
    case Vec2FLOAT64:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case Vec2INT8:
        clFormat = cl::ImageFormat(CL_RG, CL_SNORM_INT8);  break;
    case Vec2INT12:
        clFormat = cl::ImageFormat(CL_RG, CL_SNORM_INT16);  break;
    case Vec2INT16:
        clFormat = cl::ImageFormat(CL_RG, CL_SNORM_INT16);  break;
    case Vec2INT32:
        clFormat = cl::ImageFormat(CL_RG, CL_SIGNED_INT32);  break;
    case Vec2INT64:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case Vec2UINT8:
        clFormat = cl::ImageFormat(CL_RG, CL_UNORM_INT8);  break;
    case Vec2UINT12:
        clFormat = cl::ImageFormat(CL_RG, CL_UNORM_INT16);  break;
    case Vec2UINT16:
        clFormat = cl::ImageFormat(CL_RG, CL_UNORM_INT16);  break;
    case Vec2UINT32:
        clFormat = cl::ImageFormat(CL_RG, CL_UNSIGNED_INT32);  break;
    case Vec2UINT64:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case Vec3FLOAT16:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case Vec3FLOAT32:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case Vec3FLOAT64:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case Vec3INT8:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case Vec3INT12:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case Vec3INT16:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case Vec3INT32:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case Vec3INT64:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case Vec3UINT8:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case Vec3UINT12:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case Vec3UINT16:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case Vec3UINT32:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case Vec3UINT64:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case Vec4FLOAT16:
        clFormat = cl::ImageFormat(CL_RGBA, CL_HALF_FLOAT);  break;
    case Vec4FLOAT32:
        clFormat = cl::ImageFormat(CL_RGBA, CL_FLOAT);  break;
    case Vec4FLOAT64:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case Vec4INT8:
        clFormat = cl::ImageFormat(CL_RGBA, CL_SNORM_INT8);  break;
    case Vec4INT12:
        clFormat = cl::ImageFormat(CL_RGBA, CL_SNORM_INT16);  break;
    case Vec4INT16:
        clFormat = cl::ImageFormat(CL_RGBA, CL_SNORM_INT16);  break;
    case Vec4INT32:
        clFormat = cl::ImageFormat(CL_RGBA, CL_SIGNED_INT32);  break;
    case Vec4INT64:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    case Vec4UINT8:
        clFormat = cl::ImageFormat(CL_RGBA, CL_UNORM_INT8);  break; // TODO: Find out why CL_UNORM_INT8 does not work
    case Vec4UINT12:
        clFormat = cl::ImageFormat(CL_RGBA, CL_UNORM_INT16);  break;
    case Vec4UINT16:
        clFormat = cl::ImageFormat(CL_RGBA, CL_UNORM_INT16);  break;
    case Vec4UINT32:
        clFormat = cl::ImageFormat(CL_RGBA, CL_UNSIGNED_INT32);  break;
    case Vec4UINT64:
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Invalid conversion"); break;
    default:
        // Should not be able to reach here
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "Format not implmented yet"); break;
    }
#ifdef DEBUG
    if (!inviwo::OpenCL::isValidImageFormat(inviwo::OpenCL::getInstance()->getContext(), clFormat)) {
        LogErrorCustom("cl::ImageFormat typeToImageFormat", "OpenCL device does not support format");
    };
#endif 

    return clFormat;
}
}
