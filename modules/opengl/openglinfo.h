#ifndef IVW_OPENGLINFO_H
#define IVW_OPENGLINFO_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/util/resourceinfo.h>
#include "inviwoopengl.h"

namespace inviwo {

class IVW_MODULE_OPENGL_API OpenGLInfo : public ResourceInfo {
public:
    class IVW_MODULE_OPENGL_API GLSLShaderVersion {
    public:
        GLSLShaderVersion() : number_(0), profile_("") {}
        GLSLShaderVersion(int num) : number_(num), profile_("") {}
        GLSLShaderVersion(int num, std::string pro) : number_(num), profile_(pro) {}

        int getVersion() { return number_; }
        std::string getProfile() { return profile_; }

        bool hasProfile() { return (profile_ != ""); }

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

    bool isExtensionSupported(const char*);
    bool isSupported(const char*);

    bool isTexturesSupported();
    bool is3DTexturesSupported();
    bool isTextureArraysSupported();
    bool isFboSupported();
    bool isShadersSupported();
    bool isShadersSupportedARB();

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

    void addShaderVersion(GLSLShaderVersion);
    void parseAndAddShaderVersion(std::string);

    static const std::string logSource_; ///< Source string to be displayed for log messages.

private:
    GlVendor glVendor_;

    //GLSL
    bool shadersAreSupported_;
    bool shadersAreSupportedARB_;
    int maxProgramLoopCount_;
    GLSLShaderVersion currentUsedGLSLVersion_;
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
