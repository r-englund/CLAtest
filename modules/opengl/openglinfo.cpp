#include "openglinfo.h"
#include <inviwo/core/util/logdistributor.h>

namespace inviwo {

    #define OpenGLInfoNotFound(message) { LogInfo(message << " Info could not be retrieved"); }

    const std::string OpenGLInfo::logSource_ = "OpenGL Info";

    OpenGLInfo::OpenGLInfo() {}

    OpenGLInfo::~OpenGLInfo() {}

    void OpenGLInfo::retrieveStaticInfo(){
        
    }

    void OpenGLInfo::retrieveDynamicInfo(){
        
    }

    void OpenGLInfo::printInfo(){
        LogInfo("(Version) " << 4.2);
    }

} // namespace
