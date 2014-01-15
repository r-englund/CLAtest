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
#include <inviwo/core/datastructures/image/imageram.h>

namespace inviwo {

Image::Image(uvec2 dimensions, ImageType comb, const DataFormatBase* format, bool allowMissingLayers)
	: DataGroup(), StructuredGridMetaData<2>(dimensions)
    , allowMissingLayers_(allowMissingLayers)
	, imageType_(comb) {

    initialize(format);
}

Image::Image(const Image& rhs) 
    : DataGroup(rhs)
    , StructuredGridMetaData<2>(rhs)
    , allowMissingLayers_(rhs.allowMissingLayers_)
    , imageType_(rhs.imageType_)
    , inputSources_(rhs.inputSources_) {

    for (std::vector<Layer*>::const_iterator it = rhs.colorLayers_.begin() ; it != rhs.colorLayers_.end(); ++it)
        addColorLayer((*it)->clone());

    if(rhs.depthLayer_){
        depthLayer_ = rhs.depthLayer_->clone();
        addLayer(depthLayer_);
    } else {
        depthLayer_ = NULL;
    }

    if(rhs.pickingLayer_){
        pickingLayer_ = rhs.pickingLayer_->clone();
        addLayer(pickingLayer_);
    } else {
        pickingLayer_ = NULL;
    }
}

Image& Image::operator=(const Image& that) {
    if(this != &that) {
        DataGroup::operator=(that);
        StructuredGridMetaData<2>::operator=(that);

        allowMissingLayers_ = that.allowMissingLayers_;
        imageType_ = that.imageType_;
        inputSources_ = that.inputSources_;

        deinitialize();

        for(std::vector<Layer*>::const_iterator it = that.colorLayers_.begin(); it != that.colorLayers_.end(); ++it)
            addColorLayer((*it)->clone());

        if(that.depthLayer_) {
            depthLayer_ = that.depthLayer_->clone();
            addLayer(depthLayer_);
        } else {
            depthLayer_ = NULL;
        }

        if(that.pickingLayer_) {
            pickingLayer_ = that.pickingLayer_->clone();
            addLayer(pickingLayer_);
        } else {
            pickingLayer_ = NULL;
        }

    }
    return *this;
}

Image* Image::clone() const {
    return new Image(*this);
}

Image::~Image() {
    //Delete all layers
    deinitialize();

}

void Image::deinitialize() {
    for(std::vector<Layer*>::iterator it = colorLayers_.begin(); it != colorLayers_.end(); ++it)
        delete (*it);

    delete depthLayer_;
    delete pickingLayer_;
}

void Image::initialize(const DataFormatBase* format){
    addColorLayer(new Layer(getDimension(), format));

    depthLayer_ = NULL;
    pickingLayer_ = NULL;
    if (!allowMissingLayers_ || typeContainsDepth(getImageType())){
        depthLayer_ = new Layer(getDimension(), DataFLOAT32::get(), DEPTH_LAYER);
        addLayer(depthLayer_);
    }
    else
        depthLayer_ = NULL;

    if (!allowMissingLayers_ || typeContainsPicking(getImageType())){
        pickingLayer_ = new Layer(getDimension(), format);
        addLayer(pickingLayer_);
    }
    else
        pickingLayer_ = NULL;
}

uvec2 Image::getDimension() const {
	return StructuredGridMetaData<2>::getDimension();
}

void Image::setDimension(const uvec2& dim){
	StructuredGridMetaData<2>::setDimension(dim);
}

size_t Image::addColorLayer(Layer* layer){
    colorLayers_.push_back(layer);
    addLayer(layer);

    //Return index to this layer
    return colorLayers_.size()-1;
}

const std::vector<const Layer*>* Image::getAllLayers() const{
    return &allLayersConst_;
}

const std::vector<Layer*>* Image::getAllLayers(){
    return &allLayers_;
}

const Layer* Image::getLayer(LayerType type) const{
    switch (type){
    case COLOR_LAYER:
    	return getColorLayer();
    case DEPTH_LAYER:
        return getDepthLayer();
    case PICKING_LAYER:
        return getPickingLayer();
    }
    return NULL;
}

Layer* Image::getLayer(LayerType type){
    switch (type){
    case COLOR_LAYER:
        return getColorLayer();
    case DEPTH_LAYER:
        return getDepthLayer();
    case PICKING_LAYER:
        return getPickingLayer();
    }
    return NULL;
}

const Layer* Image::getColorLayer(size_t idx) const{
    return colorLayers_[idx];
}

Layer* Image::getColorLayer(size_t idx){
    return colorLayers_[idx];
}

size_t Image::getNumberOfColorLayers() const{
    return colorLayers_.size();
}

const Layer* Image::getDepthLayer() const{
    ImageSourceMap::const_iterator it = inputSources_.find(DEPTH_LAYER);
    if(it != inputSources_.end())
        return it->second->getDepthLayer();

    return depthLayer_;
}

Layer* Image::getDepthLayer(){
    return depthLayer_;
}

const Layer* Image::getPickingLayer() const{
    ImageSourceMap::const_iterator it = inputSources_.find(PICKING_LAYER);
    if(it != inputSources_.end())
        return it->second->getPickingLayer();

    return pickingLayer_;
}

Layer* Image::getPickingLayer(){
    return pickingLayer_;
}

void Image::resize(uvec2 dimensions) {
    setDimension(dimensions);

    //Resize all layers
    for (std::vector<Layer*>::iterator it = colorLayers_.begin() ; it != colorLayers_.end(); ++it)
        (*it)->resize(dimensions);

    if(depthLayer_)
        depthLayer_->resize(dimensions);

    if(pickingLayer_)
        pickingLayer_->resize(dimensions);
}

void Image::resizeRepresentations(Image* targetImage, uvec2 targetDim) {
    targetImage->resize(targetDim);
    std::vector<DataGroupRepresentation*> &targetRepresentations = targetImage->representations_;

    if (targetRepresentations.size()) {
        //Avoid resize of ImageRAM and ImageDisk if we have another representation
        bool existsMoreThenDiskAndRAMRepresentation = false;
        for (size_t j=0; j<targetRepresentations.size(); j++) {
            if(targetRepresentations[j]->getClassName() != "ImageRAM" && targetRepresentations[j]->getClassName() != "ImageDisk")
                existsMoreThenDiskAndRAMRepresentation = true;
        }

        ImageRepresentation* sourceImageRepresentation = 0;
        ImageRepresentation* targetImageRepresentation = 0;
        for (size_t i=0; i<representations_.size(); i++) {
            for (size_t j=0; j<targetRepresentations.size(); j++) {
                if (representations_[i]->getClassName()==targetRepresentations[j]->getClassName()) {
                    if(!existsMoreThenDiskAndRAMRepresentation || (targetRepresentations[j]->getClassName() != "ImageRAM" && targetRepresentations[j]->getClassName() != "ImageDisk")){
                        sourceImageRepresentation = static_cast<ImageRepresentation*>(representations_[i]);
                        sourceImageRepresentation->update(false);
                        targetImageRepresentation = static_cast<ImageRepresentation*>(targetRepresentations[j]);
                        targetImageRepresentation->update(false);
                        sourceImageRepresentation->copyAndResizeRepresentation(targetImageRepresentation);
                    }
                }
            }
        }
    }
    else {
        //If not representation exist, create ImageRAM one
        const ImageRAM* imageRAM = this->getRepresentation<ImageRAM>();
        imageRAM->copyAndResizeRepresentation(targetImage->getEditableRepresentation<ImageRAM>());
    }
}

ImageType Image::getImageType() const {
    return imageType_;
}

const DataFormatBase* Image::getDataFormat() const {
    return getColorLayer()->getDataFormat();
}

void Image::setInputSource(LayerType layer, const Image* src) {
    inputSources_[layer] = src;
}

void Image::addLayer(Layer* layer){
    allLayers_.push_back(layer);
    allLayersConst_.push_back(static_cast<const Layer*>(layer));
}





} // namespace
