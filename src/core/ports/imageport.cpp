#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/processors/processor.h>

namespace inviwo {

// Image Inport
ImageInport::ImageInport(std::string identifier)
    : DataInport<Image>(identifier), dimensions_(uvec2(256,256))
{}

ImageInport::~ImageInport() {}

void ImageInport::initialize() {}

void ImageInport::deinitialize() {}

void ImageInport::changeDimensions(uvec2 dimensions) {
    dimensions_ = dimensions;
}

uvec2 ImageInport::getDimensions() const{ 
    return dimensions_; 
}

const Image* ImageInport::getData() const{
    if (isConnected()) {
        ImageOutport* outport = dynamic_cast<ImageOutport*>(dataOutport_);
        if (dimensions_==outport->getDimensions())
            return outport->getConstData();
        else
            return const_cast<const Image*>(outport->resizeImageData(dimensions_));
    }
    else 
        return NULL;
}

uvec3 ImageInport::getColorCode() const { 
    return uvec3(90,127,183); 
}

// Image Outport
ImageOutport::ImageOutport(std::string identifier)
    : DataOutport<Image>(identifier), dimensions_(uvec2(256,256))
{
    data_ = new Image(dimensions_);
}

ImageOutport::~ImageOutport() {
    delete data_;
}

void ImageOutport::initialize() {}

void ImageOutport::deinitialize() {}

void ImageOutport::changeDimensions(uvec2 dimensions) {
    dimensions_ = dimensions;
    //TODO: data_ is sometimes un-initialized. Especially while loading workspace.
    if (data_) 
        data_->resize(dimensions_);
    
    invalidate();
}

uvec2 ImageOutport::getDimensions() const{ 
    return dimensions_; 
}

Image* ImageOutport::resizeImageData(uvec2 dimensions){
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

uvec3 ImageOutport::getColorCode() const { 
    return uvec3(90,127,183); 
}

} // namespace
