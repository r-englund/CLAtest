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

CanvasQt::~CanvasQt() {
    QGLContext::currentContext();
    delete eventConverter_;
}

void CanvasQt::initialize() {
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
    uvec2 previousDimensions = dimensions_;
    CanvasGL::resize(uvec2(static_cast<uint32_t>(width), static_cast<uint32_t>(height)));
    if(processorNetworkEvaluator_) {
        ResizeEvent* resizeEvent = new ResizeEvent(dimensions_);
        resizeEvent->setPreviousSize(previousDimensions);        
        std::vector<Canvas*> registeredCanvases = processorNetworkEvaluator_->getRegisteredCanvases();
        std::vector<uvec2> registeredCanvasSize;
        for (size_t i=0; i<registeredCanvases.size(); i++)
            registeredCanvasSize.push_back(registeredCanvases[i]->size());
        resizeEvent->setRegisteredCanvasSizes(registeredCanvasSize);

        processorNetworkEvaluator_->propagateResizeEvent(this, resizeEvent);
        processorNetworkEvaluator_->evaluate();
    }
}

void CanvasQt::update() {
    CanvasGL::update();
}

void CanvasQt::repaint() {
    //CanvasGL::repaint();
    QGLWidget::repaint();
}


void CanvasQt::mousePressEvent(QMouseEvent* e) {
    if(!processorNetworkEvaluator_) return;
    MouseEvent* mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), 
        eventConverter_->getMouseButton(e), MouseEvent::MOUSE_STATE_PRESS, 
        eventConverter_->getModifier(e), dimensions_);
    processorNetworkEvaluator_->propagateMouseEvent(this, mouseEvent);
    processorNetworkEvaluator_->evaluate();
}

void CanvasQt::mouseReleaseEvent (QMouseEvent* e) {
    if(!processorNetworkEvaluator_) return;
    MouseEvent* mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), 
        eventConverter_->getMouseButton(e),MouseEvent::MOUSE_STATE_RELEASE, 
        eventConverter_->getModifier(e), dimensions_);
    processorNetworkEvaluator_->propagateMouseEvent(this, mouseEvent);
    processorNetworkEvaluator_->evaluate();
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
    processorNetworkEvaluator_->evaluate();
}

} // namespace