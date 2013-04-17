#ifndef IVW_OPENGLMODULE_H
#define IVW_OPENGLMODULE_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwomodule.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API OpenGLModule : public InviwoModule {

public:
    OpenGLModule();

protected:
    void setupGlobalSystemSettings();

};

} // namespace

#endif // IVW_OPENGLMODULE_H
