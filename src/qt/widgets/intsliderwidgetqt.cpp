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
 * Main file author: Alexander Johansson
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/intsliderwidgetqt.h>

namespace inviwo {
inviwo::IntSliderWidgetQt::IntSliderWidgetQt(int minValue_, int maxValue_, int increment_) {
    this->minValue_ = minValue_;
    this->maxValue_ = maxValue_;
    this->increment_= increment_;
    fromSlider_ = false;
    generateWidget();
}

void inviwo::IntSliderWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    slider_ = new CustomSliderWidgetQt();
    slider_->setOrientation(Qt::Horizontal);
    slider_->setMaximum(SLIDER_MAX);
    spinBox_ = new QSpinBox();
    spinBox_->setKeyboardTracking(false);  // don't emit the valueChanged() signal while typing
    spinBox_->setRange(this->minValue_,this->maxValue_);
    spinBox_->setSingleStep(this->increment_);
    hLayout->addWidget(slider_);
    hLayout->addWidget(spinBox_);
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->setSpacing(0);
    setLayout(hLayout);
    connect(slider_,  SIGNAL(valueChanged(int)),this, SLOT(updateFromSlider()));
    connect(spinBox_, SIGNAL(valueChanged(int)), this, SLOT(updateFromSpinBox()));
}

inviwo::IntSliderWidgetQt::~IntSliderWidgetQt() {}

int inviwo::IntSliderWidgetQt::getValue() {
    return value_;
}

void inviwo::IntSliderWidgetQt::setValue(int tmpValue) {
    if (minValue_<tmpValue || tmpValue<=maxValue_) {
        value_ = tmpValue;

        if (!fromSlider_)
            updateValueSlider();
        else
            fromSlider_=false;

        spinBox_->setValue(tmpValue);
    }
}

void inviwo::IntSliderWidgetQt::setMaxValue(int max) {
    maxValue_ = max;
    spinBox_->setMaximum(maxValue_);
}

void inviwo::IntSliderWidgetQt::setMinValue(int min) {
    minValue_ = min;
    spinBox_->setMinimum(minValue_);
}

void inviwo::IntSliderWidgetQt::setRange(int min,int max) {
    setMinValue(min);
    setMaxValue(max);
}

QSpinBox* inviwo::IntSliderWidgetQt::getSpinBox() {
    return spinBox_;
}

QSlider* inviwo::IntSliderWidgetQt::getSlider() {
    return slider_;
}

void inviwo::IntSliderWidgetQt::updateValueSpinBox() {
    if (spinBox_->value()!=value_) {
        spinBox_->blockSignals(true);
        spinBox_->setValue(value_);
        spinBox_->blockSignals(false);
    }
}

void inviwo::IntSliderWidgetQt::updateValueSlider() {
    float normalizedValue = float(float(value_-minValue_)/float(maxValue_-minValue_));
    int newValue = static_cast<int>(floor(normalizedValue * SLIDER_MAX));
    slider_->blockSignals(true);
    slider_->setValue(newValue);
    slider_->blockSignals(false);
}

void IntSliderWidgetQt::setIncrement(int increment) {
    increment_ = increment;
    spinBox_->setSingleStep(increment_);
}

void IntSliderWidgetQt::updateFromSpinBox() {
    int spinboxValue = spinBox_->value();

    if (spinboxValue != value_) {
        value_ = spinboxValue;
        updateValueSlider();
        emit valueChanged(value_);
    }
}

void IntSliderWidgetQt::updateFromSlider() {
    float normalizedValue = static_cast<float>(slider_->value())/static_cast<float>(slider_->maximum());
    int newValue = static_cast<int>(floor(minValue_ + (normalizedValue * (maxValue_ - minValue_))));

    if (newValue != value_) {
        fromSlider_ = true;
        value_ = newValue;
        spinBox_->setValue(value_);
        emit valueChanged(value_);
    }
}

}//namespace