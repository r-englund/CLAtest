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

#include <modules/opencl/image/imageclgl.h>
#include <modules/opencl/image/layerclgl.h>

namespace inviwo {

ImageCLGL::ImageCLGL()
    : ImageRepresentation()
{}

ImageCLGL::ImageCLGL(const ImageCLGL& rhs )
    : ImageRepresentation(rhs)
{}

ImageCLGL::~ImageCLGL() { 
}

DataRepresentation* ImageCLGL::clone() const {
    return new ImageCLGL(*this);
}

void ImageCLGL::initialize() {
}

void ImageCLGL::deinitialize() {
}

LayerCLGL* ImageCLGL::getLayerCLGL(){
    return layerCLGL_;
}

const LayerCLGL* ImageCLGL::getLayerCLGL() const {
    return layerCLGLConst_;
}

bool ImageCLGL::copyAndResizeImageRepresentation(ImageRepresentation* targetRep) const {
    ImageCLGL* targetCLGL = dynamic_cast<ImageCLGL*>(targetRep);

    if (!targetCLGL) return false;
	
	return layerCLGL_->copyAndResizeLayer(targetCLGL->getLayerCLGL());
}

void ImageCLGL::update(bool editable) {
    //TODO: Convert more then just first color layer
    layerCLGL_ = NULL;
    layerCLGLConst_ = NULL;
    if(editable){
        layerCLGL_ = owner_->getColorLayer()->getEditableRepresentation<LayerCLGL>();
    }
    else{
        layerCLGLConst_ = owner_->getColorLayer()->getRepresentation<LayerCLGL>();
    }
}

} // namespace
