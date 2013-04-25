#include "firstivwprocessor.h"

namespace inviwo {

FirstIvwProcessor::FirstIvwProcessor()
    : ProcessorGL(),
      outport_("outport"),
      color_("color", "Color", vec3(1.0f), vec3(0.0f), vec3(1.0f), vec3(0.1f))
{
    addPort(outport_);
    addProperty(color_);
}

FirstIvwProcessor::~FirstIvwProcessor() {}

Processor* FirstIvwProcessor::create() const {
    return new FirstIvwProcessor();
}

void FirstIvwProcessor::initialize() {
    ProcessorGL::initialize();
}

void FirstIvwProcessor::deinitialize() {
    ProcessorGL::deinitialize();
}

void FirstIvwProcessor::process() {
    activateTarget(outport_);

    //glClearColor(color_.get().x, color_.get().y, color_.get().z, 1.0);
    //glClearColor(1.0*color_.get(), 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    glColor4f(color_.get().x, color_.get().y, color_.get().z, 1.0);
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
