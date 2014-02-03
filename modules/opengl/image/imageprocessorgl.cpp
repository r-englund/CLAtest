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
 * Main file author: Timo Ropinski
 *
 *********************************************************************************/

#include "imageprocessorgl.h"

namespace inviwo {

ImageProcessorGL::ImageProcessorGL()
    : ProcessorGL(),
    programFileName_("pipethrough.frag")
{}

ImageProcessorGL::ImageProcessorGL(std::string programFileName)
    : ProcessorGL(),
    programFileName_(programFileName)
{}

void ImageProcessorGL::initialize() {
    ProcessorGL::initialize();
    program_ = new Shader(programFileName_);
}

void ImageProcessorGL::deinitialize() {
    delete program_;
    program_ = 0;
    ProcessorGL::deinitialize();
}

void ImageProcessorGL::initializeResources() {
    program_->rebuild();
}

vec2 ImageProcessorGL::computeDepthRange(ImageInport* inport) {
	vec2 depthRange = vec2(1.0f, 0.0f);
	/*const ImageRAM* imageRAM = (inport->getData())->getRepresentation<ImageRAM>();
	for (unsigned int x=0; x<imageRAM->getDimension().x; x++) {
		for (unsigned int y=0; y<imageRAM->getDimension().y; y++) {
			float curDepth = imageRAM->getDepthValue(uvec2(x,y));
			if (curDepth < depthRange.x) depthRange.x = curDepth;
			if (curDepth > depthRange.y) depthRange.y = curDepth;
		}
	}*/
	return depthRange;
}

} // namespace
