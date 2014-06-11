/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file authors: Erik Sundén, Sathish Kottravel
 *
 *********************************************************************************/

#ifndef IVW_CANVASQT_H
#define IVW_CANVASQT_H

#include <modules/openglqt/openglqtmoduledefine.h>
#include <modules/opengl/canvasgl.h>
#include <inviwo/qt/widgets/eventconverterqt.h>
#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/common/inviwo.h>

#define QT_NO_OPENGL_ES_2
/*#ifdef __APPLE__
#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#endif*/
#define GLEXT_64_TYPES_DEFINED
#include <QInputEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QEvent>

#ifndef QT_NO_GESTURES
#include <QGestureEvent>
#include <QPanGesture>
#include <QPinchGesture>
#endif

#if defined(__APPLE__) && (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
#define USE_QWINDOW
#endif

#ifdef USE_QWINDOW
#include <QtGui/QWindow>
#include <QtGui/QSurfaceFormat>
class QOpenGLContext;
#define QGLWindow QWindow
#define QGLParent QWindow
#define QGLContextFormat QSurfaceFormat
#else
#include <QGLWidget>
#define QGLWindow QGLWidget
#define QGLParent QWidget
#define QGLContextFormat QGLFormat
#endif

namespace inviwo {

class IVW_MODULE_OPENGLQT_API CanvasQt : public QGLWindow, public CanvasGL {
    Q_OBJECT
public:
    explicit CanvasQt(QGLParent* parent = NULL, uvec2 dim = uvec2(256,256));
    ~CanvasQt();

    static void defineDefaultContextFormat();

    void initialize();
    void initializeSquare();
    void deinitialize();
    void activate();
    void glSwapBuffers();
    void update();
    void repaint();

protected:
    void initializeGL();
    void paintGL();

    bool event(QEvent *e);
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void wheelEvent (QWheelEvent* e);
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

#ifdef USE_QWINDOW
    void exposeEvent(QExposeEvent *event);

private:
    QOpenGLContext* thisGLContext_;

    static QOpenGLContext* sharedGLContext_; //For rendering-context sharing
#else
private:
    static QGLWidget* sharedGLContext_; //For rendering-context sharing
#endif
    static QGLContextFormat sharedFormat_;
    bool swapBuffersAllowed_;

#ifndef QT_NO_GESTURES
    bool gestureEvent(QGestureEvent *e);
    void panTriggered(QPanGesture*);
    void pinchTriggered(QPinchGesture*);
#endif
};

} // namespace

#endif // IVW_CANVASQT_H