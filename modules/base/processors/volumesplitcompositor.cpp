#include "volumesplitcompositor.h"

namespace inviwo {

VolumeSplitCompositor::VolumeSplitCompositor()
    : Processor(),
      inport_("volumebrick.in"),
      outport_("volume.out"),
      volume_(0)
{
    addPort(inport_);
    addPort(outport_);
}

VolumeSplitCompositor::~VolumeSplitCompositor() {
    delete volume_;
}

Processor* VolumeSplitCompositor::create() const {
    return new VolumeSplitCompositor();
}

void VolumeSplitCompositor::process() {
}

void VolumeSplitCompositor::initialize() {
    Processor::initialize();
}

void VolumeSplitCompositor::deinitialize() {
    Processor::deinitialize();
}

} // inviwo namespace
