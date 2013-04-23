#include <inviwo/qt/widgets/canvasqt.h>

namespace inviwo {

QGLWidget* CanvasQt::sharedWidget_ = 0;

CanvasQt::CanvasQt(QWidget* parent)
: QGLWidget(QGLFormat(QGL::Rgba | QGL::SingleBuffer | QGL::DepthBuffer), parent, sharedWidget_),
  CanvasGL(uvec2(256,256))
{
    //This is our default rendering context
    //Initialized once. So "THE" first object of this class will not have any shared context (or widget)
    //But Following objects, will share the context of initial object
    if (!sharedWidget_){
        sharedWidget_ = this;
        QGLWidget::glInit();
    }
    //setAutoBufferSwap(false);
    eventConverter_ = new EventConverterQt();
}

CanvasQt::~CanvasQt() {}

void CanvasQt::initialize() {
    CanvasGL::initialize();
    QObject::startTimer(16);
    show();
}

void CanvasQt::activate() {
    if (context() != QGLContext::currentContext())
        makeCurrent();
}

void CanvasQt::initializeGL() {
    CanvasGL::initializeGL();
    QGLWidget::initializeGL();
}

void CanvasQt::resizeGL(int width, int height) {
    CanvasGL::resize(uvec2(static_cast<uint32_t>(width), static_cast<uint32_t>(height)));
    if(processorNetworkEvaluator_) {
        ResizeEvent* resizeEvent = new ResizeEvent(dimensions_);
        processorNetworkEvaluator_->propagateResizeEvent(this, resizeEvent);
    }
}

void CanvasQt::paintGL() {
    //QGLWidget::paintGL();
    //QGLWidget::swapBuffers();
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
    if (processorNetworkEvaluator_)
        processorNetworkEvaluator_->evaluate();
}

void CanvasQt::mousePressEvent(QMouseEvent* e) {
    if(!processorNetworkEvaluator_) return;
    MouseEvent* mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), 
        eventConverter_->getMouseButton(e), MouseEvent::MOUSE_STATE_PRESS, 
        eventConverter_->getModifier(e), dimensions_);
    processorNetworkEvaluator_->propagateMouseEvent(this, mouseEvent);
}

void CanvasQt::mouseReleaseEvent (QMouseEvent* e) {
    if(!processorNetworkEvaluator_) return;
    MouseEvent* mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), 
        eventConverter_->getMouseButton(e),MouseEvent::MOUSE_STATE_RELEASE, 
        eventConverter_->getModifier(e), dimensions_);
    processorNetworkEvaluator_->propagateMouseEvent(this, mouseEvent);
}

void CanvasQt::mouseMoveEvent(QMouseEvent*  e) {
    if(!processorNetworkEvaluator_) return;

    MouseEvent* mouseEvent = 0;
    if(e->buttons() == Qt::LeftButton || e->buttons() == Qt::RightButton || e->buttons() == Qt::MiddleButton) {
        mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), 
            eventConverter_->getMouseButton(e), MouseEvent::MOUSE_STATE_PRESS, 
            eventConverter_->getModifier(e), dimensions_);
    }
    else {
        mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), 
            MouseEvent::MOUSE_BUTTON_NONE, MouseEvent::MOUSE_STATE_NONE, 
            eventConverter_->getModifier(e), dimensions_);
    }
    processorNetworkEvaluator_->propagateMouseEvent(this, mouseEvent);
}
} // namespace