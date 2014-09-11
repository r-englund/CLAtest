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
 * Main file authors: Sathish Kottravel, Timo Ropinski
 *
 *********************************************************************************/

#include <inviwo/core/processors/processorwidget.h>
#include <inviwo/core/processors/processor.h>

namespace inviwo {

ProcessorWidget::ProcessorWidget()
    : ProcessorWidgetObservable()
    , metaData_(NULL)
    , processor_(NULL) {}

ProcessorWidget::ProcessorWidget(const ProcessorWidget& rhs) : metaData_(rhs.metaData_), processor_(rhs.processor_) {

}
ProcessorWidget& ProcessorWidget::operator=(const ProcessorWidget& that) {
    if (this != &that) {
        metaData_ = that.metaData_;
        processor_ = that.processor_;
    }
    return *this;
}

ProcessorWidget::~ProcessorWidget() {
    if(processor_) {
        processor_->setProcessorWidget(NULL);
    }
}


void ProcessorWidget::initialize() {
    metaData_ = dynamic_cast<ProcessorWidgetMetaData*>(processor_->getMetaData("ProcessorWidgetMetaData"));
}

void ProcessorWidget::deinitialize() {
    //FIXME: should metaData_ be set to NULL???
}

void ProcessorWidget::setVisible(bool visible) {
    if(visible){
        notifyObserversAboutShow(this);
    }else{
        notifyObserversAboutHide(this);
    }
    metaData_->setVisibile(visible);
}

bool ProcessorWidget::isVisible(){
    return metaData_->isVisible();
}

void ProcessorWidget::show() {
    metaData_->setVisibile(true);
    notifyObserversAboutShow(this);
    if(processor_){
        processor_->invalidate(PropertyOwner::INVALID_OUTPUT);   
    }
}

void ProcessorWidget::hide() {
    metaData_->setVisibile(false);
    notifyObserversAboutShow(this);
}

void ProcessorWidget::setDimension(ivec2 dimension) {
    metaData_->setDimension(dimension);
}

void ProcessorWidget::move(ivec2 pos) {
    metaData_->setWidgetPosition(pos);
}

bool ProcessorWidget::getVisibilityMetaData() {
    return metaData_->isVisible();
}

ivec2 ProcessorWidget::getPositionMetaData() {
    return metaData_->getWidgetPosition();
}

ivec2 ProcessorWidget::getDimensionMetaData() {
    return metaData_->getDimension();
}



} // namespace
