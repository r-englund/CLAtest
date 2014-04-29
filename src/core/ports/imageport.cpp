/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file authors: Sathish Kottravel, Erik Sundén
 *
 *********************************************************************************/

#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/processors/canvasprocessor.h>

namespace inviwo {

uvec3 ImageInport::colorCode = uvec3(90,127,183);

// Image Inport
ImageInport::ImageInport(std::string identifier,
                         bool outportDeterminesSize,
                         PropertyOwner::InvalidationLevel invalidationLevel)
    : DataInport<Image>(identifier, invalidationLevel)
    , dimensions_(uvec2(256,256))
    , outportDeterminesSize_(outportDeterminesSize)
    , resizeScale_(vec2(1.f,1.f)) {
}

ImageInport::~ImageInport() {}

void ImageInport::initialize() {}

void ImageInport::deinitialize() {}

void ImageInport::changeDataDimensions(ResizeEvent* resizeEvent) {
    uvec2 dimensions = resizeEvent->size();
    //set dimension based on port groups
    std::vector<std::string> portDependencySets = getProcessor()->getPortDependencySets();
    std::vector<Port*> portSet;
    uvec2 dimMax(0);
    bool hasImageOutport = false;

    for (size_t i=0; i<portDependencySets.size(); i++) {
        portSet.clear();
        //get ports that belong to the dependency set portDependencySets[i]
        portSet = getProcessor()->getPortsByDependencySet(portDependencySets[i]);

        //check if current port belong to portSet
        if (std::find(portSet.begin(), portSet.end(), this) != portSet.end()) {
            //Find the image port with largest dimension
            for (size_t j=0; j<portSet.size(); j++) {
                ImageOutport* imageOutport = dynamic_cast<ImageOutport*>(portSet[j]);

                if (imageOutport && imageOutport->isConnected()) {
                    hasImageOutport = true;
                    uvec2 dim = imageOutport->getDimension();

                    //Largest outport dimension
                    if (dimMax.x*dimMax.y<dim.x*dim.y)
                        dimMax = imageOutport->getDimension();
                }
            }
        }
    }

    if (!hasImageOutport)
        dimensions_ = dimensions;
    else
        dimensions_ = dimMax;

    resizeEvent->setSize(dimensions_);
    propagateResizeToPredecessor(resizeEvent);
    invalidate(PropertyOwner::INVALID_OUTPUT);
}

void ImageInport::propagateResizeToPredecessor(ResizeEvent* resizeEvent) {
    if (equalColorCode(getConnectedOutport())) {
        ImageOutport* imageOutport = static_cast<ImageOutport*>(getConnectedOutport());
        imageOutport->changeDataDimensions(resizeEvent);
    }
}

void ImageInport::setResizeScale(vec2 scaling){
    resizeScale_ = scaling;
}

vec2 ImageInport::getResizeScale(){
    return resizeScale_;
}

uvec2 ImageInport::getDimension() const {
    if (isOutportDeterminingSize() && isConnected()) {
        ImageOutport* outport = dynamic_cast<ImageOutport*>(getConnectedOutport());
        return outport->getDimension();
    }
    else
        return dimensions_;
}

const Image* ImageInport::getData() const {
    if (isConnected()) {
        ImageOutport* outport = dynamic_cast<ImageOutport*>(getConnectedOutport());

        if (isOutportDeterminingSize() && getDimension()==outport->getDimension())
            return outport->getConstData();
        else
            return const_cast<const Image*>(outport->getResizedImageData(dimensions_));
    } else {
        return NULL;
    }
}

uvec3 ImageInport::getColorCode() const {
    return ImageInport::colorCode;
}

bool ImageInport::isOutportDeterminingSize() const {
    return outportDeterminesSize_;
}

void ImageInport::setOutportDeterminesSize(bool outportDeterminesSize) {
    outportDeterminesSize_ = outportDeterminesSize;
}

std::string ImageInport::getContentInfo() const {
    if(hasData())
        return getData()->getDataInfo();
    else
        return getClassName() + " has no data.";
}

// Image Outport
ImageOutport::ImageOutport(std::string identifier,
                           PropertyOwner::InvalidationLevel invalidationLevel,
                           bool handleResizeEvents)
    : DataOutport<Image>(identifier, invalidationLevel)
    , dimensions_(uvec2(256,256))
    , handleResizeEvents_(handleResizeEvents)
{
    Image* im = new Image(dimensions_);
    data_ = im;
    dataChanged();
    mapDataInvalid_ = true;
}

ImageOutport::ImageOutport(std::string identifier, 
                           ImageType type, 
                           const DataFormatBase* format,
                           PropertyOwner::InvalidationLevel invalidationLevel,
                           bool handleResizeEvents)
    : DataOutport<Image>(identifier, invalidationLevel)
    , dimensions_(uvec2(256,256))
    , handleResizeEvents_(handleResizeEvents)
{
    Image* im = new Image(dimensions_, type, format);
    data_ = im;
    dataChanged();
    mapDataInvalid_ = true;
}

ImageOutport::ImageOutport(std::string identifier,
                           ImageInport* src,
                           ImageType type,
                           PropertyOwner::InvalidationLevel invalidationLevel, 
                           bool handleResizeEvents)
    : DataOutport<Image>(identifier, invalidationLevel)
    , EventHandler(), dimensions_(uvec2(256,256))
    , handleResizeEvents_(handleResizeEvents)
{
    Image* im = new Image(dimensions_, type, DataVec4UINT8::get(), true);
    data_ = im;
    dataChanged();
    mapDataInvalid_ = true;
    inputSources_[COLOR_LAYER] = src;
    inputSources_[DEPTH_LAYER] = src;
    inputSources_[PICKING_LAYER] = src;
}

ImageOutport::~ImageOutport() {
    if (isDataOwner()) {
        for (ImagePortMap::iterator it=imageDataMap_.begin(); it!=imageDataMap_.end(); ++it)
            delete it->second;
    }

    data_ = NULL; //As data_ is referenced in imageDataMap_.
}

void ImageOutport::initialize() {}

void ImageOutport::deinitialize() {}

void ImageOutport::propagateResizeEventToPredecessor(ResizeEvent* resizeEvent) {
    if (!isHandlingResizeEvents()) {
        return;
    }
    Processor* processor = getProcessor();
    std::vector<Inport*> inPorts = processor->getInports();

    bool propagationEnded = true;
    uvec2 size = resizeEvent->size();
    uvec2 prevSize = resizeEvent->previousSize();
    for (size_t i=0; i<inPorts.size(); i++) {
        if (equalColorCode(inPorts[i])) {
            propagationEnded = false;
            ImageInport* imageInport = static_cast<ImageInport*>(inPorts[i]);
            imageInport->changeDataDimensions(scaleResizeEvent(imageInport, resizeEvent));
            resizeEvent->setSize(size);
            resizeEvent->setPreviousSize(prevSize);
        }
    }

    if(propagationEnded){
        processor->invalidate(PropertyOwner::INVALID_OUTPUT);
        //invalidate(PropertyOwner::INVALID_OUTPUT);
    }
}

void ImageOutport::invalidate(PropertyOwner::InvalidationLevel invalidationLevel) {
    mapDataInvalid_ = true;
    Outport::invalidate(invalidationLevel);
}

Image* ImageOutport::getData() {
    updateInputSources();
    return DataOutport<Image>::getData();
}

void ImageOutport::dataChanged() {
    imageDataMap_.clear();
    std::string dimensionString = glm::to_string(dimensions_);
    imageDataMap_.insert(std::make_pair(dimensionString, data_));
}

void ImageOutport::changeDataDimensions(ResizeEvent* resizeEvent) {
    if (!handleResizeEvents_) {
        return;
    }
    //This function should check which dimension request exists, by going through the successors and checking registeredDimensions.
    //We do only want to propagate when there is not a registeredDimension which is larger then the resizeevent size.
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

        if (canvasProcessor && canvasProcessor->getCanvas()) {
            uvec2 dimensions = canvasProcessor->getCanvas()->getDimension();
            registeredDimensions.push_back(dimensions);
        }
    }

    if(registeredDimensions.empty())
        return;

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

    if (imageDataMap_.find(reqDimensionString)!= imageDataMap_.end())
        resultImage = imageDataMap_[reqDimensionString];

    bool invalidPort = false;

    //requiredDimension does not exist
    if (!resultImage) {
        invalidPort = true;

        //Decide whether to resize data with previousDimensions
        bool canResize = false;

        if (std::find(registeredDimensionsStrings.begin(), registeredDimensionsStrings.end(),
                      prevDimensionString) == registeredDimensionsStrings.end())
            canResize = true;

        //Does data with previousDimensions exist
        if (imageDataMap_.find(prevDimensionString)!= imageDataMap_.end())
            resultImage = imageDataMap_[prevDimensionString];

        if (canResize && resultImage) {
            //previousDimensions exist. It is no longer needed. So it can be resized.
            //Remove old entry in map( later make new entry)
            imageDataMap_.erase(prevDimensionString);
        }
        else {
            //previousDimensions does not exist. So allocate space holder
            resultImage = static_cast<Image*>(data_->clone());
        }

        //Resize the result image
        resultImage->resize(requiredDimensions);
        //Make new entry
        imageDataMap_.insert(std::make_pair(reqDimensionString, resultImage));
    }

    //Remove unwanted map data
    std::vector<std::string> invalidImageDataStrings;

    for (ImagePortMap::iterator it=imageDataMap_.begin(); it!=imageDataMap_.end(); ++it) {
        if (std::find(registeredDimensionsStrings.begin(), registeredDimensionsStrings.end(), it->first) == registeredDimensionsStrings.end())
            invalidImageDataStrings.push_back(it->first);
    }

    //leave at least one data and discard others
    if (imageDataMap_.size()>1) {
        for (size_t i=0; i<invalidImageDataStrings.size(); i++) {
            Image* invalidImage = imageDataMap_[invalidImageDataStrings[i]];
            imageDataMap_.erase(invalidImageDataStrings[i]);
            delete invalidImage;
        }
    }

    uvec2 largestDim = getDimension();
    //Set largest data
    setLargestImageData(resizeEvent);

    //Stop resize propagation if outport change hasn't change.
    //Invalid to output new size
    if (largestDim != getDimension()){
        //Propagate the resize event
        propagateResizeEventToPredecessor(resizeEvent);
    }
    else if(invalidPort)
        invalidate(PropertyOwner::INVALID_OUTPUT);
}

uvec2 ImageOutport::getDimension() const {
    return dimensions_;
}

Image* ImageOutport::getResizedImageData(uvec2 requiredDimensions) {
    if (mapDataInvalid_) {
        //Resize all map data once
        for (ImagePortMap::iterator it=imageDataMap_.begin(); it!=imageDataMap_.end(); ++it) {
            if (it->second != data_) {
                uvec2 mapDataDimensions = it->second->getDimension();
                data_->resizeRepresentations(it->second, mapDataDimensions);
            }
        }

        mapDataInvalid_ = false;
    }

    //TODO: Map traverse is expensive. Optimize
    for (ImagePortMap::iterator it=imageDataMap_.begin(); it!=imageDataMap_.end(); ++it) {
        if (it->second->getDimension() == requiredDimensions)
            return it->second;
    }

    //Image* resultImage = new Image(requiredDimensions, data_->getImageType(), data_->getDataFormat());
    Image* resultImage = dynamic_cast<Image*>(data_->clone());
    resultImage->resize(requiredDimensions);
    std::string dimensionString = glm::to_string(requiredDimensions);
    imageDataMap_.insert(std::make_pair(dimensionString, resultImage));
    data_->resizeRepresentations(resultImage, requiredDimensions);
    return resultImage;
}

void ImageOutport::setLargestImageData(ResizeEvent* resizeEvent) {
    uvec2 maxDimensions(0);
    Image* largestImage = 0;

    for (ImagePortMap::iterator it=imageDataMap_.begin(); it!=imageDataMap_.end(); ++it) {
        uvec2 mapDataDimensions = it->second->getDimension();

        if ((maxDimensions.x*maxDimensions.y)<(mapDataDimensions.x*mapDataDimensions.y)) {
            maxDimensions = mapDataDimensions;
            largestImage = it->second;
        }
    }

    //Check if data_ is not longer largest image.
    if (largestImage && data_!=largestImage) {
        data_ = largestImage;
        mapDataInvalid_ = true;
    }

    //Send update to listeners
    if (data_->getDimension() != dimensions_)
        broadcast(resizeEvent);

    dimensions_ = data_->getDimension();
}

uvec3 ImageOutport::getColorCode() const {
    return ImageInport::colorCode;
}

bool ImageOutport::addResizeEventListener(EventListener* el) {
    return addEventListener(el);
}

bool ImageOutport::removeResizeEventListener(EventListener* el) {
    return removeEventListener(el);
}

void ImageOutport::setInputSource(LayerType layer, ImageInport* src) {
    inputSources_[layer] = src;
}

void ImageOutport::updateInputSources() {
    Image* im = static_cast<Image*>(data_);

    for (ImageInSourceMap::iterator it = inputSources_.begin(); it != inputSources_.end(); it++)
        im->setInputSource(it->first, it->second->getData());
}

void ImageOutport::setDimension(const uvec2& newDimension) {
    dimensions_ = newDimension;
    // Clear data
    dataChanged();
    // Set new dimension
    DataOutport<Image>::getData()->resize(newDimension);
}

ResizeEvent* ImageOutport::scaleResizeEvent(ImageInport* imageInport, ResizeEvent* sizeEvent){
    vec2 scale = imageInport->getResizeScale();
    sizeEvent->setPreviousSize(imageInport->getDimension());
    sizeEvent->setSize(uvec2(static_cast<unsigned int>(static_cast<float>(sizeEvent->size().x*scale.x)),
        static_cast<unsigned int>(static_cast<float>(sizeEvent->size().y*scale.y))));
    return sizeEvent;
}

} // namespace
