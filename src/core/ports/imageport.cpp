#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/processors/processor.h>

namespace inviwo {

ImagePort::ImagePort(PortDirection direction, std::string identifier)
    : DataPort<Image>(direction, identifier), dimensions_(uvec2(256,256))
{}

ImagePort::~ImagePort() {}

void ImagePort::initialize() {
    data_ = new Image(dimensions_);
}

void ImagePort::deinitialize() {
    delete data_;
}

void ImagePort::changeDimensions(uvec2 dimensions) {
    dimensions_ = dimensions;
    //TODO: data_ is sometimes un-initialized. Especially while loading workspace.
    if (data_) data_->resize(dimensions_);
    invalidate();
}

Image* ImagePort::resizeImageData(uvec2 dimensions) {
    ivwAssert(isOutport(), "This method should only be called for outports.");

    /*
    Image* result = dynamic_cast<Image*>(data_->clone());
    if (result) data_->resizeImageRepresentations(result, dimensions);
    return result;
    */

    //TODO: imageDataMap_ needs to be cleaned up, when dimensions or connections change
    Image* result = 0;
    std::ostringstream dimensionString;
    dimensionString << dimensions.x << "x" << dimensions.y;
    if (imageDataMap_.find(dimensionString.str()) == imageDataMap_.end()) {
        result = dynamic_cast<Image*>(data_->clone());
        if (result) imageDataMap_.insert(std::make_pair(dimensionString.str(), result)); 
    } else
        result = imageDataMap_[dimensionString.str()];

    //TODO: do the following resizing only if the entry in the map is invalid or the size has changed
    //if (!IS_STILL_VALID || result.getDimensions!=targetDim) { //FIXME        
        if (result) data_->resizeImageRepresentations(result, dimensions);
    //}
    return result;
}

Image* ImagePort::getData() {
    if (isOutport()) return data_;
    else if (isConnected()) {
        ImagePort* outport = dynamic_cast<ImagePort*>(connectedDataPort_);
        if (dimensions_==outport->getDimensions())
            return outport->getData();
        else
            return outport->resizeImageData(dimensions_);
    }
    else return 0;
}

} // namespace
