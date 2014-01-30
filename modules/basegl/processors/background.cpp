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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

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
	  color2_("color2", "Color 2", vec4(1.0))
{
    addPort(inport_);
    addPort(outport_);

    backgroundStyle_.addOption("linearGradient", "Linear gradient", 0);
	backgroundStyle_.addOption("uniformColor", "Uniform color", 1);
	backgroundStyle_.addOption("checkerBoard", "Checker board", 2);
    addProperty(backgroundStyle_);
	color1_.setSemantics(PropertySemantics::Color);
	addProperty(color1_);
	color2_.setSemantics(PropertySemantics::Color);
	addProperty(color2_);
}

Background::~Background() {}

void Background::initialize() {
    ProcessorGL::initialize();
    shader_ = new Shader("background.frag", false);
	initializeResources();
}

void Background::deinitialize() {
    delete shader_;
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
	shader_->build();
}

void Background::process() { 
    activateTarget(outport_);

    TextureUnit srcColorUnit;
    if(inport_.hasData())
        bindColorTexture(inport_, srcColorUnit.getEnum());

    shader_->activate();
	setGlobalShaderParameters(shader_);
    shader_->setUniform("srcColorTex_", srcColorUnit.getUnitNumber());
    if(inport_.hasData())
        setTextureParameters(inport_, shader_, "srcColorParameters_");

    shader_->setUniform("hasData_",inport_.hasData());

	shader_->setUniform("color1_", color1_.get());
	shader_->setUniform("color2_", color2_.get());

	renderImagePlaneRect();

	shader_->deactivate();
    deactivateCurrentTarget();
}

bool Background::isReady()const{
    return true;
}

} // namespace
