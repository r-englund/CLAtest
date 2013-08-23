#ifndef IVW_VOLUMESOURCE_H
#define IVW_VOLUMESOURCE_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/volumeport.h>

namespace inviwo {

class IVW_CORE_API VolumeSource : public Processor {
public:
    VolumeSource();
    ~VolumeSource();

    InviwoProcessorInfo();

    virtual void initialize();
    virtual void deinitialize();

protected:
    virtual void process();

    void loadVolume(const std::string& filename);

private:
    VolumeOutport volumePort_;
    FileProperty volumeFileName_;
};

} // namespace

#endif // IVW_VOLUMESOURCE_H
