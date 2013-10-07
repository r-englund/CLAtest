#ifndef IVW_OPENGLINFO_H
#define IVW_OPENGLINFO_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/util/resourceinfo.h>
#include <modules/opengl/inviwoopengl.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API OpenGLInfo : public ResourceInfo {
public:
    class IVW_MODULE_OPENGL_API GLSLShaderVersion {
    public:
        GLSLShaderVersion();
        GLSLShaderVersion(int num);
        GLSLShaderVersion(int num, std::string pro);

        std::string getProfile();
        int getVersion();
        std::string getVersionAsString();
        std::string getVersionAndProfileAsString();

        bool hasProfile();

    private:
        int number_;
        std::string profile_;
    };

    enum GlVendor {
        NVIDIA,
        AMD,
        INTEL,
        UNKNOWN
    };

    OpenGLInfo();
    virtual ~OpenGLInfo();

    void printInfo();

    bool canAllocate(uint64_t dataSize, uint8_t percentageOfAvailableMemory = 100);
    uvec3 calculateOptimalBrickSize(uvec3 dimensions, size_t formatSizeInBytes, uint8_t percentageOfAvailableMemory = 100);

    bool isExtensionSupported(const char*);
    bool isSupported(const char*);

    bool isTexturesSupported();
    bool is3DTexturesSupported();
    bool isTextureArraysSupported();
    bool isFboSupported();
    bool isShadersSupported();
    bool isShadersSupportedARB();

    GLSLShaderVersion getCurrentShaderVersion();
    std::string getCurrentGlobalGLSLHeader();
    std::string getCurrentGlobalGLSLFragmentDefines();

    int getCurrentAvailableTextureMem() throw (Exception);
    int getTotalAvailableTextureMem() throw (Exception);

    int getMaxProgramLoopCount();
    int getMaxTexSize();
    int getMax3DTexSize();
    int getMaxArrayTexSize();
    int getMaxColorAttachments();
    int getNumTexUnits();

protected:
    void retrieveStaticInfo();
    void retrieveDynamicInfo();

    void rebuildGLSLHeader();
    void rebuildGLSLFragmentDefines();

    void addShaderVersion(GLSLShaderVersion);
    void addShaderVersionIfEqualOrLower(GLSLShaderVersion, int);
    void parseAndAddShaderVersion(std::string);
    int parseAndRetrieveShaderVersion(std::string);

private:
    GlVendor glVendor_;

    std::string glVendorStr_;
    std::string glRenderStr_;
    std::string glVersionStr_;
    std::string glslVersionStr_;

    //GLSL
    bool shadersAreSupported_;
    bool shadersAreSupportedARB_;
    int maxProgramLoopCount_;

    size_t currentGlobalGLSLVersionIdx_;
    std::string currentGlobalGLSLHeader_;
    std::string currentGlobalGLSLFragmentDefines_;
    std::string preferredGLSLProfile_;
    std::vector<GLSLShaderVersion> supportedShaderVersions_;

    //Texturing
    bool texSupported_;
    bool tex3DSupported_;
    bool texArraySupported_;
    bool fboSupported_;
    int maxTexSize_;
    int max3DTexSize_;
    int maxArrayTexSize_;
    int maxColorAttachments_;
    int numTexUnits_;
};

} // namespace

#endif // IVW_OPENGLINFO_H
