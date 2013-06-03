#ifndef IVW_IMAGESOURCE_H
#define IVW_IMAGESOURCE_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/datastructures/image/imagedisk.h>

namespace inviwo {

class IVW_MODULE_BASE_API ImageSource : public Processor {
public:
    ImageSource();
    ~ImageSource();
    
    InviwoProcessorInfo();

    virtual void initialize();
    virtual void deinitialize();

protected:
    virtual void process();

private:
    ImageOutport outport_;
    FileProperty imageFileName_;
};

} // namespace

#endif // IVW_IMAGESOURCE_H
