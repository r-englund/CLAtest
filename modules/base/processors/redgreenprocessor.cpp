#include "redgreenprocessor.h"

namespace inviwo {

ProcessorClassName(RedGreenProcessor, "RedGreenProcessor"); 
ProcessorCategory(RedGreenProcessor, "Various");
ProcessorCodeState(RedGreenProcessor, CODE_STATE_STABLE);

RedGreenProcessor::RedGreenProcessor()
    : ProcessorGL(),
    outportRed_("outportRed"),
    outportGreen_("outportGreen")
{
    addPort(outportRed_);
    addPort(outportGreen_);
}

RedGreenProcessor::~RedGreenProcessor() {}

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
    deactivateCurrentTarget();

    activateTarget(outportGreen_);
    glClearColor(0.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    deactivateCurrentTarget();
}

} // namespace
