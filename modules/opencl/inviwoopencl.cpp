#include "modules/opencl/inviwoopencl.h"
#include "inviwo/core/util/logdistributor.h"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include "modules/opencl/cl.hpp"
#include "modules/opencl/glmcl.h"

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
    std::string loadFile(std::string fileName) {
        FILE* file;
        char* fileContent = NULL;
        long len;

        if (fileName.length() > 0) {
            OPEN_FILE(file, fileName.c_str(), "rb");
            if (file != NULL) {
                fseek(file, 0L, SEEK_END);
                len = ftell(file);
                fseek(file, 0L, SEEK_SET);
                fileContent = (char*)malloc(sizeof(char)*(len));
                if(fileContent != NULL){
                    fread(fileContent, sizeof(char), len, file);
                    fileContent[len] = '\0';
                }
                fclose(file);
            }
        }
        return std::string(fileContent);
    }

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
                LogErrorS("OpenCL", std::string("Succeeded creating OpenCL without OpenGL sharing. ") << std::string(" sf"));

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
            //printDeviceInfo(devices[0]);

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
        std::string name, vendor, profile;
        std::string deviceVersion, driverVersion;
        std::string clVersion, extensions;
        cl::size_t<3> maxWorkItemSizes;
        cl_uint maxWorkItemDimension;
                cl_uint uintProperty;
        cl_ulong ulongProperty;
        device.getInfo(CL_DEVICE_NAME, &name);
        device.getInfo(CL_DEVICE_VENDOR, &vendor);
        device.getInfo(CL_DEVICE_PROFILE, &profile);
        device.getInfo(CL_DEVICE_VERSION, &deviceVersion);
        device.getInfo(CL_DRIVER_VERSION, &driverVersion);
        device.getInfo(CL_DEVICE_OPENCL_C_VERSION, &clVersion);
        device.getInfo(CL_DEVICE_EXTENSIONS, &extensions);
        device.getInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES, &maxWorkItemSizes);
        std::ostringstream stream;
        stream << "OpenCL device info: " << std::endl;
        stream << "Name: " << name << std::endl;
        stream << "Vendor: " << vendor << std::endl;
        stream << "Profile: " << profile << std::endl;
        stream << "Device version: " << deviceVersion << std::endl;
        stream << "Driver version: " << driverVersion << std::endl;
        stream << "OpenCL C version: " << clVersion << std::endl;
        stream << "Extensions: " << extensions << std::endl;
        stream << "Max work item sizes: ";
        for(int i = 0; i < 2; ++i) {
            stream << maxWorkItemSizes[i] << ", ";
        }
        stream << maxWorkItemSizes[2] << std::endl;

        device.getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, &uintProperty);
        stream << "Preffered vector width (char): " << uintProperty << std::endl;
        device.getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, &uintProperty);
        stream << "Preffered vector width (short): " << uintProperty << std::endl;
        device.getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, &uintProperty);
        stream << "Preffered vector width (int): " << uintProperty << std::endl;
        device.getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, &uintProperty);
        stream << "Preffered vector width (long): " << uintProperty << std::endl;
        device.getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, &uintProperty);
        stream << "Preffered vector width (float): " << uintProperty << std::endl;
        device.getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, &uintProperty);
        stream << "Preffered vector width (double): " << uintProperty << std::endl;
        device.getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF, &uintProperty);
        stream << "Preffered vector width (half): " << uintProperty << std::endl;
        device.getInfo(CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR, &uintProperty);
        stream << "Preffered native vector width (char): " << uintProperty << std::endl;
        device.getInfo(CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT, &uintProperty);
        stream << "Preffered native vector width (short): " << uintProperty << std::endl;
        device.getInfo(CL_DEVICE_NATIVE_VECTOR_WIDTH_INT, &uintProperty);
        stream << "Preffered native vector width (int): " << uintProperty << std::endl;
        device.getInfo(CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG, &uintProperty);
        stream << "Preffered native vector width (long): " << uintProperty << std::endl;
        device.getInfo(CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT, &uintProperty);
        stream << "Preffered native vector width (float): " << uintProperty << std::endl;
        device.getInfo(CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE, &uintProperty);
        stream << "Preffered native vector width (double): " << uintProperty << std::endl;
        device.getInfo(CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF, &uintProperty);
        stream << "Preffered native vector width (half): " << uintProperty << std::endl;
        device.getInfo(CL_DEVICE_MAX_CLOCK_FREQUENCY, &uintProperty);
        stream << "Max clock frequency: " << uintProperty << std::endl;
        device.getInfo(CL_DEVICE_ADDRESS_BITS, &uintProperty);
        stream << "Address bits: " << uintProperty << std::endl;
        device.getInfo(CL_DEVICE_MAX_MEM_ALLOC_SIZE, &ulongProperty);
        stream << "Max alloc size: " << ulongProperty << std::endl;
        
        
        cl_command_queue_properties commandQueueProperties;
        device.getInfo(CL_DEVICE_QUEUE_PROPERTIES, &commandQueueProperties);
        std::string supported = "No";
        if ( commandQueueProperties & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE)
            supported = "Yes";
        stream << "Supports out of order execution: " << supported << std::endl;
        supported = "No";
        if ( commandQueueProperties & CL_QUEUE_PROFILING_ENABLE)
            supported = "Yes";
        stream << "Supports profiling: " << supported << std::endl;
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


}
