/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
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
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

#include <modules/glfw/canvasprocessorwidgetglfw.h>
#include <modules/glfw/canvasglfw.h>
#include <inviwo/core/processors/canvasprocessor.h>

namespace inviwo {

CanvasProcessorWidgetGLFW::CanvasProcessorWidgetGLFW()
    : ProcessorWidget(),
      canvas_(0)
{
}

CanvasProcessorWidgetGLFW::~CanvasProcessorWidgetGLFW() {}

ProcessorWidget* CanvasProcessorWidgetGLFW::create() const {
    return new CanvasProcessorWidgetGLFW();
}

void CanvasProcessorWidgetGLFW::initialize() {
    canvasProcessor_ = dynamic_cast<CanvasProcessor*>(processor_);
    ProcessorWidget::initialize();
    ivec2 dim = getDimensionMetaData();
    uvec2 dimU = uvec2(dim.x, dim.y);
    canvas_ = new CanvasGLFW(processor_->getIdentifier(), dimU);
    canvas_->initializeGL();
    canvas_->initialize();
    canvasProcessor_->setCanvas(static_cast<Canvas*>(canvas_));
    canvas_->setWindowSize(dimU);
}

void CanvasProcessorWidgetGLFW::deinitialize() {
    if (canvas_) {
        this->hide();
        canvas_->deinitialize();
        canvas_ = NULL;
    }

    ProcessorWidget::deinitialize();
}

void CanvasProcessorWidgetGLFW::show() {
    canvas_->show();
    canvasProcessor_->triggerQueuedEvaluation();
    ProcessorWidget::show();
}

void CanvasProcessorWidgetGLFW::hide() {
    canvas_->hide();
    ProcessorWidget::hide();
}

void CanvasProcessorWidgetGLFW::setDimension(ivec2 dim) {
    ProcessorWidget::setDimension(dim);
    canvas_->setWindowSize(uvec2(dim.x, dim.y));
}

} // namespace
