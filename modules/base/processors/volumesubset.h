#ifndef IVW_VOLUMESUBSET_H
#define IVW_VOLUMESUBSET_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_MODULE_BASE_API VolumeSubset : public Processor {
public:
    VolumeSubset();
    ~VolumeSubset();
    
    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();

private:
    VolumeInport inport_;
    VolumeOutport outport_;

    BoolProperty enabled_;

    IntMinMaxProperty rangeX_;
    IntMinMaxProperty rangeY_;
    IntMinMaxProperty rangeZ_;

    uvec3 dims_;
};

}

#endif //IVW_VOLUMESUBSET_H
