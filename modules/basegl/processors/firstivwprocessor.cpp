/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include "firstivwprocessor.h"

namespace inviwo {

ProcessorClassName(FirstIvwProcessor, "FirstIVWProcessor"); 
ProcessorCategory(FirstIvwProcessor, "Various");
ProcessorCodeState(FirstIvwProcessor, CODE_STATE_STABLE);


FirstIvwProcessor::FirstIvwProcessor()
    : ProcessorGL()
    , color_("color", "Color", vec3(1.0f), vec3(0.0f), vec3(1.0f), vec3(0.1f))
    , outport_("outport", COLOR_ONLY) {
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
