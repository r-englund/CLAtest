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

#include <inviwo/core/datastructures/image/imagedisk.h>
#include <inviwo/core/datastructures/image/layerdisk.h>

namespace inviwo {

ImageDisk::ImageDisk()
    : ImageRepresentation(){}

ImageDisk::~ImageDisk() {
}

void ImageDisk::initialize(){
}

void ImageDisk::deinitialize() {}

/*void ImageDisk::resize(uvec2 dimensions){        
} */

ImageDisk* ImageDisk::clone() const {
    //TODO: move to copyconstructor
    //ImageDisk* imageDiskClone = new ImageDisk(getSourceFile());
    //imageDiskClone->resize(getDimension());
    //return imageDiskClone;
    return NULL;
}

std::string ImageDisk::getClassName() const { 
     return "ImageDisk"; 
}

bool ImageDisk::copyAndResizeImageRepresentation(ImageRepresentation*) const { 
    return false; 
}

void ImageDisk::update(bool editable) {
    if(editable){
        for (size_t i=0; i<owner_->getNumberOfColorLayers(); ++i) {
            owner_->getColorLayer(i)->getEditableRepresentation<LayerDisk>();
        }

        Layer* depthLayer = owner_->getDepthLayer();
        if(depthLayer)
            depthLayer->getEditableRepresentation<LayerDisk>();

        Layer* pickingLayer = owner_->getPickingLayer();
        if(pickingLayer)
            pickingLayer->getEditableRepresentation<LayerDisk>();
    }
    else{
        for (size_t i=0; i<owner_->getNumberOfColorLayers(); ++i) {
            owner_->getColorLayer(i)->getRepresentation<LayerDisk>();
        }

        Layer* depthLayer = owner_->getDepthLayer();
        if(depthLayer)
            depthLayer->getRepresentation<LayerDisk>();

        Layer* pickingLayer = owner_->getPickingLayer();
        if(pickingLayer)
            pickingLayer->getRepresentation<LayerDisk>();
    }
}

} // namespace
