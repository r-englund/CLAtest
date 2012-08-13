#include "firstivwprocessor.h"

namespace inviwo {

FirstIvwProcessor::FirstIvwProcessor()
    : ProcessorGL(),
      outport_(Port::OUTPORT, "outport")
{
    addPort(outport_);
}

FirstIvwProcessor::~FirstIvwProcessor() {}

Processor* FirstIvwProcessor::create() const {
    return new FirstIvwProcessor();
}

void FirstIvwProcessor::initialize() {
    Processor::initialize();
}

void FirstIvwProcessor::deinitialize() {
    Processor::deinitialize();
}

void FirstIvwProcessor::process() {
    activateTarget(outport_);

    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glColor4f(1.0, 0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
        glVertex3f(-1.0f,  1.0f, 0.0f);
        glVertex3f( 1.0f,  1.0f, 0.0f);
        glVertex3f( 1.0f, -1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
    glEnd();
    glColor4f(0.0, 1.0, 0.0, 1.0);
    glBegin(GL_TRIANGLES);
    glVertex3f( 0.0f,  1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f, 0.0f);
    glEnd();

    deactivateCurrentTarget();
}

} // namespace
