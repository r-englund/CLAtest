/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Main file authors: Erik Sundén, Timo Ropinski
 *
 *********************************************************************************/

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/io/imageio.h>
#include <inviwo/core/processors/canvasprocessor.h>
#include <inviwo/core/util/canvas.h>
#include <inviwo/core/util/datetime.h>
#include <inviwo/core/util/stringconversion.h>

namespace inviwo {

CanvasProcessor::CanvasProcessor()
    : Processor()
    , inport_("inport")
    , dimensions_("dimensions", "Dimensions", ivec2(256,256), ivec2(128,128), ivec2(4096,4096))
    , visibleLayer_("visibleLayer", "Visible Layer")
    , saveLayerDirectory_("layerDir", "Output Directory", InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_DATA)+"images")
    , saveLayerButton_("saveLayer", "Save Image Layer", PropertyOwner::VALID)
    , canvas_(NULL)
    , disableResize_(false)
    , queuedRequest_(false)
{
    addPort(inport_);
    dimensions_.onChange(this, &CanvasProcessor::resizeCanvas);
    addProperty(dimensions_);
    visibleLayer_.addOption("color", "Color layer", COLOR_LAYER);
    visibleLayer_.addOption("depth", "Depth layer", DEPTH_LAYER);
    visibleLayer_.addOption("picking", "Picking layer", PICKING_LAYER);
    visibleLayer_.set(COLOR_LAYER);
    addProperty(visibleLayer_);
    addProperty(saveLayerDirectory_);
    saveLayerButton_.onChange(this, &CanvasProcessor::saveImageLayer);
    addProperty(saveLayerButton_);
}


CanvasProcessor::~CanvasProcessor() {}

void CanvasProcessor::initialize() {
    Processor::initialize();
}

void CanvasProcessor::deinitialize() {
    if (processorWidget_)
        processorWidget_->hide();

    if(canvas_ && (!canvas_->getProcessorNetworkEvaluator() || canvas_ != canvas_->getProcessorNetworkEvaluator()->getDefaultRenderContext())){
        delete canvas_;
        canvas_ = NULL;
    }

    Processor::deinitialize();
}

void CanvasProcessor::setCanvas(Canvas* canvas) { 
    canvas_ = canvas; 
}

Canvas* CanvasProcessor::getCanvas() const { 
    return canvas_; 
}

void CanvasProcessor::resizeCanvas() {
    if (!disableResize_) {
        if (processorWidget_)
            processorWidget_->setDimension(dimensions_.get());
    }
}

void CanvasProcessor::setCanvasSize(ivec2 dim) {
    disableResize_ = true;
    dimensions_.set(dim);

    if (canvas_)
        canvas_->resize(uvec2(dimensions_.get()));

    disableResize_ = false;
}

ivec2 CanvasProcessor::getCanvasSize() const {
    return dimensions_.get();
}

void CanvasProcessor::saveImageLayer() {
    std::string snapshotPath(saveLayerDirectory_.get() + "/" + toLower(getIdentifier()) + "-" + currentDateTime() + ".png");
    saveImageLayer(snapshotPath.c_str());
}

void CanvasProcessor::saveImageLayer(const char* snapshotPath) {
    const Image* image = inport_.getData();
    const Layer* layer = image->getLayer(static_cast<LayerType>(visibleLayer_.get()));
    ImageIO::saveLayer(snapshotPath, layer);
}

void CanvasProcessor::process() {
    Processor::process();
    canvas_->activate();
}

void CanvasProcessor::invalidate(PropertyOwner::InvalidationLevel invalidationLevel,
                                 Property* modifiedProperty) {
    Processor::invalidate(invalidationLevel, modifiedProperty);
}

void CanvasProcessor::triggerQueuedEvaluation() {
    if (queuedRequest_) {
        performEvaluateRequest();
        queuedRequest_ = false;
    }
}

void CanvasProcessor::performEvaluationAtNextShow() {
    queuedRequest_ = true;
}

void CanvasProcessor::performEvaluateRequest() {
    if (canvas_ && canvas_->getProcessorNetworkEvaluator()) {
        if (processorWidget_) {
            if (processorWidget_->getVisibilityMetaData())
                notifyObserversRequestEvaluate(this);
            else
                performEvaluationAtNextShow();
        }
        else
            notifyObserversRequestEvaluate(this);
    }
}

} // namespace
