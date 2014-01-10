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

#include <inviwo/core/datastructures/image/imageram.h>
#include <inviwo/core/datastructures/image/layerramprecision.h>
#include <inviwo/core/datastructures/image/layerram.h>
#include <inviwo/core/io/imageio.h>

namespace inviwo {

ImageRAM::ImageRAM()
    : ImageRepresentation(){
    ImageRAM::initialize();
}

ImageRAM::ImageRAM(const ImageRAM& rhs) 
    : ImageRepresentation(rhs){
    update(true);
}

DataRepresentation* ImageRAM::clone() const{
    return new ImageRAM(*this);
}

ImageRAM::~ImageRAM() {
    ImageRAM::deinitialize();
}  

void ImageRAM::initialize() {
}

void ImageRAM::deinitialize() {
}

std::string ImageRAM::getClassName() const { 
    return "ImageRAM"; 
}

bool ImageRAM::copyAndResizeImageRepresentation(ImageRepresentation* targetImageRam) const {
    const ImageRAM* source = this;
    ImageRAM* target = dynamic_cast<ImageRAM*>(targetImageRam);
    ivwAssert(target!=0, "Target representation missing.");

    //Copy and resize color layers
    size_t minSize = std::min(source->getOwner()->getNumberOfColorLayers(), target->getOwner()->getNumberOfColorLayers());
    for (size_t i=0; i<minSize; ++i) {
        if(!source->getColorLayerRAM(i)->copyAndResizeLayer(target->getColorLayerRAM(i)))
            return false;
    }

    //Copy and resize depth layer
    if(source->getDepthLayerRAM() && target->getDepthLayerRAM())
        if(!source->getDepthLayerRAM()->copyAndResizeLayer(target->getDepthLayerRAM()))
            return false;

    //Copy and resize picking layer
    if(source->getPickingLayerRAM() && target->getPickingLayerRAM())
        if(!source->getPickingLayerRAM()->copyAndResizeLayer(target->getPickingLayerRAM()))
            return false;

    return true;
}

void ImageRAM::update(bool editable) {
    colorLayersRAM_.clear();
    depthLayerRAM_ = NULL;
    pickingLayerRAM_ = NULL;
    if(editable){
        for (size_t i=0; i<owner_->getNumberOfColorLayers(); ++i) {
            colorLayersRAM_.push_back(owner_->getColorLayer(i)->getEditableRepresentation<LayerRAM>());
        }

        Layer* depthLayer = owner_->getDepthLayer();
        if(depthLayer)
            depthLayerRAM_ = depthLayer->getEditableRepresentation<LayerRAM>();

        Layer* pickingLayer = owner_->getPickingLayer();
        if(pickingLayer)
            pickingLayerRAM_ = pickingLayer->getEditableRepresentation<LayerRAM>();
    }
    else{
        for (size_t i=0; i<owner_->getNumberOfColorLayers(); ++i) {
            colorLayersRAM_.push_back(const_cast<LayerRAM*>(owner_->getColorLayer(i)->getRepresentation<LayerRAM>()));
        }

        Layer* depthLayer = owner_->getDepthLayer();
        if(depthLayer)
            depthLayerRAM_ = const_cast<LayerRAM*>(depthLayer->getRepresentation<LayerRAM>());

        Layer* pickingLayer = owner_->getPickingLayer();
        if(pickingLayer)
            pickingLayerRAM_ = const_cast<LayerRAM*>(pickingLayer->getRepresentation<LayerRAM>());
    }
}

LayerRAM* ImageRAM::getColorLayerRAM(size_t idx) {
    return colorLayersRAM_.at(idx);
}

LayerRAM* ImageRAM::getDepthLayerRAM() {
    return depthLayerRAM_;
}

LayerRAM* ImageRAM::getPickingLayerRAM() {
    return pickingLayerRAM_;
}

const LayerRAM* ImageRAM::getColorLayerRAM(size_t idx) const {
    return colorLayersRAM_.at(idx);
}

const LayerRAM* ImageRAM::getDepthLayerRAM() const {
    return depthLayerRAM_;
}

const LayerRAM* ImageRAM::getPickingLayerRAM() const {
    return pickingLayerRAM_;
}

} // namespace
