#ifndef IVW_OPENGLINFO_H
#define IVW_OPENGLINFO_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/util/resourceinfo.h>
#include "inviwoopengl.h"

namespace inviwo {

class IVW_MODULE_OPENGL_API OpenGLInfo : public ResourceInfo {

public:
    OpenGLInfo();
    virtual ~OpenGLInfo();

    void printInfo();

protected:
    void retrieveStaticInfo();
    void retrieveDynamicInfo();

    static const std::string logSource_; ///< Source string to be displayed for log messages.
};

} // namespace

#endif // IVW_OPENGLINFO_H
