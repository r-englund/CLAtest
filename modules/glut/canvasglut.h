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

#ifndef IVW_CANVASGLUT_H
#define IVW_CANVASGLUT_H

#include "modules/glut/glutmoduledefine.h"
#include <stdlib.h>
#include "modules/opengl/inviwoopengl.h"
#include <GL/glut.h>
#include "modules/opengl/canvasgl.h"

namespace inviwo {

class IVW_MODULE_GLUT_API CanvasGLUT : public CanvasGL {
public:
    static EventHandler* eventHandler_;

    CanvasGLUT(std::string title, uvec2 dimensions);
    ~CanvasGLUT();

    void initialize();
    void initializeGL();
    void initializeSquare();
    void deinitialize();

    void glSwapBuffers();

    static void display(void);
    static void idle(void);
    static void reshape(int width, int height);

    static MouseEvent::MouseButton mapMouseButton(const int mouseButtonGLUT);
    static MouseEvent::MouseState mapMouseState(const int mouseStateGLUT);
    static InteractionEvent::Modifier mapModifiers(const int modifiersGLUT);

    static void keyboard(unsigned char key, int x, int y);
    static void keyboardSpecial(int key, int x, int y);
    static void mouse(int button, int state, int x, int y);
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
