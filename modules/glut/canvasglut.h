/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Contact: Timo Ropinski
 *
 *********************************************************************************/

#ifndef IVW_CANVASGLUT_H
#define IVW_CANVASGLUT_H

#include "modules/glut/glutmoduledefine.h"
#include <stdlib.h>
#include "modules/opengl/inviwoopengl.h"
#include <GL/freeglut.h>
#include "modules/opengl/canvasgl.h"

namespace inviwo {

class IVW_MODULE_GLUT_API CanvasGLUT : public CanvasGL {
public:
    CanvasGLUT(std::string title, uvec2 dimensions);
    ~CanvasGLUT();

    void initialize();
    void initializeGL();
    void initializeSquare();
    void deinitialize();

    void glSwapBuffers();

    void setWindowTitle(std::string);
    void setWindowSize(uvec2);

    static void display(void);
    static void idle(void);
    static void reshape(int width, int height);

    static MouseEvent::MouseButton mapMouseButton(const int mouseButtonGLUT);
    static MouseEvent::MouseState mapMouseState(const int mouseStateGLUT);
    static InteractionEvent::Modifier mapModifiers(const int modifiersGLUT);

    static void keyboard(unsigned char key, int x, int y);
    static void mouse(int button, int state, int x, int y);
    static void mouseWheel(int button, int state, int x, int y);
    static void mouseMotion(int x, int y);

private:
    std::string windowTitle_;
    unsigned int canvasID_;

    MouseEvent::MouseButton mouseButton_;
    MouseEvent::MouseState mouseState_;
    InteractionEvent::Modifier mouseModifiers_;

    // these members are used to deal with the static
    // callback functions available through GLUT
    static unsigned int canvasCount_;
    static CanvasGLUT* canvases_[];
};

} // namespace

#endif // IVW_CANVASGLUT_H
