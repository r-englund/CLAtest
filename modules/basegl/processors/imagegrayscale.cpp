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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include "imagegrayscale.h"

namespace inviwo {

ProcessorClassName(ImageGrayscale, "ImageGrayscale"); 
ProcessorCategory(ImageGrayscale, "Image Operation");
ProcessorCodeState(ImageGrayscale, CODE_STATE_EXPERIMENTAL); 

ImageGrayscale::ImageGrayscale()
    : ProcessorGL(),
      inport_("inport"),
      outport_("outport", &inport_, COLOR_ONLY)
      
{
    shader_ = NULL;
    addPort(inport_);
    addPort(outport_);
}

ImageGrayscale::~ImageGrayscale() {}

void ImageGrayscale::initialize() {
    ProcessorGL::initialize();
    shader_ = new Shader("img_graysc.frag");
}

void ImageGrayscale::deinitialize() {
    delete shader_;
    Processor::deinitialize();
}

void ImageGrayscale::process() {    
    activateTarget(outport_);
    bindColorTexture(inport_, GL_TEXTURE0);

    shader_->activate();
    shader_->setUniform("inport_", 0);
    shader_->setUniform("dimension_", vec2(1.f / outport_.getDimensions()[0], 1.f / outport_.getDimensions()[1]));
    renderImagePlaneRect();
    shader_->deactivate();

    deactivateCurrentTarget();
    
}

} // namespace
