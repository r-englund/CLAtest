#ifndef IVW_VOLUMEBASISTRANSFORMER_H
#define IVW_VOLUMEBASISTRANSFORMER_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/volumeport.h>
#include <modules/base/basemoduledefine.h>

namespace inviwo {

class IVW_MODULE_BASE_API VolumeBasisTransformer : public Processor {

public:
    VolumeBasisTransformer();
    ~VolumeBasisTransformer();
    
    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();

private:
    VolumeInport inport_;
    VolumeOutport outport_;

    FloatVec3Property lengths_;
    FloatVec3Property angels_;
    FloatVec3Property offset_;

    mat4 orgBasisAndOffset_;
};

}

#endif
