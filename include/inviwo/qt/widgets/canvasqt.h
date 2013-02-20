#ifndef IVW_CANVASQT_H
#define IVW_CANVASQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <modules/opengl/canvasgl.h>

#include <QtOpenGL/QGLWidget>

namespace inviwo {

class IVW_QTWIDGETS_API CanvasQt : public QGLWidget, public CanvasGL {
public:

    CanvasQt(QWidget* parent);
    ~CanvasQt();

    void initialize();
    void initializeGL();
    void resizeGL(uint32_t w, uint32_t h);
    void paintGL();
    virtual void switchContext();   
    virtual void update();
    void repaint();

    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent (QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void timerEvent(QTimerEvent* e);
    MouseEvent::MouseButton getMouseButton(QMouseEvent* e);

private:
    static QGLWidget* sharedWidget_; //For rendering-context sharing
    static const std::string logSource_; ///< Source string to be displayed for log messages.
};

} // namespace

#endif // IVW_CANVASQT_H