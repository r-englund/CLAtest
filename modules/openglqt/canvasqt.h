#ifndef IVW_CANVASQT_H
#define IVW_CANVASQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <modules/opengl/canvasgl.h>
#include <inviwo/qt/widgets/eventconverterqt.h>
#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/common/inviwo.h>

#define QT_NO_OPENGL_ES_2
#include <QGLWidget>
#include <QInputEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QEvent>

namespace inviwo {

class IVW_QTWIDGETS_API CanvasQt : public QGLWidget, public CanvasGL {
public:
    CanvasQt(QWidget* parent = NULL);
    ~CanvasQt();

    void initialize();
    void initializeSquare();
    virtual void activate();
    virtual void glSwapBuffers();
    virtual void update();
    void repaint();

protected:
	void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);

    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

private:
    static QGLWidget* sharedWidget_; //For rendering-context sharing
    static QGLFormat sharedFormat_;
};

} // namespace

#endif // IVW_CANVASQT_H