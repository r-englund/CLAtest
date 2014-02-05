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
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

#include "compositeprocessorgl.h"
#include <modules/opengl/glwrap/textureunit.h>

namespace inviwo {

CompositeProcessorGL::CompositeProcessorGL()
    : ProcessorGL(),
    programFileName_("composite.frag"), compositeShader_(NULL)
{}

CompositeProcessorGL::CompositeProcessorGL(std::string programFileName)
    : ProcessorGL(),
    programFileName_(programFileName), compositeShader_(NULL)
{}

void CompositeProcessorGL::initialize() {
    ProcessorGL::initialize();
    compositeShader_ = new Shader(programFileName_);
}

void CompositeProcessorGL::deinitialize() {
    delete compositeShader_;
    compositeShader_ = NULL;
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
