#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/io/imageloader.h>
#include <inviwo/core/processors/canvasprocessor.h>
#include <inviwo/core/util/datetime.h>
#include <inviwo/core/util/stringconversion.h>

namespace inviwo {

CanvasProcessor::CanvasProcessor()
    : Processor(),
    inport_("inport"),
    snapshotButton_("snapshot", "Create Snapshot", PropertyOwner::VALID),
    canvas_(0)
{
    addPort(inport_);

    //snapshotButton_.onChange(this, &CanvasProcessor::createSnapshot);
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

Processor* CanvasProcessor::create() const {
    return new CanvasProcessor();
}

void CanvasProcessor::initialize() {
    Processor::initialize();
}

void CanvasProcessor::deinitialize() {
    processorWidget_->hide();
    //delete processorWidget_;
    //processorWidget_ = 0;
    Processor::deinitialize();
}

void CanvasProcessor::process() {
    Processor::process();
    canvas_->activate();
}

void CanvasProcessor::invalidate() {
    PropertyOwner::invalidate();
    //if (canvas_)
      //  canvas_->update();
}

} // namespace
