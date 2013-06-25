#include "volumesplitimagecompositor.h"

namespace inviwo {

ProcessorClassName(VolumeSplitImageCompositor, "VolumeSplitImageCompositor"); 
ProcessorCategory(VolumeSplitImageCompositor, "Volume Operation");
ProcessorCodeState(VolumeSplitImageCompositor, CODE_STATE_BROKEN);

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
