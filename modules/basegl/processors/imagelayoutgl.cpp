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
 * Main file authors:  Erik Sundén
 *
 *********************************************************************************/

#include "imagelayoutgl.h"
#include <modules/opengl/glwrap/textureunit.h>

namespace inviwo {

ProcessorClassName(ImageLayoutGL, "ImageLayoutGL");
ProcessorCategory(ImageLayoutGL, "Image Operation");
ProcessorCodeState(ImageLayoutGL, CODE_STATE_EXPERIMENTAL);

ImageLayoutGL::ImageLayoutGL()
    : ProcessorGL()
    , multiinport_("multiinport")
    , outport_("outport")
    , layout_("layout", "Layout")
    , horizontalSplitter_("horizontalSplitter", "Horizontal Splitter", 0.5f, 0.f, 1.f)
    , verticalSplitter_("verticalSplitter", "Vertical Splitter", 0.5f, 0.f, 1.f)
    , shader_(NULL) {
      
    addPort(multiinport_);
    addPort(outport_);
    layout_.addOption("single", "Single Only", 1);
    layout_.addOption("horizontalSplit", "Horizontal Split", 2);
    layout_.addOption("verticalSplit", "Vertical Split", 3);
    layout_.addOption("crossSplit", "Cross Split", 4);
    layout_.setCurrentStateAsDefault();
    addProperty(layout_);
    addProperty(horizontalSplitter_);
    addProperty(verticalSplitter_);
}

ImageLayoutGL::~ImageLayoutGL() {
}

void ImageLayoutGL::initialize() {
    ProcessorGL::initialize();
    shader_ = new Shader("img_texturequad.vert", "img_copy.frag");
}

void ImageLayoutGL::deinitialize() {
    delete shader_;
    shader_ = NULL;
    Processor::deinitialize();
}

void ImageLayoutGL::process() {
    std::vector<const Image*> images = multiinport_.getData();
    const ImageGL* inImageGL = images[0]->getRepresentation<ImageGL>();

    TextureUnit colorUnit, depthUnit, pickingUnit;

    activateTarget(outport_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glEnable(GL_BLEND); 
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader_->activate();
    shader_->setUniform("color_", colorUnit.getUnitNumber());
    shader_->setUniform("depth_", depthUnit.getUnitNumber());
    shader_->setUniform("picking_", pickingUnit.getUnitNumber());

    //glDepthMask(GL_TRUE);

    inImageGL->getColorLayerGL()->bindTexture(colorUnit.getEnum());
    inImageGL->getDepthLayerGL()->bindTexture(depthUnit.getEnum());
    inImageGL->getPickingLayerGL()->bindTexture(pickingUnit.getEnum());

    renderImagePlaneRect();

    inImageGL->getColorLayerGL()->unbindTexture();
    inImageGL->getDepthLayerGL()->unbindTexture();
    inImageGL->getPickingLayerGL()->unbindTexture();
    
    shader_->deactivate();
    deactivateCurrentTarget();

    //glViewport(0, 0, outport_.getDimension().x,  outport_.getDimension().y);

    //glDisable(GL_BLEND); 
}

} // namespace
