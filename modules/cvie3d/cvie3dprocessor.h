#ifndef IVW_CVIE3DPROCESSOR_H
#define IVW_CVIE3DPROCESSOR_H

#include <modules/cvie3d/cvie3dmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/processors/processor.h>

namespace inviwo {

class IVW_MODULE_CVIE3D_API CVIE3DProcessor : public Processor {
public:
    CVIE3DProcessor();
    ~CVIE3DProcessor();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();

private:
    VolumeInport inport_;
    VolumeOutport outport_;
};

} // namespace

#endif // IVW_CVIE3DPROCESSOR_H
