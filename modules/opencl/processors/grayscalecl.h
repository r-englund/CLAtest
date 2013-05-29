#ifndef IVW_GRAYSCALE_CL_H
#define IVW_GRAYSCALE_CL_H

#include <modules/opencl/openclmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/processors/processor.h>



#include <modules/opencl/inviwoopencl.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API GrayscaleCL : public Processor {
public:
    GrayscaleCL();
    ~GrayscaleCL();
    
    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();

private:
    ImageInport inputPort_;
    ImageOutport outport_;

    cl::Kernel* kernel_;
};

} // namespace

#endif // IVW_GRAYSCALE_CL_H
