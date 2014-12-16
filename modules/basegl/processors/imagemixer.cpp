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
 * Contact: Martin Falk
 *
 *********************************************************************************/

#include "imagemixer.h"
#include <modules/opengl/glwrap/textureunit.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/textureutils.h>

namespace inviwo {

ProcessorClassIdentifier(ImageMixer, "org.inviwo.ImageMixer");
ProcessorDisplayName(ImageMixer, "Image Mixer");
ProcessorTags(ImageMixer, Tags::GL);
ProcessorCategory(ImageMixer, "Image Operation");
ProcessorCodeState(ImageMixer, CODE_STATE_STABLE);

ImageMixer::ImageMixer()
    : ImageGLProcessor("img_mix.frag")
    , inport1_("inport1")
    , blendingMode_("blendMode", "Blend Mode", INVALID_RESOURCES) 
    , weight_("weight", "Weight", 0.5f, 0.0f, 1.0f)
    , image2TexUnit_(NULL)
{
    addPort(inport1_);
    
    blendingMode_.addOption("mix", "Mix", BlendModes::Mix);
    blendingMode_.addOption("over", "Over", BlendModes::Over);
    blendingMode_.addOption("multiply", "Multiply", BlendModes::Multiply);
    blendingMode_.addOption("screen", "Screen", BlendModes::Screen);
    blendingMode_.addOption("overlay", "Overlay", BlendModes::Overlay);
    blendingMode_.addOption("hardlight", "Hard Light", BlendModes::HardLight);
    blendingMode_.addOption("divide", "Divide", BlendModes::Divide);
    blendingMode_.addOption("addition", "Addition", BlendModes::Addition);
    blendingMode_.addOption("subtraction", "Subtraction", BlendModes::Subtraction);
    blendingMode_.addOption("difference", "Difference", BlendModes::Difference);
    blendingMode_.addOption("darkenonly", "DarkenOnly", BlendModes::DarkenOnly);
    blendingMode_.addOption("brightenonly", "BrightenOnly", BlendModes::BrightenOnly);
    blendingMode_.setSelectedValue(BlendModes::Mix);
    blendingMode_.setCurrentStateAsDefault();
    
    addProperty(blendingMode_);
    addProperty(weight_);
    
    inport1_.onChange(this,&ImageMixer::inport1Changed);
}

ImageMixer::~ImageMixer() {
    delete image2TexUnit_;
}

void ImageMixer::preProcess() {
    ivwAssert(inport1_.getData() != 0, "Inport1 empty.");
    if (!image2TexUnit_) {
        image2TexUnit_ = new TextureUnit;
    }
    utilgl::bindColorTexture(inport1_, *image2TexUnit_);
    shader_->setUniform("inport1_", image2TexUnit_->getUnitNumber());
    shader_->setUniform("weight_", weight_.get());
}

void ImageMixer::postProcess() {
    delete image2TexUnit_;
    image2TexUnit_ = NULL;
}

void ImageMixer::inport1Changed() {
    ImageGLProcessor::inportChanged();
}

void ImageMixer::initializeResources() {
    // compositing defines
    std::string compositingKey = "COLOR_BLENDING(colorA, colorB)";
    std::string compositingValue = "";

    switch(blendingMode_.get()) {
    case BlendModes::Over: //<! f(a,b) = b, b over a, regular front-to-back blending
        compositingValue = "over(colorA, colorB)";
        break;
    case BlendModes::Multiply: //!< f(a,b) = a * b
        compositingValue = "multiply(colorA, colorB)";
        break;
    case BlendModes::Screen: //!< f(a,b) = 1 - (1 - a) * (1 - b)
        compositingValue = "screen(colorA, colorB)";
        break;
    case BlendModes::Overlay: //!< f(a,b) = 2 * a *b, if a < 0.5,   f(a,b) = 1 - 2(1 - a)(1 - b), otherwise (combination of Multiply and Screen)
        compositingValue = "overlay(colorA, colorB)";
        break;
    case BlendModes::HardLight: //!< Overlay where a and b are swapped
        compositingValue = "overlay(colorB, colorA)";
        break;
    case BlendModes::Divide: //!< f(a,b) = a/b
        compositingValue = "divide(colorA, colorB)";
        break;
    case BlendModes::Addition: //!< f(a,b) = a + b, clamped to [0,1]
        compositingValue = "addition(colorA, colorB)";
        break;
    case BlendModes::Subtraction: //!< f(a,b) = a - b, clamped to [0,1]
        compositingValue = "subtraction(colorA, colorB)";
        break;
    case BlendModes::Difference: //!< f(a,b) = |a - b|
        compositingValue = "difference(colorA, colorB)";
        break;
    case BlendModes::DarkenOnly: //!< f(a,b) = min(a, b), per component
        compositingValue = "darkenOnly(colorA, colorB)";
        break;
    case BlendModes::BrightenOnly: //!< f(a,b) = max(a, b), per component
        compositingValue = "brightenOnly(colorA, colorB)";
        break;
    case BlendModes::Mix: //!< f(a,b) = a * (1 - alpha) + b * alpha
    default:
        compositingValue = "colorB";
        break;
    }

    if (!shader_)
        return;

    shader_->getFragmentShaderObject()->addShaderDefine(compositingKey, compositingValue);
    shader_->build();
}

}  // namespace
