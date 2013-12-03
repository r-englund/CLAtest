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

#include "compositeprocessorgl.h"
#include <modules/opengl/glwrap/textureunit.h>

namespace inviwo {

CompositeProcessorGL::CompositeProcessorGL()
    : ProcessorGL(),
    programFileName_("composite.frag")
{}

CompositeProcessorGL::CompositeProcessorGL(std::string programFileName)
    : ProcessorGL(),
    programFileName_(programFileName)
{}

void CompositeProcessorGL::initialize() {
    ProcessorGL::initialize();
    compositeShader_ = new Shader(programFileName_);
}

void CompositeProcessorGL::deinitialize() {
    delete compositeShader_;
    compositeShader_ = 0;
    ProcessorGL::deinitialize();
}

void CompositeProcessorGL::compositePortsToOutport(ImageOutport& outport, ImageInport& inport){
    if (inport.isReady() && outport.isReady()) {
        activateTarget(outport);

        TextureUnit inportColorUnit, inportDepthUnit, inportPickingUnit;
        bindTextures(inport, inportColorUnit.getEnum(), inportDepthUnit.getEnum(), inportPickingUnit.getEnum());

        TextureUnit outportColorUnit, outportDepthUnit, outportPickingUnit;
        bindTextures(outport, outportColorUnit.getEnum(), outportDepthUnit.getEnum(), outportPickingUnit.getEnum());

        compositeShader_->activate();

        setGlobalShaderParameters(compositeShader_);

        compositeShader_->setUniform("texColor0_", inportColorUnit.getUnitNumber());
        compositeShader_->setUniform("texDepth0_", inportDepthUnit.getUnitNumber());
        compositeShader_->setUniform("texPicking0_", inportPickingUnit.getUnitNumber());

        compositeShader_->setUniform("texColor1_", outportColorUnit.getUnitNumber());
        compositeShader_->setUniform("texDepth1_", outportDepthUnit.getUnitNumber());
        compositeShader_->setUniform("texPicking1_", outportPickingUnit.getUnitNumber());

        renderImagePlaneRect();

        compositeShader_->deactivate();

        deactivateCurrentTarget();
    }
}

void CompositeProcessorGL::initializeResources() {
    compositeShader_->rebuild();
}

} // namespace
