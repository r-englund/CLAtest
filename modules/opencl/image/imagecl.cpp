/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#include <modules/opencl/image/imagecl.h>

namespace inviwo {

ImageCL::ImageCL()
    : ImageRepresentation(){}

ImageCL::ImageCL(const ImageCL& rhs )
    : ImageRepresentation(rhs){}

ImageCL::~ImageCL() { 
}

DataRepresentation* ImageCL::clone() const {
    return new ImageCL(*this);
}

void ImageCL::initialize() {
}

void ImageCL::deinitialize() {
}

LayerCL* ImageCL::getLayerCL(){
    return layerCL_;
}

const LayerCL* ImageCL::getLayerCL() const {
    return layerCLConst_;
}

bool ImageCL::copyAndResizeImage(Image* im) const {
    return copyAndResizeImageRepresentation(im->getEditableRepresentation<ImageCL>());
}

bool ImageCL::copyAndResizeImageRepresentation(ImageRepresentation* targetRep) const {
    ImageCL* targetCL = dynamic_cast<ImageCL*>(targetRep);

    if (!targetCL) return false;
	
	return this->getLayerCL()->copyAndResizeLayer(targetCL->getLayerCL());
}

void ImageCL::update(bool editable) {
    //TODO: Convert more then just first color layer
    layerCL_ = NULL;
    layerCLConst_ = NULL;
    if(editable){
        layerCL_ = owner_->getColorLayer()->getEditableRepresentation<LayerCL>();
    }
    else{
        layerCLConst_ = owner_->getColorLayer()->getRepresentation<LayerCL>();
    }
}

} // namespace
