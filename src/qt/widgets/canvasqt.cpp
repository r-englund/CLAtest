#include <QMouseEvent>

#include "inviwo/core/inviwo.h"
#include "inviwo/qt/widgets/canvasqt.h"
#include "inviwo/core/network/processornetworkevaluator.h"

namespace inviwo {

bool CanvasQt::glewInitialized_ = false;
QGLWidget* CanvasQt::sharedWidget_ = 0;
const std::string CanvasQt::logSource_ = "CanvasQt";

CanvasQt::CanvasQt(QWidget* parent)
: QGLWidget(QGLFormat(QGL::Rgba | QGL::DoubleBuffer | QGL::DepthBuffer), parent, sharedWidget_),
  CanvasGL(ivec2(256,256))
{
    //This is our default rendering context
    //Intialized once. So "THE" first object of this class will not have any shared context (or widget)
    //But Following objects, will share the context of initial object
    if(!sharedWidget_)
        sharedWidget_ = this;
    //
}

CanvasQt::~CanvasQt() {}

void CanvasQt::initialize() {
    CanvasGL::initialize();
    QObject::startTimer(1);
    show();
}

void CanvasQt::switchContext() {
    if( context() != context()->currentContext() ) {
        //LogInfo("Switching Context");
        QGLWidget::doneCurrent();
        QGLWidget::makeCurrent();
    }
}

void CanvasQt::initializeGL() {    
    if (!glewInitialized_) {
        LogInfo("Initializing GLEW");
        glewInit();
        glewInitialized_ = true;
    }
    QGLWidget::initializeGL();
}

void CanvasQt::resizeGL(int width, int height) {
    CanvasGL::resize(ivec2(width, height));
    if(processorNetworkEvaluator_) {
        ResizeEvent* resizeEvent = new ResizeEvent(dimensions_);
        processorNetworkEvaluator_->propagateResizeEvent(this, resizeEvent);
    }
}

void CanvasQt::paintGL() {
    //QGLWidget::paintGL();
}

void CanvasQt::update() {
    //QGLWidget::updateGL();
}

void CanvasQt::repaint() {
    //CanvasGL::repaint();
    QGLWidget::repaint();
}


void CanvasQt::timerEvent(QTimerEvent* e) {
    IVW_UNUSED_PARAM(e);
    if(processorNetworkEvaluator_) {
        processorNetworkEvaluator_->evaluate();
        if (processorNetworkEvaluator_->repaintRequired())
            update();
    }
}

MouseEvent::MouseButton CanvasQt::getMouseButton(QMouseEvent* e) {
     if(e->buttons() == Qt::LeftButton) {
         return MouseEvent::MOUSE_BUTTON_LEFT;
     }
     else if(e->buttons() == Qt::RightButton) {
         return MouseEvent::MOUSE_BUTTON_RIGHT;
     }
     else if(e->buttons() == Qt::MiddleButton) {
         return MouseEvent::MOUSE_BUTTON_MIDDLE;
     }
     return MouseEvent::MOUSE_BUTTON_NONE;
}

void CanvasQt::mousePressEvent(QMouseEvent* e) {
    if(!processorNetworkEvaluator_) return;
    MouseEvent* mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), getMouseButton(e),
        MouseEvent::MOUSE_STATE_PRESS, MouseEvent::MODIFIER_NONE, dimensions_);
    processorNetworkEvaluator_->propagateMouseEvent(this, mouseEvent);
}

void CanvasQt::mouseReleaseEvent (QMouseEvent* e) {
    if(!processorNetworkEvaluator_) return;
    MouseEvent* mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), getMouseButton(e),
        MouseEvent::MOUSE_STATE_RELEASE, MouseEvent::MODIFIER_NONE, dimensions_);
    processorNetworkEvaluator_->propagateMouseEvent(this, mouseEvent);
}

void CanvasQt::mouseMoveEvent(QMouseEvent*  e) {
    if(!processorNetworkEvaluator_) return;

    MouseEvent* mouseEvent = 0;
    if(e->buttons() == Qt::LeftButton || e->buttons() == Qt::RightButton) {
        mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), getMouseButton(e),
        MouseEvent::MOUSE_STATE_PRESS, MouseEvent::MODIFIER_NONE, dimensions_);
    }
    else {
        mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), MouseEvent::MOUSE_BUTTON_NONE,
        MouseEvent::MOUSE_STATE_NONE, MouseEvent::MODIFIER_NONE, dimensions_);
    }
    processorNetworkEvaluator_->propagateMouseEvent(this, mouseEvent);
}

} // namespace