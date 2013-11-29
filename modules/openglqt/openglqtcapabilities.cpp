#include "openglqtcapabilities.h"
#include <inviwo/core/util/formatconversion.h>
#include <inviwo/core/util/logdistributor.h>
#include <inviwo/core/util/stringconversion.h>

namespace inviwo {

#define OpenGLQtInfoNotFound(message) { LogInfo(message << " Info could not be retrieved"); }

OpenGLQtCapabilities::OpenGLQtCapabilities() {}

OpenGLQtCapabilities::~OpenGLQtCapabilities() {}

void OpenGLQtCapabilities::printInfo(){
    //Qt General Info
}

std::vector<int> OpenGLQtCapabilities::getGLVersion() {    

    const GLubyte *glversion = glGetString(GL_VERSION);
    std::string glVersionStr = std::string((glversion!=NULL ? reinterpret_cast<const char*>(glversion) : "INVALID"));

    std::vector<std::string> versionInfoStr = splitString(glVersionStr,'.');

    //ivwAssert(versionInfo.size()!=0, "Cannot retrieve GL version.");

    std::string majorVersion = versionInfoStr[0];
    std::string minorVersion = versionInfoStr[1];

    int major = stringTo<int>(majorVersion);
    int minor = stringTo<int>(minorVersion);
    
    std::vector<int> versionInfo;
    
    versionInfo.push_back(major);
    versionInfo.push_back(minor);   

    return versionInfo;
}

} // namespace
