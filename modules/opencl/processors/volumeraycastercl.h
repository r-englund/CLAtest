#ifndef IVW_VOLUME_RAYCASTER_CL_H
#define IVW_VOLUME_RAYCASTER_CL_H

#include <modules/opencl/openclmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/properties.h>
#include <inviwo/core/properties/transferfunctionproperty.h>

#include <modules/opencl/inviwoopencl.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API VolumeRaycasterCL : public Processor {
public:
    VolumeRaycasterCL();
    ~VolumeRaycasterCL();
    virtual Processor* create() const;

    void initialize();
    void deinitialize();

    virtual std::string getClassName() const { return "VolumeRaycasterCL"; }
    virtual std::string getCategory() const  { return "Ray Caster"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_EXPERIMENTAL; }

protected:
    virtual void process();

private:
    VolumeInport volumePort_;
    ImageInport entryPort_;
    ImageInport exitPort_;
    ImageOutport outport_;

    FloatVec3Property lightSourcePos_;
    FloatProperty samplingRate_;
    TransferFunctionProperty transferFunction_;

    cl::Kernel* kernel_;
};

} // namespace

#endif // IVW_VOLUME_RAYCASTER_CL_H
