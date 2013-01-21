#ifndef IVW_INVIWOOPENCL_H
#define IVW_INVIWOOPENCL_H
#include <vector>
#include <iostream>
#include "ext/glm/glm.hpp"
#include "modules/opencl/cl.hpp"
#include "modules/opencl/openclmoduledefine.h"

namespace cl{


void LogOpenCLError(cl_int err, const char* fileName, const char* functionName, int lineNumber);
/*! \brief Get string representation of error code according to definitions in CL/cl.h
 *
 *  \return The error code string.
 */
std::string errorCodeToString(cl_int err);


};

using cl::CommandQueue;
using cl::Context;
using cl::Device;
using cl::Platform;
using cl::Program;
using cl::ImageFormat;

namespace inviwo {


#if defined(IVW_DEBUG)
#define LogCLError cl::LogOpenCLError(error, __FILE__, __FUNCTION__, __LINE__)
#else
#define LCL_ERROR
#endif


std::string loadFile(std::string fileName);

/** \class OpenCL 
*
* Singleton class that manages OpenCL context and queues. 
*
*/
class IVW_MODULE_OPENCL_API OpenCL {
public:


    static OpenCL* getInstance() {
        static OpenCL instance;// Guaranteed to be destroyed. Instantiated on first use.
        return &instance;
    }
    /*
     * Get queue that can perform tasks in serial (no need to explicitly manage events).
     * This should be the first choice of use. Expert users may want to use the 
     * asynchronous queue.
     */
    const cl::CommandQueue& getSynchronosGPUQueue() const { return synchronosGPUQueue_; }
    /*
     * Get queue that can perform tasks in parallel (need to explicitly manage events).
     * This is for expert users. be the first choice of use. Expert users may want to use the 
     * asynchronous queue.
     */
    const cl::CommandQueue& getAsynchronousGPUQueue() const { return synchronosGPUQueue_; }

    /*
     * Get default OpenCL context.
     */
    const cl::Context& getContext() const { return gpuContext_; }

    /*
     * Get default OpenCL device.
     */
    const cl::Device& getDevice() const { return gpuDevice_; }

    /*
     * Check if image format combination is valid.
     * @return True if image format combination is valid, false otherwise.
     */
    static bool isValidImageFormat(const cl::Context& context, const cl::ImageFormat& format);

    /*
     * Check if volume format (Image3D) combination is valid.
     * @return True if image format combination is valid, false otherwise.
     */
    static bool isValidVolumeFormat(const cl::Context& context, const cl::ImageFormat& format);

    /*
     * Print all available information on device.
     */
    static void printDeviceInfo(const cl::Device& device);

    /**
     * Outputs formatted build error to logger.
     * 
     * \param devices (const std::vector<cl::Device> &)
     * \param program (const cl::Program &)
     * \param filename (const std::string &)
     * \return (void)
     */
    static void printBuildError(const std::vector<cl::Device>& devices, const cl::Program& program, const std::string& filename = "");

    static void printBuildError(const cl::Device& device, const cl::Program& program, const std::string& filename = "");

    /*! Get OpenGL sharing properties depending on operating system.
     *   
     */
    static std::vector<cl_context_properties> getGLSharingContextProperties();

    
private: 
    OpenCL();
    OpenCL(OpenCL const&) {};
    void operator=(OpenCL const&) {};

    /// Initialize OpenCL context and queues.
    void initialize(bool enableOpenGLSharing);

    /*! Get the device that has most compute units.
     *  
     *  @param bestDevice Set to found device, if found.
     *  @param onPlatform Set to platform that device exist on, if found.
     *  \return True if any device found, false otherwise. 
     */
    static bool getBestGPUDevice(cl::Device& bestDevice, cl::Platform& onPlatform);
    /// Queue which can perform tasks in serial (no need to explicitly manage events)
    cl::CommandQueue synchronosGPUQueue_;
    /// Queue which can perform tasks in parallel
    cl::CommandQueue asyncGPUQueue_;
    /// Default context associated with queues and device
    cl::Context gpuContext_;
    /// Default device associated with queues and device
    cl::Device gpuDevice_;

};





}


#endif // IVW_INVIWOOPENGL_H
