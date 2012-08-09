#ifndef IVW_OPENGLMODULE_H
#define IVW_OPENGLMODULE_H

#include "inviwo/core/inviwomodule.h"

namespace inviwo {

class OpenGLModule : public InviwoModule {

public:
    OpenGLModule();

    void initialize();
    void deinitialize();

};

} // namespace

#endif // IVW_OPENGLMODULE_H
