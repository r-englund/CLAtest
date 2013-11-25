#include "cvie3dprocessor.h"

namespace inviwo {

ProcessorClassName(CVIE3DProcessor, "CVIE3DProcessor"); 
ProcessorCategory(CVIE3DProcessor, "Context Vision");
ProcessorCodeState(CVIE3DProcessor, CODE_STATE_EXPERIMENTAL); 

CVIE3DProcessor::CVIE3DProcessor()
    : Processor(),
    inport_("volume.inport"),
    outport_("volume.outport")
{
    addPort(inport_);
    addPort(outport_);
}

CVIE3DProcessor::~CVIE3DProcessor() {}

void CVIE3DProcessor::initialize() {
    Processor::initialize();
}

void CVIE3DProcessor::deinitialize() {
    Processor::deinitialize();
}

void CVIE3DProcessor::process() {

}

} // namespace
