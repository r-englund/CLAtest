#include "canvasglut.h"
#include "inviwo/core/network/processornetworkevaluator.h"

namespace inviwo {

static const unsigned int MAX_CANVAS_NUMBER = 256;
CanvasGLUT* CanvasGLUT::canvases_[MAX_CANVAS_NUMBER];

EventHandler* CanvasGLUT::eventHandler_;

//ProcessorNetworkEvaluator* CanvasGLUT::processorNetworkEvaluator_ = 0;

CanvasGLUT::CanvasGLUT(std::string windowTitle, uvec2 dimensions)
    : CanvasGL(dimensions),
    mouseButton_(MouseEvent::MOUSE_BUTTON_NONE),
    mouseState_(MouseEvent::MOUSE_STATE_NONE),
    mouseModifiers_(Event::MODIFIER_NONE)
{}

CanvasGLUT::~CanvasGLUT() {
    canvases_[canvasID_] = 0;
    glutDestroyWindow(canvasID_);
}

void CanvasGLUT::initialize() {
    glutInitWindowSize(dimensions_.x, dimensions_.y);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
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

    initializeGL();
    
    CanvasGL::initialize();
}

void CanvasGLUT::initializeGL() {
    CanvasGL::initializeGL();
}

void CanvasGLUT::deinitialize() {
    CanvasGL::deinitialize();
}

void CanvasGLUT::reshape(int width, int height) {
    canvases_[glutGetWindow()]->dimensions_ = ivec2(width, height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (height == 0) gluPerspective(60.0, static_cast<double>(width), 0.1, 10.0);
    else gluPerspective (60.0, static_cast<double>(width) / static_cast<double>(height), 0.1, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutPostRedisplay();
}

void CanvasGLUT::display(void) {
    glutSwapBuffers();
}

void CanvasGLUT::idle() {
    //FIXME: update() already called in network evaluator?!
    /*processorNetworkEvaluator_->evaluate();
    if (processorNetworkEvaluator_->repaintRequired())
        glutPostRedisplay();
    */
}

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

Event::Modifier CanvasGLUT::mapModifiers(int modifiersGLUT) {
    int result = KeyboardEvent::MODIFIER_NONE;
    if (modifiersGLUT & GLUT_ACTIVE_ALT) result |= KeyboardEvent::MODIFIER_ALT;
    if (modifiersGLUT & GLUT_ACTIVE_CTRL) result |= KeyboardEvent::MODIFIER_CTRL;
    if (modifiersGLUT & GLUT_ACTIVE_SHIFT) result |= KeyboardEvent::MODIFIER_SHIFT;
    return static_cast<Event::Modifier>(result);
}

void CanvasGLUT::mouse(int button, int state, int x, int y) {
    CanvasGLUT* thisCanvas = canvases_[glutGetWindow()];
    thisCanvas->mouseButton_ = mapMouseButton(button);
    thisCanvas->mouseState_ = mapMouseState(state);
    thisCanvas->mouseModifiers_ = mapModifiers(glutGetModifiers());
    MouseEvent* mouseEvent = new MouseEvent(ivec2(x, thisCanvas->dimensions_.y-y), thisCanvas->mouseButton_,
        thisCanvas->mouseState_, thisCanvas->mouseModifiers_, thisCanvas->dimensions_);
    canvases_[glutGetWindow()]->processorNetworkEvaluator_->propagateMouseEvent(canvases_[glutGetWindow()], mouseEvent);
}

void CanvasGLUT::mouseMotion(int x, int y) {
    CanvasGLUT* thisCanvas = canvases_[glutGetWindow()];
    MouseEvent* mouseEvent = new MouseEvent(ivec2(x, thisCanvas->dimensions_.y-y), thisCanvas->mouseButton_,
                                            thisCanvas->mouseState_, thisCanvas->mouseModifiers_, thisCanvas->dimensions_);
    canvases_[glutGetWindow()]->processorNetworkEvaluator_->propagateMouseEvent(canvases_[glutGetWindow()], mouseEvent);
}

} // namespace
