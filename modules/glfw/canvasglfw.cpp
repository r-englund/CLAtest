/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
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
 * Main file authors: Erik Sundén
 *
 *********************************************************************************/

#include "canvasglfw.h"
#include "inviwo/core/network/processornetworkevaluator.h"

namespace inviwo {

CanvasGLFW::CanvasGLFW(std::string windowTitle, uvec2 dimensions)
    : CanvasGL(dimensions), windowTitle_(windowTitle),
      glWindow_(NULL),
      mouseButton_(MouseEvent::MOUSE_BUTTON_NONE),
      mouseState_(MouseEvent::MOUSE_STATE_NONE),
      mouseModifiers_(InteractionEvent::MODIFIER_NONE)
{}

CanvasGLFW::~CanvasGLFW() {
    glfwDestroyWindow(glWindow_);
}

void CanvasGLFW::initialize() {
    CanvasGL::initialize();
}

void CanvasGLFW::initializeGL() {
    glWindow_ = glfwCreateWindow(getDimension().x, getDimension().y, windowTitle_.c_str(), NULL, NULL);

    if (!glWindow_){
        glfwTerminate();
        LogError("Could not create GLFW window.");
    }

    glfwSetWindowUserPointer(glWindow_, this);
    glfwSetWindowSizeCallback(glWindow_, reshape);
    glfwMakeContextCurrent(glWindow_);

    initializeGLEW();
}

void CanvasGLFW::initializeSquare() {
    CanvasGL::initializeSquare();
}

void CanvasGLFW::deinitialize() {
    CanvasGL::deinitialize();
}

void CanvasGLFW::glSwapBuffers() {
    glfwSwapBuffers(glWindow_);
}

void CanvasGLFW::setWindowTitle(std::string windowTitle) {
    windowTitle_ = windowTitle;
    glfwSetWindowTitle(glWindow_, windowTitle_.c_str());
}

void CanvasGLFW::setWindowSize(uvec2 size) {
    glfwSetWindowSize(glWindow_, static_cast<int>(size.x), static_cast<int>(size.y));
}

//void CanvasGLFW::refresh(void) {}

void CanvasGLFW::reshape(GLFWwindow* window, int width, int height) {
    getCanvasGLFW(window)->resize(uvec2(width, height));
}

CanvasGLFW* CanvasGLFW::getCanvasGLFW(GLFWwindow* window){
    return static_cast<CanvasGLFW*>(glfwGetWindowUserPointer(window));
}

//void CanvasGLFW::idle() {}

/*void CanvasGLFW::keyboard(unsigned char key, int x, int y) {
    CanvasGLFW* thisCanvas = canvases_[glfwGetWindow()];

    KeyboardEvent* keyEvent = new KeyboardEvent(
        key,
        mapModifiers(glfwGetModifiers()),
        KeyboardEvent::KEY_STATE_PRESS);
    
    canvases_[glfwGetWindow()]->keyPressEvent(keyEvent);

    delete keyEvent;
}

MouseEvent::MouseButton CanvasGLFW::mapMouseButton(int mouseButtonGLFW) {
    if (mouseButtonGLFW == GLFW_LEFT_BUTTON) return MouseEvent::MOUSE_BUTTON_LEFT;
    else if (mouseButtonGLFW == GLFW_MIDDLE_BUTTON) return MouseEvent::MOUSE_BUTTON_MIDDLE;
    else if (mouseButtonGLFW == GLFW_RIGHT_BUTTON) return MouseEvent::MOUSE_BUTTON_RIGHT;
    else return MouseEvent::MOUSE_BUTTON_NONE;
}

MouseEvent::MouseState CanvasGLFW::mapMouseState(int mouseStateGLFW) {
    if (mouseStateGLFW == GLFW_DOWN) return MouseEvent::MOUSE_STATE_PRESS;
    else if (mouseStateGLFW == GLFW_UP) return MouseEvent::MOUSE_STATE_RELEASE;
    else return MouseEvent::MOUSE_STATE_NONE;
}

InteractionEvent::Modifier CanvasGLFW::mapModifiers(int modifiersGLFW) {
    int result = KeyboardEvent::MODIFIER_NONE;

    if (modifiersGLFW & GLFW_ACTIVE_ALT) result |= InteractionEvent::MODIFIER_ALT;

    if (modifiersGLFW & GLFW_ACTIVE_CTRL) result |= InteractionEvent::MODIFIER_CTRL;

    if (modifiersGLFW & GLFW_ACTIVE_SHIFT) result |= InteractionEvent::MODIFIER_SHIFT;

    return static_cast<InteractionEvent::Modifier>(result);
}

void CanvasGLFW::mouse(int button, int state, int x, int y) {
    CanvasGLFW* thisCanvas = canvases_[glfwGetWindow()];
    thisCanvas->mouseButton_ = mapMouseButton(button);
    thisCanvas->mouseState_ = mapMouseState(state);
    thisCanvas->mouseModifiers_ = mapModifiers(glfwGetModifiers());
    MouseEvent* mouseEvent = new MouseEvent(ivec2(x, y), thisCanvas->mouseButton_,
                                            thisCanvas->mouseState_, thisCanvas->mouseModifiers_, thisCanvas->dimensions_);

    if (thisCanvas->mouseState_ == MouseEvent::MOUSE_STATE_PRESS) canvases_[glfwGetWindow()]->mousePressEvent(mouseEvent);
    else if (thisCanvas->mouseState_ == MouseEvent::MOUSE_STATE_RELEASE) canvases_[glfwGetWindow()]->mouseReleaseEvent(mouseEvent);

    delete mouseEvent;
}

void CanvasGLFW::mouseWheel(int button, int direction, int x, int y) {
    CanvasGLFW* thisCanvas = canvases_[glfwGetWindow()];
    thisCanvas->mouseButton_ = mapMouseButton(button);
    thisCanvas->mouseState_ = MouseEvent::MOUSE_STATE_WHEEL;
    thisCanvas->mouseModifiers_ = mapModifiers(glfwGetModifiers());
    MouseEvent* mouseEvent = new MouseEvent(ivec2(x, y), direction, thisCanvas->mouseButton_,
        thisCanvas->mouseState_, MouseEvent::MOUSE_WHEEL_VERTICAL, thisCanvas->mouseModifiers_, thisCanvas->dimensions_);

    canvases_[glfwGetWindow()]->mouseWheelEvent(mouseEvent);

    delete mouseEvent;
}

void CanvasGLFW::mouseMotion(int x, int y) {
    CanvasGLFW* thisCanvas = canvases_[glfwGetWindow()];
    MouseEvent* mouseEvent = new MouseEvent(ivec2(x, y), thisCanvas->mouseButton_,
                                            thisCanvas->mouseState_, thisCanvas->mouseModifiers_, thisCanvas->dimensions_);
    canvases_[glfwGetWindow()]->mouseMoveEvent(mouseEvent);
    delete mouseEvent;
}*/

} // namespace
