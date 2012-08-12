#include "firstivwprocessor.h"

namespace inviwo {

FirstIvwProcessor::FirstIvwProcessor()
    : ProcessorGL(),
      inport_(Port::INPORT, "image.inport"),
      outport_(Port::OUTPORT, "image.outport")
{
    addPort(inport_);
    addPort(outport_);
}

FirstIvwProcessor::~FirstIvwProcessor() {}

Processor* FirstIvwProcessor::create() const {
    return new FirstIvwProcessor();
}

void FirstIvwProcessor::initialize() {
    Processor::initialize();
    shader_ = new Shader("img_identity.frag");
}

void FirstIvwProcessor::deinitialize() {
    delete shader_;
    Processor::deinitialize();
}

void FirstIvwProcessor::process() {
    activateTarget(outport_);

    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(-1.5f, 0.0f, -6.0f);
    glColor4f(0.0, 1.0, 0.0, 1.0);
    glBegin(GL_TRIANGLES);
        glVertex3f( 0.0f,  1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f( 1.0f, -1.0f, 0.0f);
    glEnd();
    glTranslatef(3.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex3f(-1.0f,  1.0f, 0.0f);
        glVertex3f( 1.0f,  1.0f, 0.0f);
        glVertex3f( 1.0f, -1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
    glEnd();

    deactivateCurrentTarget();
}

} // namespace
