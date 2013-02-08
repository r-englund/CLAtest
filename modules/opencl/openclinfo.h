#ifndef IVW_OPENCL_INFO_H
#define IVW_OPENCL_INFO_H


#include <inviwo/core/util/resourceinfo.h>
#include <modules/opencl/openclmoduledefine.h>
#include <modules/opencl/cl.hpp>
#include <modules/opencl/glmcl.h>
#include <inviwo/core/util/logdistributor.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <fstream>


// This file contains macros and functions for printing OpenCL information.


namespace inviwo {
    
    class IVW_MODULE_OPENCL_API OpenCLInfo: public ResourceInfo {
        
    public:
        OpenCLInfo();
        virtual ~OpenCLInfo();
        
        void printInfo();
        
    protected:
        void retrieveStaticInfo();
        void retrieveDynamicInfo();
        
        static const std::string logSource_; ///< Source string to be displayed for log messages.
    };
    
}

// Copied macros __PARAM_NAME_INFO_X_X from cl.hpp 
// but modified to only contain cl_device_info.
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
std::string deviceInfoToString(cl_device_info info, const T& value, bool is_cl_bool);
template<>
std::string deviceInfoToString(cl_device_info, const std::string& value, bool) {
    return value;
}
template<typename T>
std::string deviceInfoToString(cl_device_info info, const std::vector<T>& value, bool);

// We do not print platform info so far.
template<>
std::string deviceInfoToString(cl_device_info, const cl_platform_id& value, bool);

#define __CL_PRINT_DEVICE_INFO(token, param_name, T) \
    if(std::string(#T).compare("cl_bool") == 0) { \
        LogInfoS("OpenCL", #param_name << ": " << deviceInfoToString(param_name, device.getInfo<param_name>(), true)) \
    } else {                                        \
        LogInfoS("OpenCL", #param_name << ": " << deviceInfoToString(param_name, device.getInfo<param_name>(), false)) \
    } 
        
#endif

