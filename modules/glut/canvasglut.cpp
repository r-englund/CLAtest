/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2015 Inviwo Foundation
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
 *********************************************************************************/

#include "canvasglut.h"
#include <inviwo/core/network/processornetworkevaluator.h>

namespace inviwo {

static const unsigned int MAX_CANVAS_NUMBER = 256;
CanvasGLUT* CanvasGLUT::canvases_[MAX_CANVAS_NUMBER];

CanvasGLUT::CanvasGLUT(std::string windowTitle, uvec2 dimensions)
    : CanvasGL(dimensions), windowTitle_(windowTitle),
      mouseButton_(MouseEvent::MOUSE_BUTTON_NONE),
      mouseState_(MouseEvent::MOUSE_STATE_NONE),
      mouseModifiers_(InteractionEvent::MODIFIER_NONE)
{}

CanvasGLUT::~CanvasGLUT() {
    canvases_[canvasID_] = 0;
    glutDestroyWindow(canvasID_);
}

void CanvasGLUT::initialize() {
    CanvasGL::initialize();
}

void CanvasGLUT::initializeGL() {
    glutInitWindowSize(getScreenDimension().x, getScreenDimension().y);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow(windowTitle_.c_str());
    canvasID_ = glutGetWindow();
    canvases_[canvasID_] = this;
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMouseWheelFunc(mouseWheel);
    glutMotionFunc(mouseMotion);
    glutPassiveMotionFunc(mouseMotion);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
    initializeGLEW();
}

void CanvasGLUT::initializeSquare() {
    CanvasGL::initializeSquare();
}

void CanvasGLUT::deinitialize() {
    CanvasGL::deinitialize();
}

void CanvasGLUT::glSwapBuffers() {
    glutSwapBuffers();
}

void CanvasGLUT::setWindowTitle(std::string windowTitle) {
    windowTitle_ = windowTitle;
    glutSetWindowTitle(windowTitle_.c_str());
}

void CanvasGLUT::setWindowSize(uvec2 size) {
    glutReshapeWindow(static_cast<int>(size.x), static_cast<int>(size.y));
}

void CanvasGLUT::reshape(int width, int height) {
    canvases_[glutGetWindow()]->resize(uvec2(width, height));
}

void CanvasGLUT::display(void) {}

void CanvasGLUT::idle() {}

void CanvasGLUT::keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC key
            exit(0);
            break;
    }

    CanvasGLUT* thisCanvas = canvases_[glutGetWindow()];

    KeyboardEvent* keyEvent = new KeyboardEvent(
        toupper(key),
        mapModifiers(glutGetModifiers()),
        KeyboardEvent::KEY_STATE_PRESS);
    
    canvases_[glutGetWindow()]->keyPressEvent(keyEvent);

    delete keyEvent;
}

MouseEvent::MouseButton CanvasGLUT::mapMouseButton(int mouseButtonGLUT) {
    if (mouseButtonGLUT == GLUT_LEFT_BUTTON) return MouseEvent::MOUSE_BUTTON_LEFT;
    else if (mouseButtonGLUT == GLUT_MIDDLE_BUTTON) return MouseEvent::MOUSE_BUTTON_MIDDLE;
    else if (mouseButtonGLUT == GLUT_RIGHT_BUTTON) return MouseEvent::MOUSE_BUTTON_RIGHT;
    else return MouseEvent::MOUSE_BUTTON_NONE;
}

MouseEvent::MouseState CanvasGLUT::mapMouseState(int mouseStateGLUT) {
    if (mouseStateGLUT == GLUT_DOWN) return MouseEvent::MOUSE_STATE_PRESS;
    else if (mouseStateGLUT == GLUT_UP) return MouseEvent::MOUSE_STATE_RELEASE;
    else return MouseEvent::MOUSE_STATE_NONE;
}

InteractionEvent::Modifier CanvasGLUT::mapModifiers(int modifiersGLUT) {
    int result = KeyboardEvent::MODIFIER_NONE;

    if (modifiersGLUT & GLUT_ACTIVE_ALT) result |= InteractionEvent::MODIFIER_ALT;

    if (modifiersGLUT & GLUT_ACTIVE_CTRL) result |= InteractionEvent::MODIFIER_CTRL;

    if (modifiersGLUT & GLUT_ACTIVE_SHIFT) result |= InteractionEvent::MODIFIER_SHIFT;

    return static_cast<InteractionEvent::Modifier>(result);
}

void CanvasGLUT::mouse(int button, int state, int x, int y) {
    CanvasGLUT* thisCanvas = canvases_[glutGetWindow()];
    thisCanvas->mouseButton_ = mapMouseButton(button);
    thisCanvas->mouseState_ = mapMouseState(state);
    thisCanvas->mouseModifiers_ = mapModifiers(glutGetModifiers());
    MouseEvent* mouseEvent =
        new MouseEvent(ivec2(x, y), thisCanvas->mouseButton_, thisCanvas->mouseState_,
                       thisCanvas->mouseModifiers_, thisCanvas->getScreenDimension());

    if (thisCanvas->mouseState_ == MouseEvent::MOUSE_STATE_PRESS)
        canvases_[glutGetWindow()]->mousePressEvent(mouseEvent);
    else if (thisCanvas->mouseState_ == MouseEvent::MOUSE_STATE_RELEASE)
        canvases_[glutGetWindow()]->mouseReleaseEvent(mouseEvent);

    delete mouseEvent;
}

void CanvasGLUT::mouseWheel(int button, int direction, int x, int y) {
    CanvasGLUT* thisCanvas = canvases_[glutGetWindow()];
    thisCanvas->mouseButton_ = mapMouseButton(button);
    thisCanvas->mouseState_ = MouseEvent::MOUSE_STATE_WHEEL;
    thisCanvas->mouseModifiers_ = mapModifiers(glutGetModifiers());
    MouseEvent* mouseEvent =
        new MouseEvent(ivec2(x, y), direction, thisCanvas->mouseButton_, thisCanvas->mouseState_,
                       MouseEvent::MOUSE_WHEEL_VERTICAL, thisCanvas->mouseModifiers_,
                       thisCanvas->getScreenDimension());

    canvases_[glutGetWindow()]->mouseWheelEvent(mouseEvent);

    delete mouseEvent;
}

void CanvasGLUT::mouseMotion(int x, int y) {
    CanvasGLUT* thisCanvas = canvases_[glutGetWindow()];
    MouseEvent* mouseEvent =
        new MouseEvent(ivec2(x, y), thisCanvas->mouseButton_, thisCanvas->mouseState_,
                       thisCanvas->mouseModifiers_, thisCanvas->getScreenDimension());
    canvases_[glutGetWindow()]->mouseMoveEvent(mouseEvent);
    delete mouseEvent;
}

} // namespace
