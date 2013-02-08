#include "openglinfo.h"
#include <inviwo/core/util/logdistributor.h>
#include <inviwo/core/util/stringconversion.h>

namespace inviwo {

#define OpenGLInfoNotFound(message) { LogInfo(message << " Info could not be retrieved"); }

const std::string OpenGLInfo::logSource_ = "OpenGL Info";

OpenGLInfo::OpenGLInfo() {
    supportedShaderVersions_.clear();
}

OpenGLInfo::~OpenGLInfo() {}

void OpenGLInfo::printInfo(){
    LogInfo("Vendor: " << glGetString(GL_VENDOR));
    LogInfo("Renderer: " << glGetString(GL_RENDERER));
    LogInfo("Version: " << glGetString(GL_VERSION));
    LogInfo("GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION));

    if(isSupported("GL_VERSION_2_0"))
        LogInfo("Shaders are supported");

    for(size_t i=0; i<supportedShaderVersions_.size(); i++){
        if(supportedShaderVersions_[i].hasProfile()){
            LogInfo("Supports shader version " << supportedShaderVersions_[i].getVersion() << " with profile " << supportedShaderVersions_[i].getProfile());
        }
        else{
            LogInfo("Supports shader version " << supportedShaderVersions_[i].getVersion());
        }
    }
}

bool OpenGLInfo::isExtensionSupported(const char* name){
    return (glewIsExtensionSupported(name) != '0');
}

bool OpenGLInfo::isSupported(const char* name){
    return (glewIsSupported(name) != '0');
}

void OpenGLInfo::retrieveStaticInfo(){
#ifdef GLEW_VERSION_4_3
    GLint numberOfSupportedVersions;
    glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &numberOfSupportedVersions);
    for(int i=0; i<numberOfSupportedVersions; i++){
        parseAndAddShaderVersion(toString(glGetStringi(GL_SHADING_LANGUAGE_VERSION, i)));
    }
#else
#ifdef GLEW_VERSION_4_2
    addShaderVersion(GLSLShaderVersion(420, "core"));
    addShaderVersion(GLSLShaderVersion(420, "compatibility"));
#endif
#ifdef GLEW_VERSION_4_1
    addShaderVersion(GLSLShaderVersion(410, "core"));
    addShaderVersion(GLSLShaderVersion(410, "compatibility"));
#endif
#ifdef GLEW_VERSION_4_0
    addShaderVersion(GLSLShaderVersion(400, "core"));
    addShaderVersion(GLSLShaderVersion(400, "compatibility"));
#endif
#ifdef GLEW_VERSION_3_3
    addShaderVersion(GLSLShaderVersion(330, "core"));
    addShaderVersion(GLSLShaderVersion(300, "compatibility"));
#endif
#ifdef GLEW_VERSION_3_2
    addShaderVersion(GLSLShaderVersion(150, "core"));
    addShaderVersion(GLSLShaderVersion(150, "compatibility"));
#endif
#ifdef GLEW_VERSION_3_1
    addShaderVersion(GLSLShaderVersion(140));
#endif
#ifdef GLEW_VERSION_3_0
    addShaderVersion(GLSLShaderVersion(130));
#endif
#ifdef GLEW_VERSION_2_1
    addShaderVersion(GLSLShaderVersion(120));
#endif
#ifdef GLEW_VERSION_2_0
    addShaderVersion(GLSLShaderVersion(110));
#endif
#endif  
}

void OpenGLInfo::retrieveDynamicInfo(){

}

void OpenGLInfo::addShaderVersion(GLSLShaderVersion version){
    supportedShaderVersions_.push_back(version);
}

void OpenGLInfo::parseAndAddShaderVersion(std::string versionStr){
    //Assumes <version><space><profile> or <version>, example 420 core or 300 es
    if(!versionStr.empty()){
        std::vector<std::string> versionSplit = splitString(versionStr);
        if(versionSplit.size() > 1)
            addShaderVersion(GLSLShaderVersion(stringTo<int>(versionSplit[0]), versionSplit[1]));
        else
            addShaderVersion(GLSLShaderVersion(stringTo<int>(versionSplit[0])));
    }
}

} // namespace
