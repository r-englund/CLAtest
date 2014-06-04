/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
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
 * Main file authors: Erik Sundén, Sathish Kottravel
 *
 *********************************************************************************/

#include <modules/openglqt/canvasqt.h>
#include <modules/opengl/openglcapabilities.h>

namespace inviwo {

inline QGLFormat GetQGLFormat() {
  QGLFormat sharedFormat = QGLFormat(QGL::Rgba | QGL::DoubleBuffer | QGL::AlphaChannel | QGL::DepthBuffer | QGL::StencilBuffer);
  sharedFormat.setProfile(QGLFormat::CoreProfile);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  sharedFormat.setVersion(10, 0);
#endif
  return sharedFormat;
}   

QGLWidget* CanvasQt::sharedWidget_ = NULL;
QGLFormat CanvasQt::sharedFormat_ = GetQGLFormat();

CanvasQt::CanvasQt(QWidget* parent, uvec2 dim)
    : QGLWidget(sharedFormat_, parent, sharedWidget_),
      CanvasGL(dim),
      swapBuffersAllowed_(false)
{
    //This is our default rendering context
    //Initialized once. So "THE" first object of this class will not have any shared context (or widget)
    //But Following objects, will share the context of initial object
    if (!sharedWidget_) {
        sharedFormat_ = this->format();
        sharedWidget_ = this;
        QGLWidget::glInit();
    }

    setAutoBufferSwap(false);
    setFocusPolicy(Qt::TabFocus);
    setAttribute(Qt::WA_OpaquePaintEvent);

#ifndef QT_NO_GESTURES
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);
#endif
}

void CanvasQt::defineDefaultContextFormat(){
    if(!sharedWidget_){
        std::string preferProfile = OpenGLCapabilities::getPreferredProfile();
        if(preferProfile == "core")
            sharedFormat_.setProfile(QGLFormat::CoreProfile);
        else if(preferProfile == "compatibility")
            sharedFormat_.setProfile(QGLFormat::CompatibilityProfile);
    }
}

CanvasQt::~CanvasQt() {}

void CanvasQt::initialize() {
    activate();
    CanvasGL::initialize();
}

void CanvasQt::initializeSquare() {
    CanvasGL::initializeSquare();
}

void CanvasQt::deinitialize() {
    CanvasGL::deinitialize();
}

void CanvasQt::activate() {
    makeCurrent();
}

void CanvasQt::initializeGL() {
    initializeGLEW();
    QGLWidget::initializeGL();
    activate();
}

void CanvasQt::glSwapBuffers() {
    if (swapBuffersAllowed_) {
        activate();
        QGLWidget::swapBuffers();
    }
}

void CanvasQt::update() {
    CanvasGL::update();
}

void CanvasQt::repaint() {
    QGLWidget::updateGL();
}

void CanvasQt::paintGL() {
    swapBuffersAllowed_ = true;
    CanvasGL::update();
}

bool CanvasQt::event(QEvent *e) {
#ifndef QT_NO_GESTURES
    if (e->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(e));
#endif
    return QGLWidget::event(e);
}

void CanvasQt::mousePressEvent(QMouseEvent* e) {
    if (!processorNetworkEvaluator_) return;

    MouseEvent* mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()),
                                            EventConverterQt::getMouseButton(e), MouseEvent::MOUSE_STATE_PRESS,
                                            EventConverterQt::getModifier(e), getScreenDimension());
    e->accept();
    Canvas::mousePressEvent(mouseEvent);
    delete mouseEvent;
}

void CanvasQt::mouseReleaseEvent(QMouseEvent* e) {
    if (!processorNetworkEvaluator_) return;

    MouseEvent* mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()),
                                            EventConverterQt::getMouseButton(e),MouseEvent::MOUSE_STATE_RELEASE,
                                            EventConverterQt::getModifier(e), getScreenDimension());
    e->accept();
    Canvas::mouseReleaseEvent(mouseEvent);
    delete mouseEvent;
}

void CanvasQt::mouseMoveEvent(QMouseEvent* e) {
    if (!processorNetworkEvaluator_) return;

    MouseEvent* mouseEvent = NULL;

    if (e->buttons() == Qt::LeftButton || e->buttons() == Qt::RightButton || e->buttons() == Qt::MiddleButton) {
        mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()),
                                    EventConverterQt::getMouseButton(e), MouseEvent::MOUSE_STATE_PRESS,
                                    EventConverterQt::getModifier(e), getScreenDimension());
        e->accept();
        Canvas::mouseMoveEvent(mouseEvent);
        delete mouseEvent;
    }
}

void CanvasQt::wheelEvent(QWheelEvent* e){
    if (!processorNetworkEvaluator_) return;

    MouseEvent::MouseWheelOrientation orientation;
    if (e->orientation() == Qt::Horizontal) {
        orientation = MouseEvent::MOUSE_WHEEL_HORIZONTAL;
    } else {
        orientation = MouseEvent::MOUSE_WHEEL_VERTICAL;
    }

    int numDegrees = e->delta() / 8;
    int numSteps = numDegrees / 15;

    MouseEvent* mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), numSteps,
        EventConverterQt::getMouseWheelButton(e), MouseEvent::MOUSE_STATE_WHEEL, orientation,
        EventConverterQt::getModifier(e), getScreenDimension());
    e->accept();
    Canvas::mouseWheelEvent(mouseEvent);
    delete mouseEvent;
}

void CanvasQt::keyPressEvent(QKeyEvent* e) {
    if(parentWidget() && e->key() == Qt::Key_F && e->modifiers() == Qt::ShiftModifier){
        if(parentWidget()->isFullScreen()) {
            parentWidget()->showNormal();
        } else {
            parentWidget()->showFullScreen();
        }
    }

    if (!processorNetworkEvaluator_) return;

    KeyboardEvent* keyEvent = new KeyboardEvent(
        EventConverterQt::getKeyButton(e),
        EventConverterQt::getModifier(e),
        KeyboardEvent::KEY_STATE_PRESS);
    e->accept();
    Canvas::keyPressEvent(keyEvent);
    delete keyEvent;
}

void CanvasQt::keyReleaseEvent(QKeyEvent* e) {
    if (!processorNetworkEvaluator_) return;

    KeyboardEvent* keyEvent = new KeyboardEvent(
        EventConverterQt::getKeyButton(e),
        EventConverterQt::getModifier(e),
        KeyboardEvent::KEY_STATE_RELEASE);
    e->accept();
    Canvas::keyReleaseEvent(keyEvent);
    delete keyEvent;
}

#ifndef QT_NO_GESTURES

bool CanvasQt::gestureEvent(QGestureEvent* e) {
    if (QGesture* pan = e->gesture(Qt::PanGesture))
        panTriggered(static_cast<QPanGesture *>(pan));
    if (QGesture* pinch = e->gesture(Qt::PinchGesture))
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
    e->accept();
    return true;
}

void CanvasQt::panTriggered(QPanGesture* gesture) {
#ifndef QT_NO_CURSOR
    switch (gesture->state()) {
         case Qt::GestureStarted:
         case Qt::GestureUpdated:
             setCursor(Qt::SizeAllCursor);
             break;
         default:
             setCursor(Qt::ArrowCursor);
    }
#endif

    GestureEvent* gestureEvent = new GestureEvent(vec2(gesture->delta().x(), gesture->delta().y()), 0.0,
        GestureEvent::PAN, EventConverterQt::getGestureState(gesture));
    Canvas::gestureEvent(gestureEvent);
    delete gestureEvent;
}

void CanvasQt::pinchTriggered(QPinchGesture* gesture) {
    GestureEvent* gestureEvent = new GestureEvent(vec2(gesture->centerPoint().x(), gesture->centerPoint().y()), (gesture->scaleFactor()-gesture->lastScaleFactor())*gesture->totalScaleFactor()/256,
        GestureEvent::PINCH, EventConverterQt::getGestureState(gesture));
    Canvas::gestureEvent(gestureEvent);
    delete gestureEvent;
}

#endif

} // namespace