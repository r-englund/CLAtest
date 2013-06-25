#include "imagesource.h"

namespace inviwo {

ProcessorClassName(ImageSource, "ImageSource"); 
ProcessorCategory(ImageSource, "Data Input");
ProcessorCodeState(ImageSource, CODE_STATE_EXPERIMENTAL);

ImageSource::ImageSource()
    : Processor(),
    outport_("image.outport"),
    imageFileName_("imageFileName", "Image file name", IVW_DIR+"data/images/swirl.tga")
{
    addPort(outport_);
    addProperty(imageFileName_);
}

ImageSource::~ImageSource() {}

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
	Image* outImage = outport_.getData(); 
    if (outImage){
        ImageDisk* outImageDisk = outImage->getEditableRepresentation<ImageDisk>();
        if (!outImageDisk || outImageDisk->getSourceFile() != imageFileName_.get()){ 
            outImageDisk = new ImageDisk(imageFileName_.get());
        }
        outImage->clearRepresentations();
        outImage->addRepresentation(outImageDisk);
    }
}

} // namespace
