#ifndef IVW_IMAGESOURCE_H
#define IVW_IMAGESOURCE_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/datastructures/imagedisk.h>

namespace inviwo {

class IVW_MODULE_BASE_API ImageSource : public Processor {
public:
    ImageSource();
    ~ImageSource();
    virtual Processor* create() const;

    virtual void initialize();
    virtual void deinitialize();

    virtual std::string getClassName() const { return "ImageSource"; }
    virtual std::string getCategory() const  { return "Data Source Processor"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_EXPERIMENTAL; }

protected:
    virtual void process();

private:
    ImagePort outport_;
    FileProperty imageFileName_;
};

} // namespace

#endif // IVW_IMAGESOURCE_H
