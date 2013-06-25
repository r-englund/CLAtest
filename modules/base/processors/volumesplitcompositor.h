#ifndef IVW_VOLUMESPLITCOMPOSITOR_H
#define IVW_VOLUMESPLITCOMPOSITOR_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/volumeport.h>
#include <modules/base/basemoduledefine.h>

namespace inviwo {

class IVW_MODULE_BASE_API VolumeSplitCompositor : public Processor {

public:
    VolumeSplitCompositor();
    ~VolumeSplitCompositor();
    
    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();

private:
    VolumeInport inport_;
    VolumeOutport outport_;

    Volume* volume_;
};

}

#endif
