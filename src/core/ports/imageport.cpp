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

std::vector<Processor*> ImageInport::getAscendantProcessors() {
    std::vector<Processor*> ascendantProcessors;
    getAscendantProcessorsUsingPortType<ImageInport>(ascendantProcessors);
    return ascendantProcessors;
}

// Image Outport
ImageOutport::ImageOutport(std::string identifier)
    : DataOutport<Image>(identifier), dimensions_(uvec2(256,256))
{     
    data_ = new Image(dimensions_);
    std::ostringstream dimensionString;
    dimensionString << dimensions_.x << "x" << dimensions_.y;
    imageDataMap_.insert(std::make_pair(dimensionString.str(), data_));
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

void ImageOutport::changeDataDimensions(ResizeEvent* resizeEvent) {
    //Allocates space holder, sets largest data, cleans up un-used data 
    
    uvec2 requiredDimensions = resizeEvent->size();
    uvec2 previousDimensions = resizeEvent->previousSize();
    std::ostringstream prevDimensionString;
    prevDimensionString << previousDimensions.x << "x" << previousDimensions.y;
    std::ostringstream reqDimensionString;
    reqDimensionString << requiredDimensions.x << "x" << requiredDimensions.y;

    std::vector<uvec2> registeredDimensions = resizeEvent->getRegisteredCanvasSizes();
    std::vector<std::string> registeredDimensionsStrings;    
    for (size_t i=0; i<registeredDimensions.size(); i++) {
        uvec2 dimensions = registeredDimensions[i];
        std::ostringstream dimensionString;
        dimensionString << dimensions.x << "x" << dimensions.y;        
        registeredDimensionsStrings.push_back(dimensionString.str());
    }
 
    

    //If requiredDimension does not exist then do the following:
    //  If image data with previousDimensions exists in map and 
    //  also does not exist in validDimensions
    //      Resize map data to required dimension
    //  Else
    //      Clone the current data, resize it and make new entry in map

    Image* resultImage = 0;
    if ( imageDataMap_.find(reqDimensionString.str())!= imageDataMap_.end() )
        resultImage = imageDataMap_[reqDimensionString.str()];

    //requiredDimension does not exist
    if (!resultImage) {
        //Decide whether to resize data with previousDimensions
        bool canResize = false;
        if (std::find(registeredDimensionsStrings.begin(), registeredDimensionsStrings.end(), prevDimensionString.str()) == registeredDimensionsStrings.end())
            canResize = true;    

        //Does data with previousDimensions exist
        if ( imageDataMap_.find(prevDimensionString.str())!= imageDataMap_.end() )
            resultImage = imageDataMap_[prevDimensionString.str()];

        if (canResize && resultImage) {
            //previousDimensions exist. It is no longer needed. So it can be resized.            
            //Remove old entry in map( later make new entry)
            imageDataMap_.erase(prevDimensionString.str());
        }
        else {
            //previousDimensions does not exist. So allocate space holder
            resultImage = dynamic_cast<Image*>(data_->clone());            
        }
        //Resize the result image
        resultImage->resize(requiredDimensions);
        //Make new entry
        imageDataMap_.insert(std::make_pair(reqDimensionString.str(), resultImage));
    }

    //Remove unwanted map data
    for (ImagePortMap::const_iterator it=imageDataMap_.begin(); it!=imageDataMap_.end(); ++it) {
        if (std::find(registeredDimensionsStrings.begin(), registeredDimensionsStrings.end(), it->first) == registeredDimensionsStrings.end()) {
            //discard other data
            if (it->second) delete it->second;
            imageDataMap_.erase(it->first);
            it=imageDataMap_.begin();
        }
    }

    //Set largest data
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
    Image* largestImage = 0;
    for (ImagePortMap::const_iterator it=imageDataMap_.begin(); it!=imageDataMap_.end(); ++it) {            
        uvec2 mapDataDimensions = it->second->size();
        if ( (maxDimensions.x*maxDimensions.y)<(mapDataDimensions.x*mapDataDimensions.y) ) {
            maxDimensions = mapDataDimensions;
            largestImage = it->second;
        }
    }
    
    if (largestImage && data_!=largestImage)
        data_ = largestImage;

    dimensions_ = data_->size();
}

uvec3 ImageOutport::getColorCode() const { 
    return uvec3(90,127,183); 
}

std::vector<Processor*> ImageOutport::getDescendantProcessors() {
    std::vector<Processor*> descendantProcessors;
    getDescendantProcessorsUsingPortType<ImageOutport>(descendantProcessors);
    return descendantProcessors;
}

} // namespace
