#include "volumesplitimagecompositor.h"

namespace inviwo {

VolumeSplitImageCompositor::VolumeSplitImageCompositor()
    : ProcessorGL(),
      inport_("image.in"),
      outport_("image.out")
{
    addPort(inport_);
    addPort(outport_);

    pingPongMod = 0;
}

VolumeSplitImageCompositor::~VolumeSplitImageCompositor() {
}

Processor* VolumeSplitImageCompositor::create() const {
    return new VolumeSplitImageCompositor();
}

void VolumeSplitImageCompositor::process() {
}

void VolumeSplitImageCompositor::compile() {
}

void VolumeSplitImageCompositor::initialize() {
    ProcessorGL::initialize();
}

void VolumeSplitImageCompositor::deinitialize() {
    ProcessorGL::deinitialize();
}

} // inviwo namespace
