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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include "imagemixer.h"
#include <modules/opengl/glwrap/textureunit.h>

namespace inviwo {

ProcessorClassName(ImageMixer, "ImageMixer"); 
ProcessorCategory(ImageMixer, "Image Operation");
ProcessorCodeState(ImageMixer, CODE_STATE_EXPERIMENTAL); 

ImageMixer::ImageMixer()
    : ProcessorGL(),
      inport0_("inport0"),
      inport1_("inport1"),
      outport_("outport", &inport0_, COLOR_ONLY),
      alpha_("alpha", "Alpha", 0.5f, 0.0f, 1.0f)
{
    addPort(inport0_);
    addPort(inport1_);
    addPort(outport_);
    addProperty(alpha_);
}

ImageMixer::~ImageMixer() {}

void ImageMixer::initialize() {
    ProcessorGL::initialize();
    shader_ = new Shader("img_mix.frag");
}

void ImageMixer::deinitialize() {
    delete shader_;
    ProcessorGL::deinitialize();
}

void ImageMixer::process() {    
    ivwAssert(inport0_.getData()!=0, "Inport0 empty.");
    ivwAssert(inport1_.getData()!=0, "Inport1 empty.");
    TextureUnit inportTexture0;
    TextureUnit inportTexture1;
    uvec2 csize = outport_.getData()->getDimension();    
    bindColorTexture(inport0_, inportTexture0.getEnum());
    bindColorTexture(inport1_, inportTexture1.getEnum());    
    activateAndClearTarget(outport_);
    shader_->activate();
    shader_->setUniform("inport0_", inportTexture0.getUnitNumber());
    shader_->setUniform("inport1_", inportTexture1.getUnitNumber());
    shader_->setUniform("alpha_", alpha_.get());
    shader_->setUniform("screenDimRCP_", vec2(1.f / csize[0], 1.f / csize[1]) );
    renderImagePlaneRect();
    shader_->deactivate();
    deactivateCurrentTarget();
    unbindColorTexture(inport0_);
    unbindColorTexture(inport1_);
}

} // namespace
