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
#include <inviwo/core/interaction/events/touchevent.h>

namespace inviwo {

ProcessorClassIdentifier(ImageLayoutGL, "org.inviwo.ImageLayoutGL");
ProcessorDisplayName(ImageLayoutGL,  "Image Layout");
ProcessorTags(ImageLayoutGL, Tags::GL);
ProcessorCategory(ImageLayoutGL, "Image Operation");
ProcessorCodeState(ImageLayoutGL, CODE_STATE_EXPERIMENTAL);

ImageLayoutGL::ImageLayoutGL()
    : ProcessorGL()
    , multiinport_("multiinport")
    , outport_("outport")
    , layout_("layout", "Layout")
    , resizeContent_("resizeContent", "Resize Content", false)
    , horizontalSplitter_("horizontalSplitter", "Horizontal Splitter", 0.5f, 0.f, 1.f)
    , verticalSplitter_("verticalSplitter", "Vertical Splitter", 0.5f, 0.f, 1.f)
    , shader_(NULL)
    , layoutHandler_(NULL)
    , currentLayout_(ImageLayoutTypes::CrossSplit)
    , currentDim_(0u, 0u)
    , resizeEnabled_(false)
{
      
    addPort(multiinport_);
    multiinport_.onChange(this, &ImageLayoutGL::multiInportChanged);
    addPort(outport_);
    layout_.addOption("single", "Single Only", ImageLayoutTypes::Single);
    layout_.addOption("horizontalSplit", "Horizontal Split", ImageLayoutTypes::HorizontalSplit);
    layout_.addOption("verticalSplit", "Vertical Split", ImageLayoutTypes::VerticalSplit);
    layout_.addOption("crossSplit", "Cross Split", ImageLayoutTypes::CrossSplit);
    layout_.addOption("threeRightOneLeftSplit", "Three Left, One Right", ImageLayoutTypes::ThreeLeftOneRight);
    layout_.addOption("threeLeftOneRightSplit", "Three Right, One Left", ImageLayoutTypes::ThreeRightOneLeft);
    layout_.setSelectedValue(ImageLayoutTypes::CrossSplit);
    layout_.setCurrentStateAsDefault();
    addProperty(layout_);
    addProperty(resizeContent_);
    horizontalSplitter_.setVisible(false);
    addProperty(horizontalSplitter_);
    verticalSplitter_.setVisible(false);
    addProperty(verticalSplitter_);

    layout_.onChange(this, &ImageLayoutGL::onStatusChange);
    resizeContent_.onChange(this, &ImageLayoutGL::onStatusChange);

    layoutHandler_ = new ImageLayoutGLInteractionHandler();
    addInteractionHandler(layoutHandler_);
    setAllPropertiesCurrentStateAsDefault();
}

ImageLayoutGL::~ImageLayoutGL() {
    removeInteractionHandler(layoutHandler_);
    delete layoutHandler_;
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

const std::vector<Inport*>& ImageLayoutGL::getInports(Event* e) const {
    InteractionEvent* ie = dynamic_cast<InteractionEvent*>(e);
    //Last clicked mouse position determines which inport is active
    //This is recorded with the interactionhandler before-hand
    if(ie && !viewCoords_.empty()){
        currentInteractionInport_.clear();
        if(multiinport_.isConnected()){
            std::vector<Inport*> inports = multiinport_.getInports();
            size_t minNum = std::min(inports.size(), viewCoords_.size());
            ivec2 activePos = layoutHandler_->getActivePosition();
            uvec2 dim = outport_.getConstData()->getDimension();
            activePos.y = static_cast<int>(dim.y) - activePos.y;
            for(size_t i=0; i<minNum; ++i){
                if(static_cast<int>(viewCoords_[i].x) <= activePos.x && (static_cast<int>(viewCoords_[i].x)+static_cast<int>(viewCoords_[i].z)) >= activePos.x)
                    if(static_cast<int>(viewCoords_[i].y) <= activePos.y && (static_cast<int>(viewCoords_[i].y)+static_cast<int>(viewCoords_[i].w)) >= activePos.y)
                        currentInteractionInport_.push_back(inports[i]);

            }
        }
        return currentInteractionInport_;
    }
    return Processor::getInports();
}

void ImageLayoutGL::multiInportChanged(){
    if(multiinport_.isConnected()){
        updateViewports(true);
        std::vector<Inport*> inports = multiinport_.getInports();
        size_t minNum = std::min(inports.size(), viewCoords_.size());
        for(size_t i=0; i<minNum; ++i){
            ImageInport* imageInport = dynamic_cast<ImageInport*>(inports[i]);
            if(imageInport){
                // TODO: use scale depending on viewport and output dimension
                imageInport->setResizeScale(vec2(0.25f, 0.25f)); 
            }
        }
    }
}

void ImageLayoutGL::process() {
    TextureUnit::setZeroUnit();
    std::vector<const Image*> images = multiinport_.getData();
    uvec2 dim = outport_.getData()->getDimension();

    //updateViewports();
    TextureUnit colorUnit, depthUnit, pickingUnit;

    activateAndClearTarget(outport_);

    shader_->activate();
    setGlobalShaderParameters(shader_);
    shader_->setUniform("color_", colorUnit.getUnitNumber());
    shader_->setUniform("depth_", depthUnit.getUnitNumber());
    shader_->setUniform("picking_", pickingUnit.getUnitNumber());

    size_t minNum = std::min(images.size(), viewCoords_.size());
    for(size_t i=0; i<minNum; ++i){
        bindTextures(images[i], colorUnit.getEnum(), depthUnit.getEnum(), pickingUnit.getEnum());
        glViewport(static_cast<int>(viewCoords_[i].x), static_cast<int>(viewCoords_[i].y), viewCoords_[i].z, viewCoords_[i].w);
        renderImagePlaneRect();
    }

    glViewport(0, 0, dim.x, dim.y);
    shader_->deactivate();
    deactivateCurrentTarget();
    TextureUnit::setZeroUnit();
}

void ImageLayoutGL::updateViewports(bool force) {
    uvec2 dim(256u, 256u);
    if (outport_.isConnected())
        dim = outport_.getData()->getDimension();

    if (!force && (currentDim_ == dim) 
        && (currentLayout_ == layout_.get()) 
        && (resizeEnabled_ == resizeContent_.get()))
        return; // no changes

    viewCoords_.clear();
    unsigned int smallWindowDim = dim.y/3;
    switch (layout_.getSelectedValue())
    {
    case ImageLayoutTypes::HorizontalSplit:
        viewCoords_.push_back(uvec4(0, dim.y/2, dim.x, dim.y/2));
        viewCoords_.push_back(uvec4(0, 0, dim.x, dim.y/2));
        break;
    case ImageLayoutTypes::VerticalSplit:
        viewCoords_.push_back(uvec4(0, 0, dim.x/2, dim.y));
        viewCoords_.push_back(uvec4(dim.x/2, 0, dim.x/2, dim.y));
        break;
    case ImageLayoutTypes::CrossSplit:
        viewCoords_.push_back(uvec4(0, dim.y/2, dim.x/2, dim.y/2));
        viewCoords_.push_back(uvec4(dim.x/2, dim.y/2, dim.x/2, dim.y/2));
        viewCoords_.push_back(uvec4(0, 0, dim.x/2, dim.y/2));
        viewCoords_.push_back(uvec4(dim.x/2, 0, dim.x/2, dim.y/2));
        break;
    case ImageLayoutTypes::ThreeLeftOneRight: 
        viewCoords_.push_back(uvec4(0, 2*smallWindowDim, smallWindowDim, smallWindowDim));
        viewCoords_.push_back(uvec4(0, smallWindowDim, smallWindowDim, smallWindowDim));
        viewCoords_.push_back(uvec4(0, 0, smallWindowDim, smallWindowDim));
        viewCoords_.push_back(uvec4(smallWindowDim, 0, dim.x-smallWindowDim, dim.y));
        break;
    case ImageLayoutTypes::ThreeRightOneLeft: 
        viewCoords_.push_back(uvec4(dim.x-smallWindowDim, 2*smallWindowDim, smallWindowDim, smallWindowDim));
        viewCoords_.push_back(uvec4(dim.x-smallWindowDim, smallWindowDim, smallWindowDim, smallWindowDim));
        viewCoords_.push_back(uvec4(dim.x-smallWindowDim, 0, smallWindowDim, smallWindowDim));
        viewCoords_.push_back(uvec4(0, 0, dim.x-smallWindowDim, dim.y));
        break;
    case ImageLayoutTypes::Single:
    default:
        viewCoords_.push_back(uvec4(0, 0, dim.x, dim.y));
    }

    currentDim_ = dim;
    currentLayout_ = static_cast<ImageLayoutTypes::Layout>(layout_.get());
    resizeEnabled_ = resizeContent_.get();

    // propagate viewport size to connected inports
    if (resizeEnabled_) {
        std::vector<Inport*> inports = multiinport_.getInports();
        size_t minNum = std::min(inports.size(), viewCoords_.size());
        for (std::size_t i=0; i<minNum; ++i) {
            ImageInport* imageInport = dynamic_cast<ImageInport*>(inports[i]);
            if(imageInport){
                uvec2 viewportDim(viewCoords_[i].z, viewCoords_[i].w);
                ResizeEvent e(viewportDim);
                imageInport->changeDataDimensions(&e);
            }
        }
    }
}

void ImageLayoutGL::onStatusChange() {
    updateViewports();
}

ImageLayoutGL::ImageLayoutGLInteractionHandler::ImageLayoutGLInteractionHandler() 
    : InteractionHandler()
    , activePositionChangeEvent_(ivec2(0), MouseEvent::MOUSE_BUTTON_LEFT, MouseEvent::MOUSE_STATE_PRESS, InteractionEvent::MODIFIER_NONE, uvec2(512))
    , viewportActive_(false)
    , activePosition_(ivec2(0)) {
}

void ImageLayoutGL::ImageLayoutGLInteractionHandler::invokeEvent(Event* event){
    MouseEvent* mouseEvent = dynamic_cast<MouseEvent*>(event);
    if(mouseEvent){
        if(!viewportActive_ && mouseEvent->state() == activePositionChangeEvent_.state()) {
            viewportActive_ = true;
            activePosition_ = mouseEvent->pos();
        }
        else if(viewportActive_ && mouseEvent->state() == MouseEvent::MOUSE_STATE_RELEASE){
            viewportActive_ = false;
        }
        return;
    }

    TouchEvent* touchEvent = dynamic_cast<TouchEvent*>(event);
    if(touchEvent){
        if(!viewportActive_ && touchEvent->state() == TouchEvent::TOUCH_STATE_STARTED) {
            viewportActive_ = true;
            activePosition_ = touchEvent->pos();
        }
        else if(viewportActive_ && touchEvent->state() == TouchEvent::TOUCH_STATE_ENDED){
            viewportActive_ = false;
        }
        return;
    }
}

} // namespace
