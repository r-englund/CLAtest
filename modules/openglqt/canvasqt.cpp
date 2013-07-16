#include <modules/openglqt/canvasqt.h>

namespace inviwo {

QGLWidget* CanvasQt::sharedWidget_ = 0;

CanvasQt::CanvasQt(QWidget* parent, QGLFormat glFormat)
    : QGLWidget(glFormat, parent, sharedWidget_),
    CanvasGL(uvec2(256,256))
{ 
    if (!sharedWidget_){
        sharedWidget_ = this;
        QGLWidget::glInit();
        //LogInfo("GL Major: "<< format().majorVersion() <<" GL Minor: "<< format().minorVersion());     
    }

}

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
}

CanvasQt::~CanvasQt() {
    //QGLContext::currentContext();
}

void CanvasQt::initialize() {
    activate();
    CanvasGL::initialize();
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
}

void CanvasQt::update() {
    CanvasGL::update();
}

void CanvasQt::repaint() {
    //CanvasGL::repaint();
    QGLWidget::repaint();
}


void CanvasQt::mousePressEvent(QMouseEvent* e) {
    if (!processorNetworkEvaluator_) return;
    MouseEvent* mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), 
        EventConverterQt::getMouseButton(e), MouseEvent::MOUSE_STATE_PRESS, 
        EventConverterQt::getModifier(e), dimensions_);
    processorNetworkEvaluator_->propagateMouseEvent(this, mouseEvent);
    processorNetworkEvaluator_->evaluate();
}

void CanvasQt::mouseReleaseEvent (QMouseEvent* e) {
    if (!processorNetworkEvaluator_) return;
    MouseEvent* mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), 
        EventConverterQt::getMouseButton(e),MouseEvent::MOUSE_STATE_RELEASE, 
        EventConverterQt::getModifier(e), dimensions_);
    processorNetworkEvaluator_->propagateMouseEvent(this, mouseEvent);
    processorNetworkEvaluator_->evaluate();
}

void CanvasQt::mouseMoveEvent(QMouseEvent*  e) {
    if (!processorNetworkEvaluator_) return;

    MouseEvent* mouseEvent = 0;
    if (e->buttons() == Qt::LeftButton || e->buttons() == Qt::RightButton || e->buttons() == Qt::MiddleButton) {
        mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), 
            EventConverterQt::getMouseButton(e), MouseEvent::MOUSE_STATE_PRESS, 
            EventConverterQt::getModifier(e), dimensions_);
    }
    else {
        mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), 
            MouseEvent::MOUSE_BUTTON_NONE, MouseEvent::MOUSE_STATE_NONE, 
            EventConverterQt::getModifier(e), dimensions_);
    }
    processorNetworkEvaluator_->propagateMouseEvent(this, mouseEvent);
    processorNetworkEvaluator_->evaluate();
}

} // namespace