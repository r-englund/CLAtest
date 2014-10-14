/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Contact: Erik Sundén
 *
 *********************************************************************************/

#include <modules/openglqt/canvasprocessorwidgetqt.h>
#include <modules/openglqt/canvasqt.h>
#include <inviwo/core/processors/canvasprocessor.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <QGridLayout>

#if defined(__APPLE__) && (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
//#define USE_QWINDOW
#endif

namespace inviwo {

CanvasProcessorWidgetQt::CanvasProcessorWidgetQt()
    : ProcessorWidgetQt(), canvas_(0), hasSharedCanvas_(false) {
    setMinimumSize(32, 32);
    setFocusPolicy(Qt::NoFocus);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setWindowTitle(QString::fromStdString("untitled canvas"));
}

CanvasProcessorWidgetQt::~CanvasProcessorWidgetQt() {
    if (hasSharedCanvas_ && canvas_) canvas_->setParent(NULL);

    if (processor_) {
        static_cast<CanvasProcessor*>(processor_)->setCanvas(NULL);
    }
}

ProcessorWidget* CanvasProcessorWidgetQt::create() const { return new CanvasProcessorWidgetQt(); }

void CanvasProcessorWidgetQt::initialize() {
    ProcessorWidgetQt::initialize();

    setWindowTitle(QString::fromStdString(processor_->getIdentifier()));
    CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor_);
    ivec2 dim = getDimension();
    CanvasQt* sharedCanvas = CanvasQt::getSharedCanvas();
    if (!sharedCanvas->getProcessorWidgetOwner()) {
        canvas_ = sharedCanvas;
        hasSharedCanvas_ = true;
    } else {
        canvas_ = new CanvasQt(NULL, uvec2(dim.x, dim.y));
    }

    if (!canvas_->isInitialized()) canvas_->initialize();

    canvas_->setProcessorWidgetOwner(this);
    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->setContentsMargins(0, 0, 0, 0);
#ifdef USE_QWINDOW
    QWidget* container = QWidget::createWindowContainer(canvas_);
#else
    canvas_->setMouseTracking(true);
    QWidget* container = static_cast<QWidget*>(canvas_);
#endif
    container->setAttribute(Qt::WA_OpaquePaintEvent);
    gridLayout->addWidget(container, 0, 0);
    setLayout(gridLayout);
    setWindowFlags(Qt::Window
#ifndef WIN32
                   | Qt::WindowStaysOnTopHint
#endif
                   );

    canvasProcessor->setCanvas(static_cast<Canvas*>(canvas_));
    canvas_->setNetworkEvaluator(InviwoApplication::getPtr()->getProcessorNetworkEvaluator());
    
    if (canvasProcessor->getUseCustomDimensions()) {
        canvas_->CanvasGL::resize(dim, canvasProcessor->getCustomDimensions());
    } else {
        canvas_->CanvasGL::resize(dim, dim);
    }
}

void CanvasProcessorWidgetQt::deinitialize() {
    if (canvas_) {
    
        canvas_->setNetworkEvaluator(NULL);
    
        CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor_);
        canvasProcessor->setCanvas(NULL);
    
        this->hide();
        if (hasSharedCanvas_) {
            canvas_->setProcessorWidgetOwner(NULL);
            layout()->removeWidget(canvas_);
            canvas_->setParent(NULL);
        } else {
            canvas_->deinitialize();
        }
        // FIXME: CanvasQt is child of this object.
        // Hence don't delete CanvasQt here or use deleteLater. Let the destructor destroy CanvasQt
        // widget
        // canvas_->deleteLater();
        // if (children().size())
        //    LogWarn("Canvas is not expected to have children");
        canvas_ = NULL;
    }

    ProcessorWidgetQt::deinitialize();
}

void CanvasProcessorWidgetQt::setVisible(bool visible) {
    if(visible){
        canvas_->show();
        static_cast<CanvasProcessor*>(processor_)->triggerQueuedEvaluation();
    }else{
        canvas_->hide();
    }
    ProcessorWidgetQt::setVisible(visible);
}
    
void CanvasProcessorWidgetQt::show() {
    CanvasProcessorWidgetQt::setVisible(true);
}
void CanvasProcessorWidgetQt::hide() {
    CanvasProcessorWidgetQt::setVisible(false);
}

void CanvasProcessorWidgetQt::showEvent(QShowEvent* event) {
    ProcessorWidgetQt::showEvent(event);
}

void CanvasProcessorWidgetQt::closeEvent(QCloseEvent* event) {
    canvas_->hide();
    ProcessorWidgetQt::closeEvent(event);
}

void CanvasProcessorWidgetQt::resizeEvent(QResizeEvent* event) {
    if(!event->spontaneous()) return;
    uvec2 dim(event->size().width(), event->size().height());
    CanvasProcessor* cp = static_cast<CanvasProcessor*>(processor_);
    ProcessorWidgetQt::resizeEvent(event);
    cp->updateCanvasSize(dim);  
}

}  // namespace
