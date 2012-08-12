#include "inviwo/qt/widgets/canvasqt.h"
#include "inviwo/core/network/processornetworkevaluator.h"

namespace inviwo {

    CanvasQt::CanvasQt(QWidget* parent)
        : QGLWidget(parent),
        CanvasGL(ivec2(512,512))
    {}

    CanvasQt::~CanvasQt() {}

    void CanvasQt::initialize() {
        show();
        CanvasGL::initialize();
    }

    void CanvasQt::initializeGL() {
        glewInit();
        QGLWidget::initializeGL();
    }

    void CanvasQt::resizeGL(int w, int h) {
        //sizeChanged(ivec2(w, h));
    }

    void CanvasQt::paintGL() {
        QGLWidget::paintGL();
    }

    void CanvasQt::repaint() {
        updateGL();
    }

    void CanvasQt::update() {
        QWidget::update();
    }

    void CanvasQt::swap() {
        QGLWidget::swapBuffers();
    }

    void CanvasQt::getGLFocus() {
        QGLWidget::doneCurrent();
        QGLWidget::makeCurrent();
    }

} // namespace