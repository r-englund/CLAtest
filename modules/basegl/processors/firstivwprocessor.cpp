 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2013 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file authors: Timo Ropinski, Erik Sundén
 *
 *********************************************************************************/

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
