#ifndef IVW_OPENGLMODULE_H
#define IVW_OPENGLMODULE_H

#include "inviwo/core/inviwomodule.h"

namespace inviwo {

class OpenGLModule : public InviwoModule {

public:
    OpenGLModule();

    void initialize() throw (Exception);
    void deinitialize() throw (Exception);

};

} // namespace

#endif // IVW_OPENGLMODULE_H
