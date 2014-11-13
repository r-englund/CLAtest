/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
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

#include "imagegpuprocessor.h"
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/textureutils.h>
#include <modules/opengl/shaderutils.h>
#include <modules/opengl/glwrap/framebufferobject.h>

namespace inviwo {

ImageGPUProcessor::ImageGPUProcessor(std::string fragmentShader)
    : Processor()
    , inport_(fragmentShader + "inport")
    , outport_(fragmentShader + "outport")
    , dataFormat_(NULL)
    , internalInvalid_(false)
    , fragmentShader_(fragmentShader)
    , shader_(NULL)
{
    addPort(inport_);
    addPort(outport_);

    inport_.onChange(this,&ImageGPUProcessor::inportChanged);
}

ImageGPUProcessor::~ImageGPUProcessor() {}

void ImageGPUProcessor::initialize() {
    Processor::initialize();
    delete shader_;
    shader_ = new Shader(fragmentShader_, true);
    internalInvalid_ = true;
}

void ImageGPUProcessor::deinitialize() {
    Processor::deinitialize();
    delete shader_;
}

void ImageGPUProcessor::process() {
    if (internalInvalid_ || inport_.getInvalidationLevel() >= INVALID_OUTPUT) {
        internalInvalid_ = false;
        const DataFormatBase* format = dataFormat_;
        if (format == NULL) {
            format = inport_.getData()->getDataFormat();
        }

        Image *img = new Image(inport_.getData()->getDimension(), COLOR_ONLY, format);
        img->copyMetaDataFrom(*inport_.getData());
        outport_.setData(img);
    }

    TextureUnit imgUnit;    
    utilgl::bindColorTexture(inport_, imgUnit);

    utilgl::activateTarget(outport_);
    shader_->activate();
    shader_->setUniform("inport_", imgUnit.getUnitNumber());
    shader_->setUniform("screenDimRCP_",
        vec2(1.f / outport_.getDimension()[0], 1.f / outport_.getDimension()[1]));
    utilgl::singleDrawImagePlaneRect();
    shader_->deactivate();
    utilgl::deactivateCurrentTarget();

    afterProcess();
}

}  // namespace
