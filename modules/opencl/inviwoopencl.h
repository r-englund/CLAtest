/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#ifndef IVW_INVIWOOPENCL_H
#define IVW_INVIWOOPENCL_H
#include <vector>
#include <iostream>
#include <glm/glm.hpp>

#include <inviwo/core/util/formats.h>

#include <modules/opencl/cl.hpp>
#include <modules/opencl/glmcl.h>
#include <modules/opencl/openclmoduledefine.h>

using glm::svec2;
using glm::svec3;

using cl::CommandQueue;
using cl::Context;
using cl::Device;
using cl::Platform;
using cl::Program;
using cl::ImageFormat;


namespace inviwo {

/** \class OpenCL 
*
* Singleton class that manages OpenCL context and queues. 
*
*/
class IVW_MODULE_OPENCL_API OpenCL {
public:


    static OpenCL* instance() {
        static OpenCL instance;// Guaranteed to be destroyed. Instantiated on first use.
        return &instance;
    }
    /**
     * Get queue that can perform tasks in serial (no need to explicitly manage events).
     * This should be the first choice of use. Expert users may want to use the 
     * asynchronous queue.
     */
    const cl::CommandQueue& getQueue() const { return synchronosGPUQueue_; }
    /**
     * Get queue that can perform tasks in parallel (need to explicitly manage events).
     * This is for expert users. be the first choice of use. Expert users may want to use the 
     * asynchronous queue.
     */
    const cl::CommandQueue& getAsyncQueue() const { return asyncGPUQueue_; }

    /**
     * Get default OpenCL context.
     */
    const cl::Context& getContext() const { return gpuContext_; }

    /**
     * Get default OpenCL device.
     */
    const cl::Device& getDevice() const { return gpuDevice_; }

    static cl::Program buildProgram(const std::string& fileName, const std::string& defines = "");
    static cl::Program buildProgram(const std::string& fileName, const std::string& defines, const cl::CommandQueue& queue);

    /**
     * Check if image format combination is valid.
     * @return True if image format combination is valid, false otherwise.
     */
    static bool isValidImageFormat(const cl::Context& context, const cl::ImageFormat& format);

    /**
     * Check if volume format (Image3D) combination is valid.
     * @return True if image format combination is valid, false otherwise.
     */
    static bool isValidVolumeFormat(const cl::Context& context, const cl::ImageFormat& format);

    /**
     * Outputs formatted build hint to logger.
     * 
     * @param devices (const std::vector<cl::Device> &)
     * @param program (const cl::Program &)
     * @param filename (const std::string &)
     * @return (void)
     */
    static void printBuildError(const std::vector<cl::Device>& devices, const cl::Program& program, const std::string& filename = "");

    static void printBuildError(const cl::Device& device, const cl::Program& program, const std::string& filename = "");

    /** 
     * Get OpenGL sharing properties depending on operating system.
     */
    static std::vector<cl_context_properties> getGLSharingContextProperties();

    /**
     * Add a directory as an include path to be used when compiling OpenCL kernels.
     * 
     * @param directoryPath Directory path to include
     */
    void addCommonIncludeDirectory(const std::string& directoryPath);

    /**
     * Remove common include path.
     * 
     * @param directoryPath Directory path to remove
     */
    void removeCommonIncludeDirectory(const std::string& directoryPath);

    const std::vector<std::string>& getCommonIncludeDirectories() const { return includeDirectories_; }

    
private: 
    OpenCL();
    OpenCL(OpenCL const&) {};
    void operator=(OpenCL const&) {};

    /// Initialize OpenCL context and queues.
    void initialize(bool enableOpenGLSharing);

    /** 
     *  Get the device that has most compute units.
     *  @param bestDevice Set to found device, if found.
     *  @param onPlatform Set to platform that device exist on, if found.
     *  @return True if any device found, false otherwise. 
     */
    static bool getBestGPUDevice(cl::Device& bestDevice, cl::Platform& onPlatform);

    
    /**
     * Merges all include directories into a string. Each include directory will be preceded by -I 
     * 
     * @return Define to be used when building the programs
     */
    std::string getIncludeDefine() const;

    /// Queue which can perform tasks in serial (no need to explicitly manage events)
    cl::CommandQueue synchronosGPUQueue_;
    /// Queue which can perform tasks in parallel
    cl::CommandQueue asyncGPUQueue_;
    /// Default context associated with queues and device
    cl::Context gpuContext_;
    /// Default device associated with queues and device
    cl::Device gpuDevice_;
    // Include directories define
    std::vector<std::string> includeDirectories_;

};


/**
 * Computes the nearest multiple of local work group size.
 * global work group size = localWorkGroupSize*ceil((float)nItems/(float)localWorkGroupSize)
 * 
 * @param nItems 
 * @param localWorkGroupSize Local work group size of kernel
 * @return 
 */
IVW_MODULE_OPENCL_API size_t getGlobalWorkGroupSize(size_t nItems, size_t localWorkGroupSize);

IVW_MODULE_OPENCL_API svec2 getGlobalWorkGroupSize(svec2 nItems, glm::svec2 localWorkGroupSize);
IVW_MODULE_OPENCL_API svec3 getGlobalWorkGroupSize(svec3 nItems, glm::svec3 localWorkGroupSize);


/**
 * Creates a readable hint report from an OpenCL exception. 
 * Example usage: LogError(getCLErrorString(err))
 * 
 * @param err OpenCL exception
 * @return Error report string.
 */
 IVW_MODULE_OPENCL_API std::string getCLErrorString(const cl::Error& err);

/**
 * Creates an error report and outputs it using the log functionality.
 * 
 * @param err OpenCL error code
 * @param message Message to be passed along with the error.
 */
 IVW_MODULE_OPENCL_API void LogOpenCLError(cl_int err, const char* message = "");

/** \brief Get string representation of hint code according to definitions in CL/cl.h
 *
 *  \return The hint code string.
 */
IVW_MODULE_OPENCL_API std::string errorCodeToString(cl_int err);

/**
 * Returns hints on how to resolve a particular OpenCL hint. 
 * 
 * @param err 
 * @return A hint on what could cause the hint.
 */
IVW_MODULE_OPENCL_API std::string getCLErrorResolveHint(cl_int err);

#if defined(IVW_DEBUG)
#define LogCLError #if defined(__CL_ENABLE_EXCEPTIONS) \\LogOpenCLError(error)
#else
#define LogCLError
#endif

// 
/**
 * Create a cl::ImageFormat based on DataFormatId. 
 * Outputs an error message if a corresponding format does not exist and then returns the default ImageFormat.
 *
 * @see DataFormat
 * @param format Id of a DataFormat
 * @return Default ImageFormat created by the constructor 
 * if a corresponding format does not exist, otherwise an ImageFormat with corresponding channel and data type.
 */
IVW_MODULE_OPENCL_API cl::ImageFormat dataFormatToCLImageFormat(DataFormatId format);


}


#endif // IVW_INVIWOOPENGL_H
