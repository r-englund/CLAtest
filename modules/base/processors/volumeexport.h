#ifndef IVW_VOLUMEEXPORT_H
#define IVW_VOLUMEEXPORT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/volumeport.h>

namespace inviwo {

class IVW_CORE_API VolumeExport : public Processor {
public:
    VolumeExport();
    ~VolumeExport();

    InviwoProcessorInfo();

    virtual void initialize();
    virtual void deinitialize();
    void exportVolume();

protected:
    virtual void process();

private:
    VolumeInport volumePort_;
    FileProperty volumeFileName_;
    ButtonProperty exportVolumeButton_;
};

} // namespace

#endif // IVW_VOLUMEEXPORT_H
