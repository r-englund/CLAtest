#include "redgreenprocessor.h"

namespace inviwo {

RedGreenProcessor::RedGreenProcessor()
    : ProcessorGL(),
    outportRed_(Port::OUTPORT, "outportRed"),
    outportGreen_(Port::OUTPORT, "outportGreen")
{
    addPort(outportRed_);
    addPort(outportGreen_);
}

RedGreenProcessor::~RedGreenProcessor() {
    Processor::~Processor();
}

Processor* RedGreenProcessor::create() const {
    return new RedGreenProcessor();
}

void RedGreenProcessor::initialize() {
    Processor::initialize();
}

void RedGreenProcessor::deinitialize() {
    Processor::deinitialize();
}

void RedGreenProcessor::process() {
    activateTarget(outportRed_);
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    activateTarget(outportGreen_);
    glClearColor(0.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    deactivateCurrentTarget();
}

} // namespace
