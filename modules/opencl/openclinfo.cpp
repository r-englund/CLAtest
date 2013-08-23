#include "openclinfo.h"
#include <inviwo/core/util/logdistributor.h>
#include <modules/opencl/inviwoopencl.h>

namespace inviwo {

    std::string imageFormatToString(const cl::ImageFormat& format) {
        std::string order;
        switch (format.image_channel_order) {
            case CL_R:          order = "CL_R"; break;
            case CL_RG:         order = "CL_RG"; break;   
            case CL_RA:         order = "CL_RA"; break;
            case CL_RGB:        order = "CL_RGB"; break;
            case CL_RGBA:       order = "CL_RGBA"; break;
            case CL_BGRA:       order = "CL_BGRA"; break;
            case CL_ARGB:       order = "CL_ARGB"; break; 
            case CL_INTENSITY:  order = "CL_INTENSITY"; break;
            case CL_LUMINANCE:  order = "CL_LUMINANCE"; break;  
        #ifdef CL_Rx
            case CL_Rx:         order = "CL_Rx"; break;
            case CL_RGx:        order = "CL_RGx"; break;
            case CL_RGBx:       order = "CL_RGBx"; break;
        #endif
            default:            order = "unkown channel order"; break;
        }
        std::string type;
        switch (format.image_channel_data_type) {
            case CL_SNORM_INT8: type =          "CL_SNORM_INT8"; break;
            case CL_SNORM_INT16: type =         "CL_SNORM_INT16"; break;
            case CL_UNORM_INT8: type =          "CL_UNORM_INT8"; break;
            case CL_UNORM_INT16: type =         "CL_UNORM_INT16"; break;    
            case CL_UNORM_SHORT_565: type =     "CL_UNORM_SHORT_565"; break;   
            case CL_UNORM_SHORT_555: type =     "CL_UNORM_SHORT_555"; break;
            case CL_UNORM_INT_101010: type =    "CL_UNORM_INT_101010"; break;
            case CL_SIGNED_INT8: type =         "CL_SIGNED_INT8"; break;
            case CL_SIGNED_INT16: type =        "CL_SIGNED_INT16"; break;
            case CL_SIGNED_INT32: type =        "CL_SIGNED_INT32"; break;
            case CL_UNSIGNED_INT8: type =       "CL_UNSIGNED_INT8"; break;
            case CL_UNSIGNED_INT16: type =      "CL_UNSIGNED_INT16"; break; 
            case CL_UNSIGNED_INT32: type =      "CL_UNSIGNED_INT32"; break; 
            case CL_HALF_FLOAT: type =          "CL_HALF_FLOAT"; break; 
            case CL_FLOAT: type =               "CL_FLOAT"; break;                 
            default: type =                     "unkown data type"; break;
        }     
        
        std::ostringstream stream;
        stream << "(" << order << ", " << type << ")";
        return stream.str();
        
    }

    OpenCLInfo::OpenCLInfo() {}

    OpenCLInfo::~OpenCLInfo() {}

    void OpenCLInfo::retrieveStaticInfo(){
        
    }

    void OpenCLInfo::retrieveDynamicInfo(){
        
    }

    void OpenCLInfo::printInfo(){
        OpenCLInfo::printDeviceInfo(OpenCL::instance()->getDevice());
        
        //// Supported image 2D formats
        //std::vector<cl::ImageFormat> formats;
        //OpenCL::instance()->getContext().getSupportedImageFormats(CL_MEM_READ_WRITE, CL_MEM_OBJECT_IMAGE2D, &formats);
        //std::ostringstream stream;
        ////stream << "Supported 2D READ_WRITE formats: " << imageFormatToString(formats[0]);
        ////for(::size_t i = 1; i < formats.size(); ++i) {
        ////    stream << ", " << imageFormatToString(formats[i]) << std::endl;
        ////}
        //stream << "Supported 2D READ_WRITE formats: " << std::endl;
        //for(::size_t i = 0; i < formats.size(); ++i) {
        //    stream << imageFormatToString(formats[i]) << std::endl;
        //}
        //LogInfo(stream.str())
        //formats.clear();
        //OpenCL::instance()->getContext().getSupportedImageFormats(CL_MEM_READ_WRITE, CL_MEM_OBJECT_IMAGE3D, &formats);
        //stream.clear();
        ////stream << "Supported 3D READ_WRITE formats: " << imageFormatToString(formats[0]);
        ////for(::size_t i = 1; i < formats.size(); ++i) {
        ////    stream << ", " << imageFormatToString(formats[i]) << std::endl;
        ////}
        //stream << "Supported 3D READ_WRITE formats: " << std::endl;
        //for(::size_t i = 0; i < formats.size(); ++i) {
        //    stream << imageFormatToString(formats[i]) << std::endl;
        //}
        //LogInfo(stream.str())
    }

    void OpenCLInfo::printDeviceInfo(const cl::Device& device) {

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
        catch (cl::Error& e)
        {
            LogErrorCustom("OpenCL", "Error while retrieving device info: " << e.what());
        }        
    }



} // namespace

