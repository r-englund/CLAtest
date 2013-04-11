#include "imagesource.h"

namespace inviwo {

ImageSource::ImageSource()
    : ProcessorGL(),
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
    ProcessorGL::initialize();
	Image* image = outport_.getData();
    if (image) {
        image->clearRepresentations();
    }
}

void ImageSource::deinitialize() {
    ProcessorGL::deinitialize();
}

/**
* Creates a ImageDisk representation if there isn't an object already defined.
**/
void ImageSource::process() {

	Image* outImage = outport_.getData();
    
    if(outImage){
        ImageDisk *outImageDisk_ = outImage->getRepresentation<ImageDisk>();
        if(!outImageDisk_ || outImageDisk_->getSourceFile() != imageFileName_.get()){ 
            outImageDisk_ = new ImageDisk(imageFileName_.get());
        }
        outImage->clearRepresentations();
        outImage->addRepresentation(outImageDisk_);


    }
}

} // namespace
