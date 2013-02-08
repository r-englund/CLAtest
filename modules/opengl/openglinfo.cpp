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
        const GLubyte* test = glGetString(GL_RENDERER);
        std::string test2 = std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        //LogInfo("Vendor: " << ));
        LogInfo("Renderer: " << glGetString(GL_RENDERER));
        LogInfo("Version: " << glGetString(GL_VERSION));
        LogInfo("GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION));
    }

} // namespace
