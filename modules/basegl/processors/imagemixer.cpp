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
 * Main file authors: Timo Ropinski, Sathish Kottravel, Erik Sundén
 *
 *********************************************************************************/

#include "imagemixer.h"
#include <modules/opengl/glwrap/textureunit.h>

namespace inviwo {

ProcessorClassName(ImageMixer, "ImageMixer");
ProcessorCategory(ImageMixer, "Image Operation");
ProcessorCodeState(ImageMixer, CODE_STATE_EXPERIMENTAL);

ImageMixer::ImageMixer()
    : ProcessorGL()
    , inport0_("inport0")
    , inport1_("inport1")
    , outport_("outport", &inport0_, COLOR_ONLY)
    , alpha_("alpha", "Alpha", 0.5f, 0.0f, 1.0f)
    , blendingMode_("compositingMode", "Compositing", PropertyOwner::INVALID_RESOURCES)
{
    addPort(inport0_);
    addPort(inport1_);
    addPort(outport_);
    addProperty(alpha_);
    blendingMode_.addOption("mix", "Mix");
    blendingMode_.addOption("add", "Add");
    blendingMode_.setSelectedIdentifier("mix");
    blendingMode_.setCurrentStateAsDefault();
    addProperty(blendingMode_);
}

ImageMixer::~ImageMixer() {}

void ImageMixer::initialize() {
    ProcessorGL::initialize();
    shader_ = new Shader("img_mix.frag", false);
    initializeResources();
}

void ImageMixer::deinitialize() {
    delete shader_;
    ProcessorGL::deinitialize();
}

void ImageMixer::process() {
    ivwAssert(inport0_.getData()!=0, "Inport0 empty.");
    ivwAssert(inport1_.getData()!=0, "Inport1 empty.");
    uvec2 csize = outport_.getData()->getDimension();
    bindColorTexture(inport0_, GL_TEXTURE1);
    bindColorTexture(inport1_, GL_TEXTURE2);
    activateAndClearTarget(outport_);
    shader_->activate();
    shader_->setUniform("inport0_", 1);
    shader_->setUniform("inport1_", 2);
    shader_->setUniform("alpha_", alpha_.get());
    shader_->setUniform("screenDimRCP_", vec2(1.f / csize[0], 1.f / csize[1]));
    renderImagePlaneRect();
    shader_->deactivate();
    deactivateCurrentTarget();
}

void ImageMixer::initializeResources() {
    // compositing defines
    std::string compositingKey = "COLOR_BLENDING(colorA, colorB, param)";
    std::string compositingValue = "";

    if (blendingMode_.isSelectedIdentifier("mix")) {
        compositingValue = "colorMix(colorA, colorB, param);";
        alpha_.setVisible(true);
    } else if (blendingMode_.isSelectedIdentifier("add")) {
        compositingValue = "colorAdd(colorA, colorB);";
        alpha_.setVisible(false);
    }
    shader_->getFragmentShaderObject()->addShaderDefine(compositingKey, compositingValue);
    shader_->build();
}

} // namespace
