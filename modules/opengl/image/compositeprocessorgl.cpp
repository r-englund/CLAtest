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
    program_ = new Shader(programFileName_);
}

void CompositeProcessorGL::deinitialize() {
    delete program_;
    program_ = 0;
    ProcessorGL::deinitialize();
}

void CompositeProcessorGL::compositePortsToOutport(ImageOutport& outport, ImageInport& inport){
    if (inport.isReady() && outport.isReady()) {
        activateTarget(outport);

        TextureUnit inportColorUnit, inportDepthUnit, inportPickingUnit;
        bindTextures(inport, inportColorUnit.getEnum(), inportDepthUnit.getEnum(), inportPickingUnit.getEnum());

        TextureUnit outportColorUnit, outportDepthUnit, outportPickingUnit;
        bindTextures(outport, outportColorUnit.getEnum(), outportDepthUnit.getEnum(), outportPickingUnit.getEnum());

        program_->activate();

        setGlobalShaderParameters(program_);

        program_->setUniform("texColor0_", inportColorUnit.getUnitNumber());
        program_->setUniform("texDepth0_", inportDepthUnit.getUnitNumber());
        program_->setUniform("texPicking0_", inportPickingUnit.getUnitNumber());

        program_->setUniform("texColor1_", outportColorUnit.getUnitNumber());
        program_->setUniform("texDepth1_", outportDepthUnit.getUnitNumber());
        program_->setUniform("texPicking1_", outportPickingUnit.getUnitNumber());

        renderImagePlaneRect();

        program_->deactivate();

        deactivateCurrentTarget();
    }
}

void CompositeProcessorGL::initializeResources() {
    program_->rebuild();
}

} // namespace
