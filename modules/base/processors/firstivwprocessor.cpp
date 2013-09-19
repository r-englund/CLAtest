#include "firstivwprocessor.h"

namespace inviwo {

ProcessorClassName(FirstIvwProcessor, "FirstIVWProcessor"); 
ProcessorCategory(FirstIvwProcessor, "Various");
ProcessorCodeState(FirstIvwProcessor, CODE_STATE_STABLE);


FirstIvwProcessor::FirstIvwProcessor()
    : ProcessorGL(),
      outport_("outport", COLOR_ONLY),
      color_("color", "Color", vec3(1.0f), vec3(0.0f), vec3(1.0f), vec3(0.1f))
{
    addProperty(color_);
    addPort(outport_);
}

void FirstIvwProcessor::process() {
    activateAndClearTarget(outport_);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glColor4f(color_.get().x, color_.get().y, color_.get().z, 1.0);
    glBegin(GL_QUADS);
        glVertex3f(-1.0f,  1.0f, 0.999f);
        glVertex3f( 1.0f,  1.0f, 0.999f);
        glVertex3f( 1.0f, -1.0f, 0.999f);
        glVertex3f(-1.0f, -1.0f, 0.999f);
    glEnd();

    glColor4f(0.0, 1.0, 0.0, 1.0);
    glBegin(GL_TRIANGLES);
        glVertex3f( 0.0f,  1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f( 1.0f, -1.0f, 0.0f);
    glEnd();
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    deactivateCurrentTarget();
}

} // namespace
