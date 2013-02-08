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

    OpenGLInfo();
    virtual ~OpenGLInfo();

    void printInfo();

    bool isExtensionSupported(const char*);
    bool isSupported(const char*);

protected:
    void retrieveStaticInfo();
    void retrieveDynamicInfo();

    void addShaderVersion(GLSLShaderVersion);
    void parseAndAddShaderVersion(std::string);

    static const std::string logSource_; ///< Source string to be displayed for log messages.

private:
    GLSLShaderVersion currentUsedGLSLVersion_;
    std::vector<GLSLShaderVersion> supportedShaderVersions_;
};

} // namespace

#endif // IVW_OPENGLINFO_H
