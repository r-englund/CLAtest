#ifndef IVW_VOLUMESOURCE_H
#define IVW_VOLUMESOURCE_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/volumeport.h>

namespace inviwo {

class IVW_MODULE_BASE_API VolumeSource : public Processor {
public:
    VolumeSource();
    ~VolumeSource();

    InviwoProcessorInfo();

protected:
    void loadVolume();

private:
    VolumeOutport volumePort_;
    FileProperty volumeFile_;
};

} // namespace

#endif // IVW_VOLUMESOURCE_H
