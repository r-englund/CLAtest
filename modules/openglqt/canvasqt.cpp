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

#include <modules/openglqt/canvasqt.h>


namespace inviwo {

const QGLWidget* CanvasQt::sharedWidget_ = NULL;
QGLFormat CanvasQt::sharedFormat_ = QGLFormat(QGL::Rgba | QGL::DoubleBuffer | QGL::AlphaChannel | QGL::DepthBuffer | QGL::StencilBuffer);

CanvasQt::CanvasQt(QWidget* parent, uvec2 dim)
: QGLWidget(sharedFormat_, parent, sharedWidget_),
  CanvasGL(dim),
  swapBuffersAllowed_(false)
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

    setAutoBufferSwap(false);
	setFocusPolicy(Qt::TabFocus);
    setAttribute(Qt::WA_OpaquePaintEvent);
}

CanvasQt::~CanvasQt() {}

void CanvasQt::initialize() {
    activate();
    CanvasGL::initialize();
}

void CanvasQt::deinitialize() {
    CanvasGL::deinitialize();
}

void CanvasQt::initializeSquare(){
    CanvasGL::initializeSquare();
}

void CanvasQt::activate() {
    makeCurrent();
}

void CanvasQt::initializeGL() {
    initializeGLEW();
    QGLWidget::initializeGL();
}

void CanvasQt::resizeGL(int width, int height) {    
    CanvasGL::resize(uvec2(static_cast<uint32_t>(width), static_cast<uint32_t>(height)));    
}

void CanvasQt::glSwapBuffers(){
    if(swapBuffersAllowed_){
        activate();
        QGLWidget::swapBuffers();
    }
}

void CanvasQt::update() {
    CanvasGL::update();
}

void CanvasQt::repaint() {
    QGLWidget::updateGL();
}

void CanvasQt::paintGL() {
    swapBuffersAllowed_ = true;
    CanvasGL::update();
}

void CanvasQt::mousePressEvent(QMouseEvent* e) {
    if (!processorNetworkEvaluator_) return;
    MouseEvent* mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), 
        EventConverterQt::getMouseButton(e), MouseEvent::MOUSE_STATE_PRESS, 
        EventConverterQt::getModifier(e), dimensions_);
    e->accept();
    Canvas::mousePressEvent(mouseEvent);
    delete mouseEvent;
}

void CanvasQt::mouseReleaseEvent(QMouseEvent* e) {
    if (!processorNetworkEvaluator_) return;
    MouseEvent* mouseEvent = new MouseEvent(ivec2(e->pos().x(), e->pos().y()), 
        EventConverterQt::getMouseButton(e),MouseEvent::MOUSE_STATE_RELEASE, 
        EventConverterQt::getModifier(e), dimensions_);
    e->accept();
    Canvas::mouseReleaseEvent(mouseEvent);
    delete mouseEvent;
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
        delete mouseEvent;
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
    delete keyEvent;
}

void CanvasQt::keyReleaseEvent(QKeyEvent* e) {
	if (!processorNetworkEvaluator_) return;
	KeyboardEvent* keyEvent = new KeyboardEvent( 
		EventConverterQt::getKeyButton(e),  
		EventConverterQt::getModifier(e),
		KeyboardEvent::KEY_STATE_RELEASE);
    e->accept();
	Canvas::keyReleaseEvent(keyEvent);
    delete keyEvent;
}

} // namespace