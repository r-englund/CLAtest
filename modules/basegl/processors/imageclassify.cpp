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

#include "imageclassify.h"
#include <modules/opengl/glwrap/textureunit.h>

namespace inviwo {

ProcessorClassName(ImageClassify, "ImageClassify"); 
ProcessorCategory(ImageClassify, "Image Operation");
ProcessorCodeState(ImageClassify, CODE_STATE_EXPERIMENTAL); 

ImageClassify::ImageClassify()
    : ProcessorGL(),
      inport_("inport"),
      outport_("outport", &inport_, COLOR_ONLY),
      transferFunction_("transferFunction", "Transfer function", TransferFunction())
{
    shader_ = NULL;
    addPort(inport_);
    addPort(outport_);

    addProperty(transferFunction_);
}

ImageClassify::~ImageClassify() {}

void ImageClassify::initialize() {
    ProcessorGL::initialize();
    shader_ = new Shader("img_classify.frag");
}

void ImageClassify::deinitialize() {
    delete shader_;
    Processor::deinitialize();
}

void ImageClassify::process() {
    TextureUnit transFuncUnit;
    const Image* tfImage = transferFunction_.get().getData();
    const ImageGL* transferFunctionGL = tfImage->getRepresentation<ImageGL>();
    transferFunctionGL->bindColorTexture(transFuncUnit.getEnum());

    TextureUnit inUnit;
    bindColorTexture(inport_, inUnit.getEnum());

    activateTarget(outport_);

    shader_->activate();
    shader_->setUniform("inport_", inUnit.getUnitNumber());
    shader_->setUniform("dimension_", vec2(1.f / outport_.getDimensions()[0], 1.f / outport_.getDimensions()[1]));
    shader_->setUniform("transferFunc_", transFuncUnit.getUnitNumber());
    renderImagePlaneRect();
    shader_->deactivate();

    deactivateCurrentTarget();
    
}

} // namespace
