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
 * Main file author: Sathish Kottravel
 *
 *********************************************************************************/

#include <QGridLayout>
#include <modules/openglqt/processors/canvasprocessorwidgetqt.h>

namespace inviwo {

CanvasProcessorWidgetQt::CanvasProcessorWidgetQt()
    : ProcessorWidgetQt(),
      canvas_(0)
{
    setMinimumSize(32, 32);
    setFocusPolicy(Qt::NoFocus);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setWindowTitle(QString::fromStdString("untitled canvas"));
}

CanvasProcessorWidgetQt::~CanvasProcessorWidgetQt() {}

ProcessorWidget* CanvasProcessorWidgetQt::create() const {
    return new CanvasProcessorWidgetQt();
}

void CanvasProcessorWidgetQt::initialize() {
    setWindowTitle(QString::fromStdString(processor_->getIdentifier()));
    canvasProcessor_ = dynamic_cast<CanvasProcessor*>(processor_);
    ProcessorWidgetQt::initialize();
    ivec2 dim = getDimensionMetaData();
    //FIXME: Consider creating widget outside this class. Weird qt problem.
    //Because NULL does not make any difference here. CanvasQt has this object as parent.
    canvas_ = new CanvasQt(NULL, uvec2(dim.x, dim.y));
    canvas_->initialize();
    canvas_->setMouseTracking(true);
    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(static_cast<QWidget*>(canvas_), 0, 0);
    setLayout(gridLayout);
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    canvasProcessor_->setCanvas(static_cast<Canvas*>(canvas_));
    QWidget::resize(dim.x, dim.y);
}

void CanvasProcessorWidgetQt::deinitialize() {
    if (canvas_) {
        this->hide();
        canvas_->deinitialize();
        //FIXME: CanvasQt is child of this object.
        //Hence don't delete CanvasQt here or use deleteLater. Let the destructor destroy CanvasQt widget
        //canvas_->deleteLater();
        //if (children().size())
        //    LogWarn("Canvas is not expected to have children");
        canvas_ = NULL;
    }

    ProcessorWidgetQt::deinitialize();
}

void CanvasProcessorWidgetQt::resizeEvent(QResizeEvent* event) {
    ProcessorWidgetQt::resizeEvent(event);
    canvasProcessor_->setCanvasSize(ivec2(event->size().width(), event->size().height()));
}

void CanvasProcessorWidgetQt::show() {
    canvas_->show();
    canvasProcessor_->triggerQueuedEvaluation();
    ProcessorWidgetQt::show();
}

void CanvasProcessorWidgetQt::showEvent(QShowEvent* event) {
    ProcessorWidgetQt::showEvent(event);
}

void CanvasProcessorWidgetQt::hide() {
    canvas_->hide();
    ProcessorWidgetQt::hide();
}

void CanvasProcessorWidgetQt::closeEvent(QCloseEvent* event) {
    canvas_->hide();
    ProcessorWidgetQt::closeEvent(event);
}

} // namespace
