#ifndef IVW_OPENCLMODULE_H
#define IVW_OPENCLMODULE_H

#include "inviwo/core/inviwomodule.h"

namespace inviwo {
/*
 * See inviwoopencl.h for usage.
 */ 
class OpenCLModule : public InviwoModule {

public:
    OpenCLModule();

    void initialize();
    void deinitialize();

};

} // namespace

#endif // IVW_OPENGLMODULE_H
