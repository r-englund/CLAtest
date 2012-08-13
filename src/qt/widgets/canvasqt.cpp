#include <QMouseEvent>

#include "inviwo/qt/widgets/canvasqt.h"
#include "inviwo/core/network/processornetworkevaluator.h"

namespace inviwo {

    CanvasQt::CanvasQt(QWidget* parent)
        : QGLWidget(QGLFormat(QGL::SingleBuffer | QGL::DepthBuffer), parent),
        CanvasGL(ivec2(256,256))
    {}

    CanvasQt::~CanvasQt() {}

    void CanvasQt::initialize() {
        CanvasGL::initialize();
        QObject::startTimer(1);
        show();
    }

    void CanvasQt::initializeGL() {
        glewInit();
        QGLWidget::initializeGL();
    }

    void CanvasQt::resizeGL(int width, int height) {
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (height == 0) gluPerspective(60.0, static_cast<double>(width), 0.1, 10.0);
        else gluPerspective (60.0, static_cast<double>(width) / static_cast<double>(height), 0.1, 10.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        //sizeChanged(ivec2(w, h));
    }

    void CanvasQt::paintGL() {
        QGLWidget::paintGL();
    }

    void CanvasQt::repaint() {
        CanvasGL::repaint();
        QGLWidget::repaint();
    }


    void CanvasQt::timerEvent(QTimerEvent* e) {
        processorNetworkEvaluator_->evaluate();
        if (processorNetworkEvaluator_->repaintRequired())
            repaint();
    }


    void CanvasQt::mousePressEvent(QMouseEvent* e) {
        MouseEvent* mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), MouseEvent::MOUSE_BUTTON_LEFT,
            MouseEvent::MOUSE_STATE_PRESS, MouseEvent::MODIFIER_NONE, dimensions_);
        processorNetworkEvaluator_->propagateMouseEvent(this, mouseEvent);
    }

    void CanvasQt::mouseReleaseEvent (QMouseEvent* e) {
        MouseEvent* mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), MouseEvent::MOUSE_BUTTON_LEFT,
            MouseEvent::MOUSE_STATE_RELEASE, MouseEvent::MODIFIER_NONE, dimensions_);
        processorNetworkEvaluator_->propagateMouseEvent(this, mouseEvent);
    }

    void CanvasQt::mouseMoveEvent(QMouseEvent*  e) {
        MouseEvent* mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), MouseEvent::MOUSE_BUTTON_LEFT,
            MouseEvent::MOUSE_STATE_PRESS, MouseEvent::MODIFIER_NONE, dimensions_);
        processorNetworkEvaluator_->propagateMouseEvent(this, mouseEvent);
    }


} // namespace