#ifndef IVW_VOLUMESLICE_H
#define IVW_VOLUMESLICE_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/optionproperties.h>
#include <inviwo/core/properties/scalarproperties.h>
#include <inviwo/core/datastructures/volume/volumeramslice.h>

namespace inviwo {

class IVW_MODULE_BASE_API VolumeSlice : public Processor {
public:
    VolumeSlice();
    ~VolumeSlice();
    
    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();

private:
    VolumeInport inport_;
    ImageOutport outport_;

    TemplateOptionProperty<CoordinatePlane> coordinatePlane_;
    IntProperty sliceNumber_;
};

}

#endif //IVW_VOLUMESLICE_H
