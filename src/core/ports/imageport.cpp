#include "inviwo/core/ports/imageport.h"
#include "../../inviwo/modules/opengl/imagegl.h"
#include "inviwo/core/processors/processor.h"

namespace inviwo {

ImagePort::ImagePort(PortDirection direction, std::string identifier)
    : dimensions_(ivec2(256,256)),
      DataPort<Image>(direction, identifier)
{}

ImagePort::~ImagePort() {}

void ImagePort::initialize() {
    data_ = new Image(dimensions_);
}

void ImagePort::deinitialize() {
    delete data_;
}

void ImagePort::resize(ivec2 dimensions) {
    dimensions_ = dimensions;
    data_->resize(dimensions_);
    invalidate();
}

Image* ImagePort::resizeImageData(std::string processorID, ivec2 targetDim) {
    ivwAssert(isOutport(), "This method should only be called for outports.");

    Image* result = 0;
    if (imageDataMap_.find(processorID) == imageDataMap_.end()) {
        result = new Image();
        imageDataMap_.insert(std::make_pair(processorID, result)); 
    } else
        result = imageDataMap_[processorID];

    //TODO: do the following resizing only if the entry in the map is invalid or the size has changed
    //if (!IS_STILL_VALID || result.getDimensions!=targetDim) { //FIXME
        //TODO: ImageGL module should not be accessed from here
        ImageGL* newImageGL = result->getRepresentation<ImageGL>();
        result->resize(targetDim);
        ImageGL* imageGL = data_->getRepresentation<ImageGL>();
        imageGL->blit(newImageGL);
    //}

    return result;
}

Image* ImagePort::getData() {
    if (isOutport()) return data_;
    else if (isConnected()) {
        ImagePort* outport = dynamic_cast<ImagePort*>(connectedDataPort_);
        if (dimensions_.x==outport->getDimensions().x && dimensions_.y==outport->getDimensions().y) //TODO: check if component wise comparsion support by ivec2
            return outport->getData();
        else
            return outport->resizeImageData(getProcessor()->getIdentifier(), dimensions_);
    }
    else return 0;
}

} // namespace
