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
    : Processor(),
    inport_("inport"),
	dimensions_("dimensions", "Dimensions", ivec2(256,256), ivec2(128,128), ivec2(4096,4096)),
    visibleLayer_("visibleLayer_", "Visible layer"),
    snapshotButton_("snapshot", "Create Snapshot", PropertyOwner::VALID),
    canvas_(0)
{
    addPort(inport_);

	dimensions_.onChange(this, &CanvasProcessor::resizeCanvas);
	addProperty(dimensions_);

	visibleLayer_.addOption("color", "Color layer", COLOR_LAYER);
    visibleLayer_.addOption("depth", "Depth layer", DEPTH_LAYER);
    visibleLayer_.addOption("picking", "Picking layer", PICKING_LAYER);
    visibleLayer_.set(COLOR_LAYER);
    addProperty(visibleLayer_);

    snapshotButton_.onChange(this, &CanvasProcessor::createSnapshot);
	addProperty(snapshotButton_);
}


CanvasProcessor::~CanvasProcessor() {
    if (processorWidget_) {
        delete processorWidget_;
        processorWidget_ = 0;
    }
}

void CanvasProcessor::initialize() {
	Processor::initialize();
}

void CanvasProcessor::deinitialize() {
	processorWidget_->hide();
	delete processorWidget_;
	processorWidget_ = 0;
	Processor::deinitialize();
}

void CanvasProcessor::resizeCanvas() {
	//std::cout << "onChange" << std::endl;
	if (canvas_!=0)
		canvas_->resize(uvec2(dimensions_.get()));
	if (processorWidget_!=0) {
		processorWidget_->setDimensions(dimensions_.get());
	}
}

void CanvasProcessor::createSnapshot() {
    std::string snapshotPath(IVW_DIR+"data/images/" + toLower(getIdentifier()) + "-" + currentDateTime() + ".png");
    createSnapshot(snapshotPath.c_str());
}
    
void CanvasProcessor::createSnapshot(const char* snapshotPath) {
    const Image* image = inport_.getData();
    ImageIO::saveImage(snapshotPath, image);
    InviwoApplication::getRef().playSound(InviwoApplication::IVW_OK);
}

void CanvasProcessor::process() {
    Processor::process();
    canvas_->activate();
}

void CanvasProcessor::invalidate(PropertyOwner::InvalidationLevel invalidationLevel) {
    Processor::invalidate(invalidationLevel);
    if (canvas_ && canvas_->getNetworkEvaluator())
        if(processorWidget_ && processorWidget_->getVisibilityMetaData())
            canvas_->getNetworkEvaluator()->requestEvaluate();
}

} // namespace
