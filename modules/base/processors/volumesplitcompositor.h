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
    virtual Processor* create() const;

    void initialize();
    void deinitialize();

    virtual std::string getClassName() const { return "VolumeSplitCompositor"; }
    virtual std::string getCategory() const  { return "Network"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_EXPERIMENTAL; }

protected:
    virtual void process();

private:
    VolumePort inport_;
    VolumePort outport_;

    Volume* volume_;
};

}

#endif
