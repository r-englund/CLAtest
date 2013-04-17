#ifndef IVW_OPENCLMODULE_H
#define IVW_OPENCLMODULE_H

#include <inviwo/core/common/inviwomodule.h>
#include <modules/opencl/openclmoduledefine.h>
namespace inviwo {
/*
 * See inviwoopencl.h for usage.
 */ 
class IVW_MODULE_OPENCL_API OpenCLModule : public InviwoModule {

public:
    OpenCLModule();

    void initialize();
    void deinitialize();

};

} // namespace

#endif // IVW_OPENGLMODULE_H
