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
#ifdef USE_QWINDOW
#include <QtGui/QOpenGLContext>
#endif

namespace inviwo {

inline QGLContextFormat GetQGLFormat() {
  QGLContextFormat sharedFormat = QGLContextFormat(
#ifndef USE_QWINDOW
      QGL::Rgba | QGL::DoubleBuffer | QGL::AlphaChannel | QGL::DepthBuffer | QGL::StencilBuffer
#endif
      );
  sharedFormat.setProfile(QGLContextFormat::CoreProfile);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  sharedFormat.setVersion(10, 0);
#endif
  return sharedFormat;
}

QGLContextFormat CanvasQt::sharedFormat_ = GetQGLFormat();

#ifdef USE_QWINDOW
QOpenGLContext* CanvasQt::sharedGLContext_ = NULL;

CanvasQt::CanvasQt(QWindow* parent, uvec2 dim)
    : QWindow(parent)
    , CanvasGL(dim)
    , thisGLContext_(NULL)
    , swapBuffersAllowed_(false)
#ifndef QT_NO_GESTURES
    , gestureMode_(false)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    , lastNumFingers_(0)
#endif
#endif
{
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(sharedFormat_);
    create();

    thisGLContext_ = new QOpenGLContext(this);
    thisGLContext_->setFormat(sharedFormat_);

    //This is our default rendering context
    //Initialized once. So "THE" first object of this class will not have any shared context (or widget)
    //But Following objects, will share the context of initial object
    bool contextCreated;
    if (!sharedGLContext_) {
        contextCreated = thisGLContext_->create();
        sharedFormat_ = thisGLContext_->format();
        sharedGLContext_ = thisGLContext_;
        activate();
        initializeGL();
    }
    else{
        thisGLContext_->setShareContext(sharedGLContext_);
        contextCreated = thisGLContext_->create();
    }

    if (!contextCreated) {
        std::cout << "OpenGL context was not created successfully!" << std::endl;
        int major = sharedFormat_.majorVersion();
        int minor = sharedFormat_.minorVersion();
        std::cout << "GL Version: " << major << "." << minor << std::endl;
        std::cout << "GL Profile: " << (sharedFormat_.profile() == QSurfaceFormat::CoreProfile ? "Core" : "CompatibilityProfile") << std::endl;
        const GLubyte* vendor = glGetString(GL_VENDOR);
        std::string vendorStr = std::string((vendor!=NULL ? reinterpret_cast<const char*>(vendor) : "INVALID"));
        std::cout << "GL Vendor: " << vendorStr << std::endl;
    }
}
#else

QGLWidget* CanvasQt::sharedGLContext_ = NULL;

CanvasQt::CanvasQt(QWidget* parent, uvec2 dim)
    : QGLWidget(sharedFormat_, parent, sharedGLContext_)
      , CanvasGL(dim)
      , swapBuffersAllowed_(false)
#ifndef QT_NO_GESTURES
      , gestureMode_(false)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
      , lastNumFingers_(0)
#endif
#endif
{
    //This is our default rendering context
    //Initialized once. So "THE" first object of this class will not have any shared context (or widget)
    //But Following objects, will share the context of initial object
    if (!sharedGLContext_) {
        sharedFormat_ = this->format();
        sharedGLContext_ = this;
        QGLWidget::glInit();
    }

    setAutoBufferSwap(false);
    setFocusPolicy(Qt::TabFocus);

#ifndef QT_NO_GESTURES
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);
#endif
}
#endif

void CanvasQt::defineDefaultContextFormat(){
    if(!sharedGLContext_){
        std::string preferProfile = OpenGLCapabilities::getPreferredProfile();
        if(preferProfile == "core")
            sharedFormat_.setProfile(QGLContextFormat::CoreProfile);
        else if(preferProfile == "compatibility")
            sharedFormat_.setProfile(QGLContextFormat::CompatibilityProfile);
    }
}

CanvasQt::~CanvasQt() {}

void CanvasQt::initialize() {
    activate();
    CanvasGL::initialize();
}

void CanvasQt::initializeSquare() {
    activate();
    CanvasGL::initializeSquare();
}

void CanvasQt::deinitialize() {
    CanvasGL::deinitialize();
}

void CanvasQt::activate() {
#ifdef USE_QWINDOW
    thisGLContext_->makeCurrent(this);
#else
    makeCurrent();
#endif
}

void CanvasQt::initializeGL() {
    initializeGLEW();
#ifndef USE_QWINDOW
    QGLWidget::initializeGL();
    activate();
#endif
}

void CanvasQt::glSwapBuffers() {
    if (swapBuffersAllowed_) {
        activate();
#ifdef USE_QWINDOW
        thisGLContext_->swapBuffers(this);
#else
        QGLWidget::swapBuffers();
#endif
    }
}

void CanvasQt::update() {
    CanvasGL::update();
}

void CanvasQt::repaint() {
#ifndef USE_QWINDOW
    QGLWidget::updateGL();
#endif
}

void CanvasQt::paintGL() {
#ifdef USE_QWINDOW
    if (!isExposed())
        return;
#endif

    swapBuffersAllowed_ = true;
    CanvasGL::update();
}

bool CanvasQt::event(QEvent *e) {
    switch (e->type()) {
    case QEvent::KeyPress:
        keyPressEvent(static_cast<QKeyEvent*>(e));
        return true;
    case QEvent::KeyRelease:
        keyReleaseEvent(static_cast<QKeyEvent*>(e));
        return true;
    case QEvent::MouseButtonPress:
        mousePressEvent(static_cast<QMouseEvent*>(e));
        return true;
    case QEvent::MouseButtonRelease:
        mouseReleaseEvent(static_cast<QMouseEvent*>(e));
        return true;
    case QEvent::MouseMove:
        mouseMoveEvent(static_cast<QMouseEvent*>(e));
        return true;
    case QEvent::Wheel:
        wheelEvent(static_cast<QWheelEvent*>(e));
        return true;
#ifndef QT_NO_GESTURES
    case QEvent::TouchBegin:
    case QEvent::TouchEnd:
    case QEvent::TouchUpdate:
        touchEvent(static_cast<QTouchEvent*>(e));
        return true;
    case QEvent::Gesture:
        return gestureEvent(static_cast<QGestureEvent*>(e));
#endif
#ifdef USE_QWINDOW
    case QEvent::UpdateRequest:
        paintGL();
        return true;
#endif
    default:
        return QGLWindow::event(e);
    }
}

void CanvasQt::mousePressEvent(QMouseEvent* e) {
    if (!processorNetworkEvaluator_) return;

#ifndef QT_NO_GESTURES
    if (gestureMode_) return;
#endif

    MouseEvent* mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()),
                                            EventConverterQt::getMouseButton(e), MouseEvent::MOUSE_STATE_PRESS,
                                            EventConverterQt::getModifier(e), getScreenDimension());
    e->accept();
    Canvas::mousePressEvent(mouseEvent);
    delete mouseEvent;
}

void CanvasQt::mouseReleaseEvent(QMouseEvent* e) {
    if (!processorNetworkEvaluator_) return;

#ifndef QT_NO_GESTURES
    if (gestureMode_) return;
#endif

    MouseEvent* mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()),
                                            EventConverterQt::getMouseButton(e),MouseEvent::MOUSE_STATE_RELEASE,
                                            EventConverterQt::getModifier(e), getScreenDimension());
    e->accept();
    Canvas::mouseReleaseEvent(mouseEvent);
    delete mouseEvent;
}

void CanvasQt::mouseMoveEvent(QMouseEvent* e) {
    if (!processorNetworkEvaluator_) return;

#ifndef QT_NO_GESTURES
    if (gestureMode_) return;
#endif

    MouseEvent* mouseEvent = NULL;

    if (e->buttons() == Qt::LeftButton || e->buttons() == Qt::RightButton || e->buttons() == Qt::MiddleButton) {
        mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()),
                                    EventConverterQt::getMouseButton(e), MouseEvent::MOUSE_STATE_MOVE,
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
#ifdef USE_QWINDOW
    QWindow* parent = this->parent();
#else
    QWidget* parent = this->parentWidget();
#endif
    if(parent && e->key() == Qt::Key_F && e->modifiers() == Qt::ShiftModifier){
        if(parent->windowState() == Qt::WindowFullScreen) {
            parent->showNormal();
        } else {
            parent->showFullScreen();
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

#ifdef USE_QWINDOW
void CanvasQt::exposeEvent(QExposeEvent *e){
    Q_UNUSED(e);

    if (isExposed())
        paintGL();
}
#endif

#ifndef QT_NO_GESTURES

void CanvasQt::touchEvent(QTouchEvent* touch) {
    if (!processorNetworkEvaluator_) return;

    QTouchEvent::TouchPoint firstPoint = touch->touchPoints()[0];

    ivec2 pos = ivec2(static_cast<int>(glm::floor(firstPoint.pos().x())), static_cast<int>(glm::floor(firstPoint.pos().y())));
    TouchEvent::TouchState touchState;

    switch (firstPoint.state())
    {
    case Qt::TouchPointPressed:
        touchState = TouchEvent::TOUCH_STATE_STARTED;
    	break;
    case Qt::TouchPointMoved:
        touchState = TouchEvent::TOUCH_STATE_UPDATED;
        break;
    case Qt::TouchPointReleased:
        touchState = TouchEvent::TOUCH_STATE_ENDED;
        break;
    default:
        touchState = TouchEvent::TOUCH_STATE_NONE;
    }

    TouchEvent* touchEvent = new TouchEvent(pos, touchState);
    touch->accept();
    Canvas::touchEvent(touchEvent);
    delete touchEvent;

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    if(touch->touchPoints().size() == 1 && lastNumFingers_ < 2){
        MouseEvent* mouseEvent = NULL;
        switch (touchState)
        {
        case TouchEvent::TOUCH_STATE_STARTED:
            mouseEvent = new MouseEvent(pos, MouseEvent::MOUSE_BUTTON_LEFT, MouseEvent::MOUSE_STATE_PRESS, 
                EventConverterQt::getModifier(touch), getScreenDimension());
            Canvas::mousePressEvent(mouseEvent);
            break;
        case TouchEvent::TOUCH_STATE_UPDATED:
            mouseEvent = new MouseEvent(pos, MouseEvent::MOUSE_BUTTON_LEFT, MouseEvent::MOUSE_STATE_MOVE, 
                EventConverterQt::getModifier(touch), getScreenDimension());
            Canvas::mouseMoveEvent(mouseEvent);
            break;
        case TouchEvent::TOUCH_STATE_ENDED:
            mouseEvent = new MouseEvent(pos, MouseEvent::MOUSE_BUTTON_LEFT, MouseEvent::MOUSE_STATE_RELEASE, 
                EventConverterQt::getModifier(touch), getScreenDimension());
            Canvas::mouseReleaseEvent(mouseEvent);
            break;
        default:
            break;
        }
        delete mouseEvent;
    }

    lastNumFingers_ = static_cast<int>(touch->touchPoints().size());
#endif
}

bool CanvasQt::gestureEvent(QGestureEvent* ge) {
    gestureMode_ = true;
    if (QGesture* pan = ge->gesture(Qt::PanGesture))
        panTriggered(static_cast<QPanGesture *>(pan));
    else if (QGesture* pinch = ge->gesture(Qt::PinchGesture))
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
    ge->accept();
    return true;
}

void CanvasQt::panTriggered(QPanGesture* gesture) {
    if (!processorNetworkEvaluator_) return;

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
    GestureEvent* gestureEvent = new GestureEvent(
        vec2(2*(gesture->lastOffset().x()-gesture->offset().x())/getScreenDimension().x, 2*(gesture->offset().y()-gesture->lastOffset().y())/getScreenDimension().y), 0.0,
        GestureEvent::PAN, EventConverterQt::getGestureState(gesture));
    Canvas::gestureEvent(gestureEvent);
    delete gestureEvent;
}

void CanvasQt::pinchTriggered(QPinchGesture* gesture) {
    if (!processorNetworkEvaluator_) return;

    GestureEvent* gestureEvent = new GestureEvent(vec2(gesture->centerPoint().x(), gesture->centerPoint().y()), 1.0-gesture->scaleFactor(),
        GestureEvent::PINCH, EventConverterQt::getGestureState(gesture));
    Canvas::gestureEvent(gestureEvent);
    delete gestureEvent;
}

#endif

} // namespace