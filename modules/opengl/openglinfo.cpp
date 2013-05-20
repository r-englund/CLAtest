#include "openglinfo.h"
#include <inviwo/core/util/formatconversion.h>
#include <inviwo/core/util/logdistributor.h>
#include <inviwo/core/util/stringconversion.h>

namespace inviwo {

#define OpenGLInfoNotFound(message) { LogInfo(message << " Info could not be retrieved"); }

OpenGLInfo::GLSLShaderVersion::GLSLShaderVersion() : number_(0), profile_("") {}

OpenGLInfo::GLSLShaderVersion::GLSLShaderVersion(int num) : number_(num), profile_("") {}

OpenGLInfo::GLSLShaderVersion::GLSLShaderVersion(int num, std::string pro) : number_(num), profile_(pro) {}

std::string OpenGLInfo::GLSLShaderVersion::getProfile() { 
    return profile_; 
}

int OpenGLInfo::GLSLShaderVersion::getVersion() { 
    return number_; 
}

std::string OpenGLInfo::GLSLShaderVersion::getVersionAsString(){
    return toString<int>(number_); 
}

std::string OpenGLInfo::GLSLShaderVersion::getVersionAndProfileAsString() { 
    return (hasProfile() ? getVersionAsString() + " " + profile_ : getVersionAsString()); 
}

bool OpenGLInfo::GLSLShaderVersion::hasProfile() { 
    return (profile_ != ""); 
}

OpenGLInfo::OpenGLInfo() {
    supportedShaderVersions_.clear();
    currentGlobalGLSLHeader_ = "";
    preferredGLSLProfile_ = "compatibility";
}

OpenGLInfo::~OpenGLInfo() {}

void OpenGLInfo::printInfo(){
    //OpenGL General Info
    LogInfo("Vendor: " << glVendorStr_);
    LogInfo("Renderer: " << glRenderStr_);
    LogInfo("Version: " << glVersionStr_);

    //GLSL
    if (isShadersSupported()){
        LogInfo("GLSL version: " << glslVersionStr_);
        LogInfo("Current set global GLSL version: " << getCurrentShaderVersion().getVersionAndProfileAsString());
        LogInfo("Shaders supported: YES");
    }
    else if (isShadersSupportedARB()){
        LogInfo("GLSL version: " << glslVersionStr_);
        LogInfo("Current set global GLSL version: " << getCurrentShaderVersion().getVersionAndProfileAsString());
        LogInfo("Shaders supported: YES(ARB)");
    }
    else
        LogInfo("Shaders supported: NO");

   LogInfo("Framebuffer objects supported: " << (isFboSupported() ? "YES" : "NO "));

    // Texturing
    LogInfo("1D/2D textures supported: " << (isTexturesSupported() ? "YES" : "NO "));
    LogInfo("3D textures supported: " << (is3DTexturesSupported() ? "YES" : "NO "));
    LogInfo("Array textures supported: " << (isTextureArraysSupported() ? "YES" : "NO "));
    
    if (isTexturesSupported()){
        LogInfo("Max 1D/2D texture size: " << getMaxTexSize());
    }
    if (is3DTexturesSupported()){
        LogInfo("Max 3D texture size: " << getMax3DTexSize());
    }
    if (isTextureArraysSupported()){
        LogInfo("Max array texture size: " << getMaxArrayTexSize());
    }
    if (isFboSupported()){
        LogInfo("Max color attachments: " << getMaxColorAttachments());
    }


    if (isTexturesSupported()){
        LogInfo("Max number of texture units: " << getNumTexUnits());
        int totalMem = getTotalAvailableTextureMem();
        LogInfo("Total available texture memory: " << (totalMem>0 ? formatBytesToString(totalMem) : "UNKNOWN"));
        int curMem = getCurrentAvailableTextureMem();
        LogInfo("Current available texture memory: " << (curMem>0 ? formatBytesToString(curMem) : "UNKNOWN"));
    }
}

bool OpenGLInfo::canAllocate(uint64_t dataSize, uint8_t percentageOfAvailableMemory){
    return getCurrentAvailableTextureMem()*percentageOfAvailableMemory/100 >= dataSize;
}

uvec3 OpenGLInfo::calculateOptimalBrickSize(uvec3 dimensions, size_t formatSizeInBytes, uint8_t percentageOfAvailableMemory){
    uvec3 currentBrickDimensions = dimensions;
    while(!canAllocate(getMemorySizeInBytes(currentBrickDimensions, formatSizeInBytes), percentageOfAvailableMemory)){
        int theMaxDim = (currentBrickDimensions.x > currentBrickDimensions.y ? (currentBrickDimensions.x > currentBrickDimensions.z ? 0 : 2) : (currentBrickDimensions.y > currentBrickDimensions.z ? 1 : 2));
        if (currentBrickDimensions[theMaxDim] % 2 != 0)
            currentBrickDimensions[theMaxDim]++; //Make the dim we are dividing even
        currentBrickDimensions[theMaxDim] /= 2;
    }

    // adapt brick size according to maximum texture dimension
    unsigned int maxGPUTextureDim = static_cast<unsigned int>(getMaxTexSize());
    while (currentBrickDimensions.x>maxGPUTextureDim || currentBrickDimensions.y>maxGPUTextureDim || currentBrickDimensions.z>maxGPUTextureDim) {
        int theMaxDim = (currentBrickDimensions.x > currentBrickDimensions.y ? (currentBrickDimensions.x > currentBrickDimensions.z ? 0 : 2) : (currentBrickDimensions.y > currentBrickDimensions.z ? 1 : 2));
        if (currentBrickDimensions[theMaxDim] % 2 != 0)
            currentBrickDimensions[theMaxDim]++; //Make the dim we are dividing even
        currentBrickDimensions[theMaxDim] /= 2;
    }

    return currentBrickDimensions;
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

OpenGLInfo::GLSLShaderVersion OpenGLInfo::getCurrentShaderVersion(){
    return supportedShaderVersions_[currentGlobalGLSLVersionIdx_];
}

std::string OpenGLInfo::getCurrentGlobalGLSLHeader(){
    if (currentGlobalGLSLHeader_ == "")
        rebuildGLSLHeader();

    return currentGlobalGLSLHeader_;
}

int OpenGLInfo::getCurrentAvailableTextureMem() throw (Exception) {
    int currentAvailableTexMeminBytes = 0;

    try{
        GLint nCurAvailMemoryInKB = 0;
        if (glVendor_ == NVIDIA) {
#ifdef GL_NVX_gpu_memory_info
            glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &nCurAvailMemoryInKB);
#endif
        }
        else if (glVendor_ == AMD) {
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
        if (glVendor_ == NVIDIA) {
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
    return maxColorAttachments_;
}

void OpenGLInfo::retrieveStaticInfo(){
    //GL
    const GLubyte *vendor = glGetString(GL_VENDOR);
    glVendorStr_ = std::string((vendor!=NULL ? reinterpret_cast<const char*>(vendor) : "INVALID"));
    if (glVendorStr_.find("NVIDIA") != std::string::npos)
        glVendor_ = NVIDIA;
    else if (glVendorStr_.find("AMD") != std::string::npos || glVendorStr_.find("ATI") != std::string::npos)
        glVendor_ = AMD;
    else if (glVendorStr_.find("INTEL") != std::string::npos || glVendorStr_.find("Intel") != std::string::npos)
        glVendor_ = INTEL;
    else 
        glVendor_ = UNKNOWN;

    const GLubyte *glrender = glGetString(GL_RENDERER);
    glRenderStr_ = std::string((glrender!=NULL ? reinterpret_cast<const char*>(glrender) : "INVALID"));

    const GLubyte *glversion = glGetString(GL_VERSION);
    glVersionStr_ = std::string((glversion!=NULL ? reinterpret_cast<const char*>(glversion) : "INVALID"));

    //GLSL
    shadersAreSupported_ = isSupported("GL_VERSION_2_0");
    shadersAreSupportedARB_ = isExtensionSupported("GL_ARB_fragment_program");

    GLint numberOfSupportedVersions = 0;
#ifdef GLEW_VERSION_4_3
    glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &numberOfSupportedVersions);
    for(int i=0; i<numberOfSupportedVersions; i++){
        parseAndAddShaderVersion(toString<const GLubyte*>(glGetStringi(GL_SHADING_LANGUAGE_VERSION, i)));
    }
#endif
    if (numberOfSupportedVersions == 0){
        const GLubyte *glslStrByte = NULL;
        if (isShadersSupported())
            glslStrByte = glGetString(GL_SHADING_LANGUAGE_VERSION);
        else if (isShadersSupportedARB())
            glslStrByte = glGetString(GL_SHADING_LANGUAGE_VERSION_ARB);

        glslVersionStr_ = std::string((glslStrByte!=NULL ? reinterpret_cast<const char*>(glslStrByte) : "000"));

        int glslVersion = parseAndRetrieveShaderVersion(glslVersionStr_);

        if (glslVersion != 0){
#ifdef GLEW_VERSION_4_3
            addShaderVersionIfEqualOrLower(GLSLShaderVersion(430, "core"), glslVersion);
            addShaderVersionIfEqualOrLower(GLSLShaderVersion(430, "compatibility"), glslVersion);
#endif
#ifdef GLEW_VERSION_4_2
            addShaderVersionIfEqualOrLower(GLSLShaderVersion(420, "core"), glslVersion);
            addShaderVersionIfEqualOrLower(GLSLShaderVersion(420, "compatibility"), glslVersion);
#endif
#ifdef GLEW_VERSION_4_1
            addShaderVersionIfEqualOrLower(GLSLShaderVersion(410, "core"), glslVersion);
            addShaderVersionIfEqualOrLower(GLSLShaderVersion(410, "compatibility"), glslVersion);
#endif
#ifdef GLEW_VERSION_4_0
            addShaderVersionIfEqualOrLower(GLSLShaderVersion(400, "core"), glslVersion);
            addShaderVersionIfEqualOrLower(GLSLShaderVersion(400, "compatibility"), glslVersion);
#endif
#ifdef GLEW_VERSION_3_3
            addShaderVersionIfEqualOrLower(GLSLShaderVersion(330, "core"), glslVersion);
            addShaderVersionIfEqualOrLower(GLSLShaderVersion(330, "compatibility"), glslVersion);
#endif
#ifdef GLEW_VERSION_3_2
            addShaderVersionIfEqualOrLower(GLSLShaderVersion(150, "core"), glslVersion);
            addShaderVersionIfEqualOrLower(GLSLShaderVersion(150, "compatibility"), glslVersion);
#endif
#ifdef GLEW_VERSION_3_1
            addShaderVersionIfEqualOrLower(GLSLShaderVersion(140), glslVersion);
#endif
#ifdef GLEW_VERSION_3_0
            addShaderVersionIfEqualOrLower(GLSLShaderVersion(130), glslVersion);
#endif
#ifdef GLEW_VERSION_2_1
            addShaderVersionIfEqualOrLower(GLSLShaderVersion(120), glslVersion);
#endif
#ifdef GLEW_VERSION_2_0
            addShaderVersionIfEqualOrLower(GLSLShaderVersion(110), glslVersion);
#endif
        }
    }

    //Set current used GLSL version to highest(i.e. 1st in vector) with preferred profile (or no profile)
    if (isShadersSupported() || isShadersSupportedARB()){
        size_t i = 0;
        while(i<supportedShaderVersions_.size() && (supportedShaderVersions_[i].hasProfile() && supportedShaderVersions_[i].getProfile() != preferredGLSLProfile_))
            i++;
        currentGlobalGLSLVersionIdx_ = i;
    }

    maxProgramLoopCount_ = -1;
    if (GLEW_NV_fragment_program2) {
        GLint i = -1;
        glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_LOOP_COUNT_NV, &i);
        if (i > 0) {
            //Restrict cycles to realistic samplingRate*maximumDimension, 20*(10 000) slices = 200 000
            maxProgramLoopCount_ = std::min<int>(static_cast<int>(i), 200000);
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
    if (is3DTexturesSupported())
        glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE_EXT, (GLint*)&max3DTexSize_);
    else
        max3DTexSize_ = 0;
#endif
#ifdef GLEW_VERSION_3_0
    texArraySupported_ = true;
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, (GLint*)&maxArrayTexSize_);
#else
    texArraySupported_ = isExtensionSupported("GL_EXT_texture_array");
    if (isTextureArraysSupported())
        glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS_EXT, (GLint*)&maxArrayTexSize_);
    else
        maxArrayTexSize_ = 0;
#endif

    numTexUnits_ = -1;
    if (isShadersSupported())
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, (GLint*)&numTexUnits_);
    if (getNumTexUnits() < 0)
        glGetIntegerv(GL_MAX_TEXTURE_UNITS, (GLint*)&numTexUnits_);


    //FBO
    fboSupported_ = isExtensionSupported("GL_EXT_framebuffer_object");
    
    maxColorAttachments_ = 0;
    if (isFboSupported())
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &maxColorAttachments_);
}

void OpenGLInfo::retrieveDynamicInfo(){

}

void OpenGLInfo::rebuildGLSLHeader(){
    currentGlobalGLSLHeader_ = "#version " + supportedShaderVersions_[currentGlobalGLSLVersionIdx_].getVersionAndProfileAsString() + "\n";

    if (supportedShaderVersions_[currentGlobalGLSLVersionIdx_].getVersion() == 140 && preferredGLSLProfile_ == "compatibility")
        currentGlobalGLSLHeader_ += "#extension GL_ARB_compatibility : enable\n";

    if (supportedShaderVersions_[currentGlobalGLSLVersionIdx_].hasProfile()){
        currentGlobalGLSLHeader_ += "#define GLSL_PROFILE_" + toUpper(supportedShaderVersions_[currentGlobalGLSLVersionIdx_].getProfile()) + "\n";
    }

    int lastVersion = -1;
    for(size_t i=currentGlobalGLSLVersionIdx_; i<supportedShaderVersions_.size(); i++){
        if (lastVersion != supportedShaderVersions_[i].getVersion()){
            currentGlobalGLSLHeader_ += "#define GLSL_VERSION_" + supportedShaderVersions_[i].getVersionAsString() + "\n";
            lastVersion = supportedShaderVersions_[i].getVersion();
        }
    }

    if (getMaxProgramLoopCount() > 0){
        currentGlobalGLSLHeader_ += "#define MAX_PROGRAM_LOOP_COUNT " + toString(getMaxProgramLoopCount()) + "\n";
    }
}

void OpenGLInfo::addShaderVersion(GLSLShaderVersion version){
    supportedShaderVersions_.push_back(version);
}

void OpenGLInfo::addShaderVersionIfEqualOrLower(GLSLShaderVersion version, int compVersion){
    if (version.getVersion() <= compVersion)
        addShaderVersion(version);
}

void OpenGLInfo::parseAndAddShaderVersion(std::string versionStr){
    //Assumes <version><space><profile> or <version>, example 420 core or 140
    if (!versionStr.empty()){
        std::vector<std::string> versionSplit = splitString(versionStr);
        if (versionSplit.size() > 1)
            addShaderVersion(GLSLShaderVersion(stringTo<int>(versionSplit[0]), versionSplit[1]));
        else
            addShaderVersion(GLSLShaderVersion(stringTo<int>(versionSplit[0])));
    }
}

int OpenGLInfo::parseAndRetrieveShaderVersion(std::string versionStr){
    //Assumes <version><space><desc>
    if (!versionStr.empty()){
        std::vector<std::string> versionSplit = splitString(versionStr);
        return stringTo<int>(removeFromString(versionSplit[0], '.'));
    }
    return 0;
}

} // namespace
