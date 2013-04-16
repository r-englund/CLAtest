#include "imagesource.h"

namespace inviwo {

ImageSource::ImageSource()
    : Processor(),
    outport_(Port::OUTPORT, "image.outport"),
    imageFileName_("imageFileName", "Image file name", IVW_DIR+"data/images/swirl.tga")
{
    addPort(outport_);
    addProperty(imageFileName_);
}

ImageSource::~ImageSource() {}

Processor* ImageSource::create() const {
    return new ImageSource();
}

void ImageSource::initialize() {
    Processor::initialize();
}

void ImageSource::deinitialize() {
    Processor::deinitialize();
}

/**
* Creates a ImageDisk representation if there isn't an object already defined.
**/
void ImageSource::process() {
	Image* outImage = outport_.getEditableData(); 
    if(outImage){
        ImageDisk *outImageDisk_ = outImage->getEditableRepresentation<ImageDisk>();
        if(!outImageDisk_ || outImageDisk_->getSourceFile() != imageFileName_.get()){ 
            outImageDisk_ = new ImageDisk(imageFileName_.get());
        }
        outImage->clearRepresentations();
        outImage->addRepresentation(outImageDisk_);
    }
}

} // namespace
