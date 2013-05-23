#ifndef IVW_CANVASQT_H
#define IVW_CANVASQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <modules/opengl/canvasgl.h>
#include <inviwo/qt/widgets/eventconverterqt.h>
#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/common/inviwo.h>

#include <QtOpenGL/QGLWidget>
#include <QInputEvent>
#include <QMouseEvent>

namespace inviwo {

class IVW_QTWIDGETS_API CanvasQt : public QGLWidget, public CanvasGL {
public:

    CanvasQt(QWidget* parent);
    ~CanvasQt();

    void initialize();
    void initializeGL();
    void resizeGL(int w, int h);
    virtual void activate();   
    virtual void update();
    void repaint();

    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent (QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);

private:
    EventConverterQt* eventConverter_;
    static QGLWidget* sharedWidget_; //For rendering-context sharing
};

} // namespace

#endif // IVW_CANVASQT_H