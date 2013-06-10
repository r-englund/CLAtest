#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/processors/canvasprocessor.h>

namespace inviwo {

// Image Inport
ImageInport::ImageInport(std::string identifier)
    : DataInport<Image>(identifier), dimensions_(uvec2(256,256))
{}

ImageInport::~ImageInport() {}

void ImageInport::initialize() {}

void ImageInport::deinitialize() {}

void ImageInport::changeDataDimensions(ResizeEvent* resizeEvent) {
    uvec2 dimensions = resizeEvent->size();

    //set dimension based on port groups
    std::vector<std::string> portGroups = getProcessor()->getPortDependencySets();
    std::vector<Port*> ports;
    uvec2 dimMax(0);
    bool hasImageOutport = false;

    for (size_t j=0; j<portGroups.size(); j++) {
        ports.clear();
        ports = getProcessor()->getPortsByDependencySet(portGroups[j]);

        if (std::find(ports.begin(), ports.end(), this) != ports.end()) {            
            for (size_t j=0; j<ports.size(); j++) {
                ImageOutport* imageOutport = dynamic_cast<ImageOutport*>(ports[j]);
                if (imageOutport) {  
                    hasImageOutport = true;
                    uvec2 dim = imageOutport->getDimensions();
                    //Largest outport dimension
                    if (dimMax.x*dimMax.y<dim.x*dim.y)
                        dimMax = imageOutport->getDimensions();
                }
            }
        }
    }

    if (!hasImageOutport)
        dimensions_ = dimensions;
    else    
        dimensions_ = dimMax;

    invalidate(PropertyOwner::INVALID_OUTPUT);
    resizeEvent->setSize(dimensions_);
    propagateResizeToPredecessor(resizeEvent);    
}

void ImageInport::propagateResizeToPredecessor(ResizeEvent* resizeEvent) {    
    ImageOutport* imageOutport = dynamic_cast<ImageOutport*>(getConnectedOutport());
    if (imageOutport)
        imageOutport->changeDataDimensions(resizeEvent);
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
            return const_cast<const Image*>(outport->getResizedImageData(dimensions_));            
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
    std::string dimensionString = glm::to_string(dimensions_);
    imageDataMap_.insert(std::make_pair(dimensionString, data_));    
    mapDataInvalid_ = true;
}

ImageOutport::~ImageOutport() {
    for (ImagePortMap::const_iterator it=imageDataMap_.begin(); it!=imageDataMap_.end(); ++it)
        delete it->second;
}

void ImageOutport::initialize() {}

void ImageOutport::deinitialize() {}

void ImageOutport::propagateResizeEventToPredecessor(ResizeEvent* resizeEvent) {    
    Processor* processor = getProcessor();
    std::vector<Inport*> inPorts = processor->getInports();
    for (size_t i=0; i<inPorts.size(); i++) {
        ImageInport* imageInport = dynamic_cast<ImageInport*>(inPorts[i]);
        if (imageInport) {
            imageInport->changeDataDimensions(resizeEvent);
        }
    }
    processor->invalidate(PropertyOwner::INVALID_OUTPUT);
    invalidate(PropertyOwner::INVALID_OUTPUT);
}

void ImageOutport::invalidate(PropertyOwner::InvalidationLevel invalidationLevel) {
    mapDataInvalid_ = true;
    Outport::invalidate(invalidationLevel);
}

void ImageOutport::changeDataDimensions(ResizeEvent* resizeEvent) {
    //Allocates space holder, sets largest data, cleans up un-used data 
    
    uvec2 requiredDimensions = resizeEvent->size();
    uvec2 previousDimensions = resizeEvent->previousSize();
    std::string prevDimensionString = glm::to_string(previousDimensions);
    std::string reqDimensionString = glm::to_string(requiredDimensions);

    std::vector<Processor*> directSuccessors;
    directSuccessors = getDirectSuccessors();

    std::vector<uvec2> registeredDimensions;
    for (size_t i=0; i<directSuccessors.size(); i++) {
        CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(directSuccessors[i]);
        if (canvasProcessor) {
            uvec2 dimensions = canvasProcessor->getCanvas()->size();
            registeredDimensions.push_back(dimensions);
        }
    }
    
    std::vector<std::string> registeredDimensionsStrings;    
    for (size_t i=0; i<registeredDimensions.size(); i++) {
        uvec2 dimensions = registeredDimensions[i];
        std::string dimensionString = glm::to_string(dimensions);        
        registeredDimensionsStrings.push_back(dimensionString);
    }    

    //If requiredDimension does not exist then do the following:
    //  If image data with previousDimensions exists in map and 
    //  also does not exist in validDimensions
    //      Resize map data to required dimension
    //  Else
    //      Clone the current data, resize it and make new entry in map

    Image* resultImage = 0;
    if ( imageDataMap_.find(reqDimensionString)!= imageDataMap_.end() )
        resultImage = imageDataMap_[reqDimensionString];

    //requiredDimension does not exist
    if (!resultImage) {
        //Decide whether to resize data with previousDimensions
        bool canResize = false;
        if (std::find(registeredDimensionsStrings.begin(), registeredDimensionsStrings.end(), prevDimensionString) == registeredDimensionsStrings.end())
            canResize = true;    

        //Does data with previousDimensions exist
        if ( imageDataMap_.find(prevDimensionString)!= imageDataMap_.end() )
            resultImage = imageDataMap_[prevDimensionString];

        if (canResize && resultImage) {
            //previousDimensions exist. It is no longer needed. So it can be resized.            
            //Remove old entry in map( later make new entry)
            imageDataMap_.erase(prevDimensionString);
        }
        else {
            //previousDimensions does not exist. So allocate space holder
            resultImage = dynamic_cast<Image*>(data_->clone());            
        }
        //Resize the result image
        resultImage->resize(requiredDimensions);
        //Make new entry
        imageDataMap_.insert(std::make_pair(reqDimensionString, resultImage));        
    }

    //Remove unwanted map data
   ImagePortMap::iterator it=imageDataMap_.begin();
   while (it!=imageDataMap_.end()) {
        if (std::find(registeredDimensionsStrings.begin(), registeredDimensionsStrings.end(), it->first) == registeredDimensionsStrings.end()) {
            //discard other data but leave atleast one data
            if (it->second && imageDataMap_.size()>1) {
                delete it->second;
                imageDataMap_.erase(it);
            }            
        }
        it++;
    }

    //Set largest data
    setLargestImageData();
    invalidate(PropertyOwner::INVALID_OUTPUT);

    //Propagate the resize event
    propagateResizeEventToPredecessor(resizeEvent);
}

uvec2 ImageOutport::getDimensions() const{ 
    return dimensions_; 
}

Image* ImageOutport::getResizedImageData(uvec2 requiredDimensions){

    if (mapDataInvalid_) {
        //Resize all map data once
        for (ImagePortMap::const_iterator it=imageDataMap_.begin(); it!=imageDataMap_.end(); ++it) {
            if (it->second != data_) { 
                uvec2 mapDataDimensions = it->second->getDimension();
                data_->resizeImageRepresentations(it->second, mapDataDimensions);
            }
        }
        mapDataInvalid_ = false;
    }

    //TODO: Map traverse is expensive. Optimize
    for (ImagePortMap::const_iterator it=imageDataMap_.begin(); it!=imageDataMap_.end(); ++it) {            
        if (it->second->getDimension() == requiredDimensions) {
            return it->second;
        }
    }

    //ivwAssert(1==0, "Required dimension " << requiredDimensions.x << " x " << requiredDimensions.y << "does not exist in outport.");    
    //FIXME: Should not enter this region. Port inspectors needs this.
    Image* resultImage = dynamic_cast<Image*>(data_->clone());
    resultImage->resize(requiredDimensions);
    std::string dimensionString = glm::to_string(requiredDimensions);
    imageDataMap_.insert(std::make_pair(dimensionString, resultImage));
    data_->resizeImageRepresentations(resultImage, requiredDimensions);
    return resultImage;
}

void ImageOutport::setLargestImageData() {
    uvec2 maxDimensions(0);
    Image* largestImage = 0;
    for (ImagePortMap::const_iterator it=imageDataMap_.begin(); it!=imageDataMap_.end(); ++it) {            
        uvec2 mapDataDimensions = it->second->getDimension();
        if ( (maxDimensions.x*maxDimensions.y)<(mapDataDimensions.x*mapDataDimensions.y) ) {
            maxDimensions = mapDataDimensions;
            largestImage = it->second;
        }
    }
    
    if (largestImage && data_!=largestImage) {
        data_ = largestImage;
        mapDataInvalid_ = true;
    }

    dimensions_ = data_->getDimension();
}

uvec3 ImageOutport::getColorCode() const { 
    return uvec3(90,127,183); 
}

} // namespace
