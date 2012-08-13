#ifndef IVW_CANVASQT_H
#define IVW_CANVASQT_H

#include "../../../../modules/opengl/canvasgl.h"

#include <QtOpenGL/QGLWidget>

namespace inviwo {

    class CanvasQt : public CanvasGL, public QGLWidget {
    public:

        static EventHandler* eventHandler_;

        CanvasQt(QWidget* parent);
        ~CanvasQt();

        void initialize();
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();

        void repaint();

        void mousePressEvent(QMouseEvent* e);
        void mouseReleaseEvent (QMouseEvent* e);
        void mouseMoveEvent(QMouseEvent* e);
        void timerEvent(QTimerEvent* e);
    };

} // namespace

#endif // IVW_CANVASQT_H