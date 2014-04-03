/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
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
 * Main file authors: Timo Ropinski, Rickard Englund
 *
 *********************************************************************************/

#include "background.h"
#include <modules/opengl/glwrap/textureunit.h>

namespace inviwo {

ProcessorClassName(Background, "Background");
ProcessorCategory(Background, "Image Operation");
ProcessorCodeState(Background, CODE_STATE_EXPERIMENTAL);

Background::Background()
    : ProcessorGL(),
      inport_("inport"),
      outport_("outport", &inport_, COLOR_ONLY),
      backgroundStyle_("backgroundStyle", "Style", PropertyOwner::INVALID_RESOURCES),
      color1_("color1", "Color 1", vec4(0.0)),
      color2_("color2", "Color 2", vec4(1.0)) {
      
    addPort(inport_);
    addPort(outport_);
    backgroundStyle_.addOption("linearGradient", "Linear gradient", 0);
    backgroundStyle_.addOption("uniformColor", "Uniform color", 1);
    backgroundStyle_.addOption("checkerBoard", "Checker board", 2);
    backgroundStyle_.setCurrentStateAsDefault();
    addProperty(backgroundStyle_);
    color1_.setSemantics(PropertySemantics::Color);
    addProperty(color1_);
    color2_.setSemantics(PropertySemantics::Color);
    addProperty(color2_);
}

Background::~Background() {
}

void Background::initialize() {
    ProcessorGL::initialize();
    shader_ = new Shader("background.frag", false);
    initializeResources();
}

void Background::deinitialize() {
    delete shader_;
    shader_ = NULL;
    Processor::deinitialize();
}

void Background::initializeResources() {
    std::string shaderDefine;

    switch (backgroundStyle_.get()) {
        case 0 : // linear gradient
            shaderDefine = "linearGradient(texCoords)";
            break;

        case 1 : // uniform color
            shaderDefine = "color1_";
            break;

        case 2 : // checker board
            shaderDefine = "checkerBoard(texCoords)";
            break;
    }

    shader_->getFragmentShaderObject()->addShaderDefine("BACKGROUND_STYLE_FUNCTION", shaderDefine);

    if (inport_.hasData()) {
        shader_->getFragmentShaderObject()->addShaderDefine("SRC_COLOR", "texture2D(srcColorTex_, texCoords)");
        hadData_ = true;
    } else {
        shader_->getFragmentShaderObject()->addShaderDefine("SRC_COLOR", "vec4(0.0,0.0,0.0,0.0)");
        hadData_ = false;
    }

    shader_->build();
}

void Background::process() {
    if (inport_.hasData() != hadData_) {
        initializeResources();
    }

    activateTarget(outport_);
    TextureUnit srcColorUnit,srcDepthUnit;

    if (inport_.hasData())
        bindTextures(inport_, srcColorUnit.getEnum(),srcDepthUnit.getEnum());

    shader_->activate();
    setGlobalShaderParameters(shader_);
    shader_->setUniform("srcColorTex_", srcColorUnit.getUnitNumber());
    shader_->setUniform("depth_", srcDepthUnit.getUnitNumber());

    if (inport_.hasData())
        setTextureParameters(inport_, shader_, "srcColorParameters_");

    shader_->setUniform("hasData_",inport_.hasData());
    shader_->setUniform("color1_", color1_.get());
    shader_->setUniform("color2_", color2_.get());
    renderImagePlaneRect();
    shader_->deactivate();
    deactivateCurrentTarget();
}

bool Background::isReady()const {
    return true;
}

} // namespace
