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
 * Main file authors: Sathish Kottravel, Erik Sundén
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
#define GLEXT_64_TYPES_DEFINED
#include <QGLWidget>
#include <QInputEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QEvent>

namespace inviwo {

class IVW_MODULE_OPENGLQT_API CanvasQt : public QGLWidget, public CanvasGL {
    Q_OBJECT
public:
    CanvasQt(QWidget* parent = NULL, uvec2 dim = uvec2(256,256));
    ~CanvasQt();

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

    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void wheelEvent (QWheelEvent* e);

    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

private:
    static QGLWidget* sharedWidget_; //For rendering-context sharing
    static QGLFormat sharedFormat_;
    static bool sharedInitialized_;
    bool swapBuffersAllowed_;
};

} // namespace

#endif // IVW_CANVASQT_H