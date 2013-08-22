#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/io/imageloader.h>
#include <inviwo/core/processors/canvasprocessor.h>
#include <inviwo/core/util/datetime.h>
#include <inviwo/core/util/stringconversion.h>

namespace inviwo {

CanvasProcessor::CanvasProcessor()
    : Processor(),
    inport_("inport"),
    displayedLayer_("displayedLayer_", "Display Layer"),
    snapshotButton_("snapshot", "Create Snapshot", PropertyOwner::VALID),
    canvas_(0)
{
    addPort(inport_);

    displayedLayer_.addOption("color", "Color Layer", COLOR_LAYER);
    displayedLayer_.addOption("depth", "Depth Layer", DEPTH_LAYER);
    displayedLayer_.addOption("picking", "Picking Layer", PICKING_LAYER);
    displayedLayer_.set(COLOR_LAYER);
    addProperty(displayedLayer_);

    snapshotButton_.registerClassMemberFunction(this, &CanvasProcessor::createSnapshot);
	addProperty(snapshotButton_);
}


CanvasProcessor::~CanvasProcessor() {
    if (processorWidget_) {
        delete processorWidget_;
        processorWidget_ = 0;
    }
}

void CanvasProcessor::createSnapshot() {
    std::string snapshotPath(IVW_DIR+"data/images/" + toLower(getIdentifier()) + "-" + currentDateTime() + ".png");
    createSnapshot(snapshotPath.c_str());
}
    
void CanvasProcessor::createSnapshot(const char* snapshotPath) {
    const Image* image = inport_.getData();
    ImageLoader::saveImage(snapshotPath, image);
    InviwoApplication::getRef().playSound(InviwoApplication::IVW_OK);
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

void CanvasProcessor::process() {
    Processor::process();
    canvas_->activate();
}

void CanvasProcessor::invalidate(PropertyOwner::InvalidationLevel invalidationLevel) {
    PropertyOwner::invalidate(invalidationLevel);
    if (canvas_ && canvas_->getNetworkEvaluator())
        canvas_->getNetworkEvaluator()->evaluate();
}

} // namespace
