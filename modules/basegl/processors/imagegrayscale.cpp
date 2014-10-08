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

#include "imagegrayscale.h"
#include <modules/opengl/textureutils.h>
#include <modules/opengl/glwrap/shader.h>

namespace inviwo {

ProcessorClassIdentifier(ImageGrayscale, "org.inviwo.ImageGrayscale");
ProcessorDisplayName(ImageGrayscale, "Image Grayscale");
ProcessorTags(ImageGrayscale, Tags::GL);
ProcessorCategory(ImageGrayscale, "Image Operation");
ProcessorCodeState(ImageGrayscale, CODE_STATE_EXPERIMENTAL);

ImageGrayscale::ImageGrayscale()
    : Processor(), inport_("inport"), outport_("outport", &inport_, COLOR_ONLY), shader_(NULL) {

    addPort(inport_);
    addPort(outport_);
}

ImageGrayscale::~ImageGrayscale() {}

void ImageGrayscale::initialize() {
    Processor::initialize();
    shader_ = new Shader("img_graysc.frag");
}

void ImageGrayscale::deinitialize() {
    delete shader_;
    Processor::deinitialize();
}

void ImageGrayscale::process() {
    TextureUnit image;
    utilgl::activateTarget(outport_);
    utilgl::bindColorTexture(inport_, image);
    shader_->activate();
    shader_->setUniform("inport_", image.getUnitNumber());
    shader_->setUniform("screenDimRCP_",
                        vec2(1.f / outport_.getDimension()[0], 1.f / outport_.getDimension()[1]));
    utilgl::singleDrawImagePlaneRect();
    shader_->deactivate();
    utilgl::deactivateCurrentTarget();
}

}  // namespace
