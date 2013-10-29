#ifndef IVW_IMAGESOURCESERIES_H
#define IVW_IMAGESOURCESERIES_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/datastructures/image/imagedisk.h>
#include <inviwo/core/properties/directoryproperty.h>

namespace inviwo {

class IVW_MODULE_BASE_API ImageSourceSeries : public Processor {
public:
    ImageSourceSeries();
    ~ImageSourceSeries();
    
    InviwoProcessorInfo();

    virtual void initialize();
    virtual void deinitialize();
    virtual void onFindFiles();

protected:
    virtual void process();

private:
    ImageOutport outport_;
    ButtonProperty findFilesButton_;
    DirectoryProperty imageFileDirectory_;
    IntProperty currentImageIndex_;
};

} // namespace

#endif // IVW_IMAGESOURCESERIES_H
