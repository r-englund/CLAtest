#include "volumesplitcompositor.h"

namespace inviwo {

ProcessorClassName(VolumeSplitCompositor, "VolumeSplitCompositor"); 
ProcessorCategory(VolumeSplitCompositor, "Volume Operation");
ProcessorCodeState(VolumeSplitCompositor, CODE_STATE_BROKEN);

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

void VolumeSplitCompositor::process() {
}

void VolumeSplitCompositor::initialize() {
    Processor::initialize();
}

void VolumeSplitCompositor::deinitialize() {
    Processor::deinitialize();
}

} // inviwo namespace
