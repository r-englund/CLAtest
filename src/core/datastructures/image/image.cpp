/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include <inviwo/core/datastructures/image/image.h>
#include <inviwo/core/datastructures/image/imagedisk.h>
#include <inviwo/core/datastructures/image/imageram.h>

namespace inviwo {

Image::Image(uvec2 dimensions, ImageType comb, const DataFormatBase* format, bool allowMissingLayers) 
	: StructuredData<2>(dimensions)
	, imageType_(comb) 
	, allowMissingLayers_(allowMissingLayers) {
        Data::setDataFormat(format);
}

Image::Image(ImageRepresentation* in, bool allowMissingLayers)
    : StructuredData<2>(in->getDimensions())
    , imageType_(in->getImageType())
    , allowMissingLayers_(allowMissingLayers){
    Data::setDataFormat(in->getDataFormat());    
    clearRepresentations();
    addRepresentation(in);
}

Image::Image(const Image& rhs) : StructuredData<2>(rhs)
    , allowMissingLayers_(rhs.allowMissingLayers_)
    , imageType_(rhs.imageType_)
    , inputSources_(rhs.inputSources_) {}

Image* Image::clone() const {
    return new Image(*this);
}

Image::~Image() {
    // Representations are deleted by Data destructor.
}

void Image::resize(uvec2 dimensions) {
    setDimension(dimensions);
    if(lastValidRepresentation_) {
        // Resize last valid representation and remove the other ones
        static_cast<ImageRepresentation*>(lastValidRepresentation_)->resize(dimensions);
        std::vector<DataRepresentation*>::iterator it = std::find(representations_.begin(), representations_.end(), lastValidRepresentation_);
        // First delete the representations before erasing them from the vector
        for (size_t i=0; i<representations_.size(); i++) {
            if(representations_[i] != lastValidRepresentation_) {
                delete representations_[i]; representations_[i] = NULL;
            }
        }
        // Erasing representations: start from the back 
        if(it != --representations_.end()) {
            std::vector<DataRepresentation*>::iterator eraseFrom = it + 1;
            representations_.erase(eraseFrom, representations_.end()); 
        }
        // and then erase the ones infron of the valid representation
        if(representations_.begin() != it) {
            representations_.erase(representations_.begin(), it);
        }
         
    }
    setAllRepresentationsAsInvalid();
}


uvec2 Image::getDimension() const {
	return StructuredData<2>::getDimension();
}
void  Image::setDimension(const uvec2& dim){
	StructuredData<2>::setDimension(dim);
}

void Image::resizeImageRepresentations(Image* targetImage, uvec2 targetDim) {
    //TODO: check if getClassName() is necessary.
    //TODO: And also need to be tested on multiple representations_ such as ImageRAM, ImageDisk etc.,
    //TODO: optimize the code
    targetImage->resize(targetDim);
    ImageRepresentation* imageRepresentation = 0;
    ImageRepresentation* targetRepresentation = 0;
    std::vector<DataRepresentation*> &targetRepresentations = targetImage->representations_;

    if (targetRepresentations.size()) {
        for (int i=0; i<static_cast<int>(representations_.size()); i++) {
            imageRepresentation = dynamic_cast<ImageRepresentation*>(representations_[i]) ;        
            ivwAssert(imageRepresentation!=0, "Only image representations should be used here.");
            if (isRepresentationValid(i)) {
                int numberOfTargets = static_cast<int>(targetRepresentations.size());
                for (int j=0; j<numberOfTargets; j++) {
                    targetRepresentation = dynamic_cast<ImageRepresentation*>(targetRepresentations[j]) ;
                    ivwAssert(targetRepresentation!=0, "Only image representations should be used here.");
                    if (imageRepresentation->getClassName()==targetRepresentation->getClassName()) {
                        if (imageRepresentation->copyAndResizeImage(targetRepresentation)) {
                            targetImage->setRepresentationAsValid(j);
                            targetImage->lastValidRepresentation_ = targetRepresentations[j];
                        }
                    }
                }
            }
        }
    }
    else {
        ivwAssert(lastValidRepresentation_!=0, "Last valid representation is expected.");
        targetImage->setAllRepresentationsAsInvalid();
        targetImage->createDefaultRepresentation();
        ImageRepresentation* lastValidRepresentation = dynamic_cast<ImageRepresentation*>(lastValidRepresentation_);
        ImageRepresentation* cloneOfLastValidRepresentation = dynamic_cast<ImageRepresentation*>(lastValidRepresentation->clone());        
        targetImage->addRepresentation(cloneOfLastValidRepresentation);        

       targetImage->resize(targetDim);
       if (lastValidRepresentation->copyAndResizeImage(cloneOfLastValidRepresentation)) {
            targetImage->setRepresentationAsValid(static_cast<int>(targetImage->representations_.size())-1);
            targetImage->lastValidRepresentation_ = cloneOfLastValidRepresentation;
       }
    }
}

void Image::setInputSource(ImageLayerType layer, const Image* src) {
    inputSources_[layer] = src;
}

void Image::setAllowMissingLayers(bool allowMissingLayers) {
    allowMissingLayers_ = allowMissingLayers;
}

DataRepresentation* Image::createDefaultRepresentation() {
	return createImageRAM((uvec2)getDimension(), getImageType(), getDataFormat());
}

void Image::newRepresentationCreated() const {
    if (!allowMissingLayers_) {
        ImageRepresentation* lastValidRepresentation = dynamic_cast<ImageRepresentation*>(lastValidRepresentation_);
        ImageSourceMap::const_iterator it;
        if(lastValidRepresentation){
            if(!typeContainsColor(getImageType())){
                it = inputSources_.find(COLOR_LAYER);
                if(it != inputSources_.end())
                    lastValidRepresentation->useInputSource(COLOR_LAYER, it->second);
                else
                    lastValidRepresentation->createAndAddLayer(COLOR_LAYER);
            }
            if(!typeContainsDepth(getImageType())){
                it = inputSources_.find(DEPTH_LAYER);
                if(it != inputSources_.end())
                    lastValidRepresentation->useInputSource(DEPTH_LAYER, it->second);
                else
                    lastValidRepresentation->createAndAddLayer(DEPTH_LAYER);
            }
            if(!typeContainsPicking(getImageType())){
                it = inputSources_.find(PICKING_LAYER);
                if(it != inputSources_.end())
                    lastValidRepresentation->useInputSource(PICKING_LAYER, it->second);
                else
                    lastValidRepresentation->createAndAddLayer(PICKING_LAYER);
            }
        }
    }
}

} // namespace
