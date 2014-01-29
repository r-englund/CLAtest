/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
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

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/io/imageio.h>
#include <inviwo/core/processors/canvasprocessor.h>
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
    , canvas_(0)
    , disableResize_(false)
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
	Processor::deinitialize();
}

void CanvasProcessor::resizeCanvas() {
    if(!disableResize_){
	    if (processorWidget_)
		    processorWidget_->setDimension(dimensions_.get());
    }
}

void CanvasProcessor::setCanvasSize(ivec2 dim){
    disableResize_ = true;
    dimensions_.set(dim);
    if (canvas_)
        canvas_->resize(uvec2(dimensions_.get()));
    disableResize_ = false;
}

ivec2 CanvasProcessor::getCanvasSize() const{
    return dimensions_.get();
}

void CanvasProcessor::saveImageLayer() {
    std::string snapshotPath(saveLayerDirectory_.get() + "/" + toLower(getIdentifier()) + "-" + currentDateTime() + ".png");
    saveImageLayer(snapshotPath.c_str());
}

void CanvasProcessor::saveImageLayer(const char* snapshotPath) {
    const Image* image = inport_.getData();
    const Layer* layer = image->getLayer(visibleLayer_.get());
    ImageIO::saveLayer(snapshotPath, layer);
}

void CanvasProcessor::process() {
    Processor::process();
    canvas_->activate();
}

void CanvasProcessor::invalidate(PropertyOwner::InvalidationLevel invalidationLevel) {
    Processor::invalidate(invalidationLevel);
}

void CanvasProcessor::performEvaluateRequest(){
    if (canvas_ && canvas_->getNetworkEvaluator()){
        if(processorWidget_){
            if(processorWidget_->getVisibilityMetaData()){
                canvas_->getNetworkEvaluator()->requestEvaluate();
            }
            else{
                canvas_->performEvaluationAtNextShow();
            }
        }
        else{
            canvas_->getNetworkEvaluator()->requestEvaluate();
        }
    }
}

} // namespace
