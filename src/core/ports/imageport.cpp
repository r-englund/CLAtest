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
    //TODO: a. Default data is un-neccessary
    //      b. Also getProcessor() not working since processor 
    //         for this port is not yet set in the constructor
    data_ = new Image(dimensions_);
    imageDataMap_.insert(std::make_pair(getProcessor(), data_));
}

ImageOutport::~ImageOutport() {
    for (ImagePortMap::const_iterator it=imageDataMap_.begin(); it!=imageDataMap_.end(); ++it)
        delete it->second;    
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

void ImageOutport::changeDataDimensions(uvec2 requiredDimensions, Processor* eventInitiatorProcessor) {

    Image* resultImage = 0;  

    std::string processorName = getProcessor()->getClassName();
    std::string portName = getIdentifier();

    if (imageDataMap_.find(eventInitiatorProcessor) == imageDataMap_.end()) {        
        for (ImagePortMap::const_iterator it=imageDataMap_.begin(); it!=imageDataMap_.end(); ++it) {            
            uvec2 mapDataDimensions = it->second->size();
            if (mapDataDimensions == requiredDimensions) {
                resultImage = it->second;
            }
        }

        if (!resultImage)
            resultImage = dynamic_cast<Image*>(data_->clone());

        // This is okay, because multiple eventInitiatorProcessor 
        // can be associated with single resultImage.
        // Also updates existing associations

        // TODO: Cleanup the map
        imageDataMap_.insert(std::make_pair(eventInitiatorProcessor, resultImage));
        
    } else 
        resultImage = imageDataMap_[eventInitiatorProcessor];

    if (resultImage->size()!=requiredDimensions)
        resultImage->resize(requiredDimensions);

    setLargestImageData();
    invalidate();
}

uvec2 ImageOutport::getDimensions() const{ 
    return dimensions_; 
}

Image* ImageOutport::resizeImageData(uvec2 requiredDimensions){
    Image* resultImage = 0;
    // TODO: Map travese is expensive. Optimize
    for (ImagePortMap::const_iterator it=imageDataMap_.begin(); it!=imageDataMap_.end(); ++it) {            
        uvec2 mapDataDimensions = it->second->size();
        if (mapDataDimensions == requiredDimensions) {
            resultImage = it->second;
        }
    }
     
    if (resultImage && resultImage!=data_)
        data_->resizeImageRepresentations(resultImage, requiredDimensions);

    return resultImage;
}

void ImageOutport::setLargestImageData() {
    uvec2 maxDimensions(0);
    Image* defaultImage = 0;
    for (ImagePortMap::const_iterator it=imageDataMap_.begin(); it!=imageDataMap_.end(); ++it) {            
        uvec2 mapDataDimensions = it->second->size();
        if ( (maxDimensions.x*maxDimensions.y)<(mapDataDimensions.x*mapDataDimensions.y) ) {
            maxDimensions = mapDataDimensions;
            defaultImage = it->second;
        }
    }
    
    if (defaultImage && data_!=defaultImage)
        data_ = defaultImage;

    dimensions_ = data_->size();    
}

uvec3 ImageOutport::getColorCode() const { 
    return uvec3(90,127,183); 
}

} // namespace
