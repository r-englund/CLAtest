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

#include "canvasglut.h"
#include "inviwo/core/network/processornetworkevaluator.h"

namespace inviwo {

static const unsigned int MAX_CANVAS_NUMBER = 256;
CanvasGLUT* CanvasGLUT::canvases_[MAX_CANVAS_NUMBER];

EventHandler* CanvasGLUT::eventHandler_;

CanvasGLUT::CanvasGLUT(std::string windowTitle, uvec2 dimensions)
    : CanvasGL(dimensions),
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
    glutInitWindowSize(dimensions_.x, dimensions_.y);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow(windowTitle_.c_str());

    canvasID_ = glutGetWindow();
    canvases_[canvasID_] = this;

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpecial);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutPassiveMotionFunc(mouseMotion);

    initializeGLEW();
}

void CanvasGLUT::initializeSquare(){
    CanvasGL::initializeSquare();
}

void CanvasGLUT::deinitialize() {
    CanvasGL::deinitialize();
}

void CanvasGLUT::glSwapBuffers() {
    glutSwapBuffers();
}

void CanvasGLUT::reshape(int width, int height) {
    canvases_[glutGetWindow()]->resize(uvec2(width, height));
}

void CanvasGLUT::display(void) {}

void CanvasGLUT::idle() {}

void CanvasGLUT::keyboard(unsigned char /*key*/, int /*x*/, int /*y*/) {
    KeyboardEvent* keyboardEvent = new KeyboardEvent();
    eventHandler_->broadcast(keyboardEvent);
}

void CanvasGLUT::keyboardSpecial(int /*key*/, int /*x*/, int /*y*/) {
    KeyboardEvent* keyboardEvent = new KeyboardEvent();
    eventHandler_->broadcast(keyboardEvent);
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
    MouseEvent* mouseEvent = new MouseEvent(ivec2(x, thisCanvas->dimensions_.y-y), thisCanvas->mouseButton_,
        thisCanvas->mouseState_, thisCanvas->mouseModifiers_, thisCanvas->dimensions_);
    if (thisCanvas->mouseState_ == MouseEvent::MOUSE_STATE_PRESS) canvases_[glutGetWindow()]->mousePressEvent(mouseEvent);
    else if (thisCanvas->mouseState_ == MouseEvent::MOUSE_STATE_RELEASE) canvases_[glutGetWindow()]->mouseReleaseEvent(mouseEvent);
    delete mouseEvent;
}

void CanvasGLUT::mouseMotion(int x, int y) {
    CanvasGLUT* thisCanvas = canvases_[glutGetWindow()];
    MouseEvent* mouseEvent = new MouseEvent(ivec2(x, thisCanvas->dimensions_.y-y), thisCanvas->mouseButton_,
                                            thisCanvas->mouseState_, thisCanvas->mouseModifiers_, thisCanvas->dimensions_);
    canvases_[glutGetWindow()]->mouseMoveEvent(mouseEvent);
    delete mouseEvent;
}

} // namespace
