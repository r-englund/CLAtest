/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#ifndef IVW_CANVASQT_H
#define IVW_CANVASQT_H

#include <modules/openglqt/openglqtmoduledefine.h>
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

class IVW_MODULE_OPENGLQT_API CanvasQt : public QGLWidget, public CanvasGL {
    Q_OBJECT
public:
    CanvasQt(QWidget* parent = NULL);
    ~CanvasQt();

    virtual void initialize();
    virtual void deinitialize();
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
    static const QGLWidget* sharedWidget_; //For rendering-context sharing
    static QGLFormat sharedFormat_;
};

} // namespace

#endif // IVW_CANVASQT_H