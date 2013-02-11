#include "openglinfo.h"
#include <inviwo/core/util/logdistributor.h>
#include <inviwo/core/util/stringconversion.h>
#include <inviwo/core/util/formatconversion.h>
#include <string>

namespace inviwo {

#define OpenGLInfoNotFound(message) { LogInfo(message << " Info could not be retrieved"); }

const std::string OpenGLInfo::logSource_ = "OpenGL Info";

OpenGLInfo::OpenGLInfo() {
    supportedShaderVersions_.clear();
}

OpenGLInfo::~OpenGLInfo() {}

void OpenGLInfo::printInfo(){
    LogInfo("GL Vendor: " << glGetString(GL_VENDOR));
    LogInfo("GL Renderer: " << glGetString(GL_RENDERER));
    LogInfo("GL Version: " << glGetString(GL_VERSION));

    //GLSL
    
    if(isShadersSupported()){
        LogInfo("GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION));
        LogInfo("Shaders supported: YES");
    }
    else if(isShadersSupportedARB()){
        LogInfo("GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION_ARB));
        LogInfo("Shaders supported: YES(ARB)");
    }
    else
        LogInfo("Shaders supported: NO");

   LogInfo("Framebuffer objects supported: " << (isFboSupported() ? "YES" : "NO "));

    // Texturing
    LogInfo("1D/2D textures supported: " << (isTexturesSupported() ? "YES" : "NO "));
    LogInfo("3D textures supported: " << (is3DTexturesSupported() ? "YES" : "NO "));
    LogInfo("Array textures supported: " << (isTextureArraysSupported() ? "YES" : "NO "));
    
    if(isTexturesSupported()){
        LogInfo("Max 1D/2D texture size: " << getMaxTexSize());
    }
    if(is3DTexturesSupported()){
        LogInfo("Max 3D texture size: " << getMax3DTexSize());
    }
    if(isTextureArraysSupported()){
        LogInfo("Max array texture size: " << getMaxArrayTexSize());
    }

    if(isTexturesSupported()){
        LogInfo("Max number of texture units: " << getNumTexUnits());
        int totalMem = getTotalAvailableTextureMem();
        LogInfo("Total available texture memory: " << (totalMem>0 ? formatBytesToString(totalMem) : "UNKNOWN"));
        int curMem = getCurrentAvailableTextureMem();
        LogInfo("Current available texture memory: " << (curMem>0 ? formatBytesToString(curMem) : "UNKNOWN"));
    }

    /*for(size_t i=0; i<supportedShaderVersions_.size(); i++){
        if(supportedShaderVersions_[i].hasProfile()){
            LogInfo("Supports shader version " << supportedShaderVersions_[i].getVersion() << " with profile " << supportedShaderVersions_[i].getProfile());
        }
        else{
            LogInfo("Supports shader version " << supportedShaderVersions_[i].getVersion());
        }
    }*/
}

bool OpenGLInfo::isExtensionSupported(const char* name){
    return (glewIsExtensionSupported(name) != '0');
}

bool OpenGLInfo::isSupported(const char* name){
    return (glewIsSupported(name) != '0');
}

bool OpenGLInfo::isTexturesSupported(){
    return texSupported_;
}

bool OpenGLInfo::isTextureArraysSupported() {
    return texArraySupported_;
}

bool OpenGLInfo::is3DTexturesSupported(){
    return tex3DSupported_;
}

bool OpenGLInfo::isFboSupported(){
    return fboSupported_;
}

bool OpenGLInfo::isShadersSupported(){
    return shadersAreSupported_;
}

bool OpenGLInfo::isShadersSupportedARB(){
    return shadersAreSupportedARB_;
}

int OpenGLInfo::getCurrentAvailableTextureMem() throw (Exception) {
    int currentAvailableTexMeminBytes = 0;

    try{
        GLint nCurAvailMemoryInKB = 0;
        if(glVendor_ == NVIDIA) {
#ifdef GL_NVX_gpu_memory_info
            glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &nCurAvailMemoryInKB);
#endif
        }
        else if(glVendor_ == AMD) {
#ifdef GL_ATI_meminfo
            glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI, &nCurAvailMemoryInKB);
#endif
        }
        currentAvailableTexMeminBytes = KILOBYTES_TO_BYTES(static_cast<int>(nCurAvailMemoryInKB));
    }
    catch(const Exception& e){
        LogWarn("Failed to fetch current available texture memory: " << e.what());
    }

    return currentAvailableTexMeminBytes;
}

int OpenGLInfo::getTotalAvailableTextureMem() throw (Exception) {
    int totalAvailableTexMemInBytes = 0;

    try{
        GLint nTotalAvailMemoryInKB = 0;
        if(glVendor_ == NVIDIA) {
#ifdef GL_NVX_gpu_memory_info
            glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &nTotalAvailMemoryInKB);
#endif
        }
        totalAvailableTexMemInBytes = KILOBYTES_TO_BYTES(static_cast<int>(nTotalAvailMemoryInKB));
    }
    catch(const Exception& e){
        LogWarn("Failed to fetch total available texture memory: " << e.what());
    }

    return totalAvailableTexMemInBytes;
}

int OpenGLInfo::getMaxProgramLoopCount(){
    return maxProgramLoopCount_;
}

int OpenGLInfo::getNumTexUnits(){
    return numTexUnits_;
}

int OpenGLInfo::getMaxTexSize(){
    return maxTexSize_;
}

int OpenGLInfo::getMax3DTexSize(){
    return max3DTexSize_;
}

int OpenGLInfo::getMaxArrayTexSize(){
    return maxArrayTexSize_;
}

int OpenGLInfo::getMaxColorAttachments(){
    return maxArrayTexSize_;
}

void OpenGLInfo::retrieveStaticInfo(){
    //GL Vendor
    std::string glVendorStr = std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    if (glVendorStr.find("NVIDIA") != std::string::npos)
        glVendor_ = NVIDIA;
    else if (glVendorStr.find("AMD") != std::string::npos || glVendorStr.find("ATI") != std::string::npos)
        glVendor_ = AMD;
    else if (glVendorStr.find("INTEL") != std::string::npos || glVendorStr.find("Intel") != std::string::npos)
        glVendor_ = INTEL;
    else 
        glVendor_ = UNKNOWN;

    //GLSL
    shadersAreSupported_ = isSupported("GL_VERSION_2_0");
    shadersAreSupportedARB_ = isExtensionSupported("GL_ARB_fragment_program");

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

    maxProgramLoopCount_ = -1;
    if (GLEW_NV_fragment_program2) {
        GLint i = -1;
        glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_LOOP_COUNT_NV, &i);
        if (i > 0) {
            //Restrict cycles to realistic samplingRate*maximumDimension, 20*(10 000) slices = 200 000
            //maxProgramLoopCount_ = std::min(static_cast<int>(i), 200000);
        }
    }

    //Texturing
#ifdef GLEW_VERSION_1_1
    texSupported_ = true;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, (GLint*)&maxTexSize_);
#else
    texSupported_ = isExtensionSupported("GL_EXT_texture");
    maxTexSize_ = 0;
#endif
#ifdef GLEW_VERSION_1_2
    tex3DSupported_ = true;
    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, (GLint*)&max3DTexSize_);
#else
    tex3DSupported_ = isExtensionSupported("GL_EXT_texture3D");
    if(is3DTexturesSupported())
        glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE_EXT, (GLint*)&max3DTexSize_);
    else
        max3DTexSize_ = 0;
#endif
#ifdef GLEW_VERSION_3_0
    texArraySupported_ = true;
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, (GLint*)&maxArrayTexSize_);
#else
    texArraySupported_ = isExtensionSupported("GL_EXT_texture_array");
    if(isTextureArraysSupported())
        glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS_EXT, (GLint*)&maxArrayTexSize_);
    else
        maxArrayTexSize_ = 0;
#endif

    numTexUnits_ = -1;
    if(isShadersSupported())
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, (GLint*)&numTexUnits_);
    if(getNumTexUnits() < 0)
        glGetIntegerv(GL_MAX_TEXTURE_UNITS, (GLint*)&numTexUnits_);


    //FBO
    fboSupported_ = isExtensionSupported("GL_EXT_framebuffer_object");
}

void OpenGLInfo::retrieveDynamicInfo(){

}

void OpenGLInfo::addShaderVersion(GLSLShaderVersion version){
    supportedShaderVersions_.push_back(version);
}

void OpenGLInfo::parseAndAddShaderVersion(std::string versionStr){
    //Assumes <version><space><profile> or <version>, example 420 core or 140
    if(!versionStr.empty()){
        std::vector<std::string> versionSplit = splitString(versionStr);
        if(versionSplit.size() > 1)
            addShaderVersion(GLSLShaderVersion(stringTo<int>(versionSplit[0]), versionSplit[1]));
        else
            addShaderVersion(GLSLShaderVersion(stringTo<int>(versionSplit[0])));
    }
}

} // namespace
