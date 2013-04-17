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
    virtual Processor* create() const;

    virtual void initialize();
    virtual void deinitialize();

    virtual std::string getClassName() const { return "VolumeSource"; }
    virtual std::string getCategory() const  { return "Data Source Processor"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_EXPERIMENTAL; }

protected:
    virtual void process();

private:
    VolumePort volumePort_;
    FileProperty volumeFileName_;
};

} // namespace

#endif // IVW_VOLUMESOURCE_H
