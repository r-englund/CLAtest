#ifndef IVW_OPENGLMODULE_H
#define IVW_OPENGLMODULE_H

#include "modules/opengl/openglmoduledefine.h"
#include "inviwo/core/inviwomodule.h"

namespace inviwo {

class IVW_MODULE_OPENGL_API OpenGLModule : public InviwoModule {

public:
    OpenGLModule();

    void initialize();
    void deinitialize();

};

} // namespace

#endif // IVW_OPENGLMODULE_H
