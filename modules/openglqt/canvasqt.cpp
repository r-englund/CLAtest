#include <modules/openglqt/canvasqt.h>


namespace inviwo {

QGLWidget* CanvasQt::sharedWidget_ = 0;
QGLFormat CanvasQt::sharedFormat_ = QGLFormat(QGL::Rgba | QGL::SingleBuffer | QGL::AlphaChannel | QGL::DepthBuffer | QGL::StencilBuffer);

CanvasQt::CanvasQt(QWidget* parent)
: QGLWidget(sharedFormat_, parent, sharedWidget_),
  CanvasGL(uvec2(256,256))
{
    //This is our default rendering context
    //Initialized once. So "THE" first object of this class will not have any shared context (or widget)
    //But Following objects, will share the context of initial object
    if (!sharedWidget_){
        QGLFormat f = this->format();
        f.setVersion(10, 0);  //Will choose highest available version
        f.setProfile(QGLFormat::CompatibilityProfile);
        setFormat(f);
        sharedFormat_ = f;
        sharedWidget_ = this;
        QGLWidget::glInit();
    }

    //setAutoBufferSwap(false);
	setFocusPolicy(Qt::StrongFocus);
}

CanvasQt::~CanvasQt() {
    CanvasGL::deinitialize();
    //QGLContext::currentContext();
}

void CanvasQt::initialize() {
    activate();
    CanvasGL::initialize();
}

void CanvasQt::initializeSquare(){
    CanvasGL::initializeSquare();
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
    e->accept();
    Canvas::mousePressEvent(mouseEvent);
}

void CanvasQt::mouseReleaseEvent(QMouseEvent* e) {
    if (!processorNetworkEvaluator_) return;
    MouseEvent* mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), 
        EventConverterQt::getMouseButton(e),MouseEvent::MOUSE_STATE_RELEASE, 
        EventConverterQt::getModifier(e), dimensions_);
    e->accept();
    processorNetworkEvaluator_->propagateInteractionEvent(this, mouseEvent);
    processorNetworkEvaluator_->evaluate();
    Canvas::mouseReleaseEvent(mouseEvent);
}

void CanvasQt::mouseMoveEvent(QMouseEvent* e) {
    if (!processorNetworkEvaluator_) return;

    MouseEvent* mouseEvent = NULL;
    if (e->buttons() == Qt::LeftButton || e->buttons() == Qt::RightButton || e->buttons() == Qt::MiddleButton) {
        mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), 
            EventConverterQt::getMouseButton(e), MouseEvent::MOUSE_STATE_PRESS, 
            EventConverterQt::getModifier(e), dimensions_);
        e->accept();
        Canvas::mouseMoveEvent(mouseEvent);
    }
}

void CanvasQt::keyPressEvent(QKeyEvent* e) {
	if (!processorNetworkEvaluator_) return;
	KeyboardEvent* keyEvent = new KeyboardEvent( 
		EventConverterQt::getKeyButton(e),  
		EventConverterQt::getModifier(e),
		KeyboardEvent::KEY_STATE_PRESS);
    e->accept();
	Canvas::keyPressEvent(keyEvent);
}

void CanvasQt::keyReleaseEvent(QKeyEvent* e) {
	if (!processorNetworkEvaluator_) return;
	KeyboardEvent* keyEvent = new KeyboardEvent( 
		EventConverterQt::getKeyButton(e),  
		EventConverterQt::getModifier(e),
		KeyboardEvent::KEY_STATE_RELEASE);
    e->accept();
	Canvas::keyReleaseEvent(keyEvent);
}

} // namespace