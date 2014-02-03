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
 * Main file authors: Timo Ropinski, Alexander Johansson
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/floatsliderwidgetqt.h>

namespace inviwo {

FloatSliderWidgetQt::FloatSliderWidgetQt() {
    generateWidget();
}

void FloatSliderWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    slider_ = new CustomSliderWidgetQt();
    slider_->setOrientation(Qt::Horizontal);
    slider_->setMaximum(SLIDER_MAX);
    spinBox_ = new CustomDoubleSpinBoxQt();
    spinBox_->setKeyboardTracking(false); // don't emit the valueChanged() signal while typing
    //spinBox_->setFixedWidth(50);
    hLayout->addWidget(slider_);
    hLayout->addWidget(spinBox_);
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->setSpacing(0);
    setLayout(hLayout);
    connect(slider_,  SIGNAL(valueChanged(int)),    this, SLOT(updateFromSlider()));
    connect(spinBox_, SIGNAL(valueChanged(double)), this, SLOT(updateFromSpinBox()));
}

FloatSliderWidgetQt::~FloatSliderWidgetQt() {}


void FloatSliderWidgetQt::setValue(float value) {
    if (value>=minValue_ && value<=maxValue_ && value!=value_) {
        value_ = value;
        emit valueChanged(value_);
    }

    updateSlider();
    updateSpinBox();
}

float inviwo::FloatSliderWidgetQt::getValue() {
    return value_;
}

void FloatSliderWidgetQt::setMinValue(float minValue) {
    minValue_ = minValue;
    spinBox_->setMinimum(static_cast<double>(minValue));
}

void FloatSliderWidgetQt::setMaxValue(float maxValue) {
    maxValue_ = maxValue;
    spinBox_->setMaximum(static_cast<double>(maxValue));
}

void FloatSliderWidgetQt::setRange(float minValue, float maxValue) {
    setMinValue(minValue);
    setMaxValue(maxValue);
}

void FloatSliderWidgetQt::setIncrement(float increment) {
    increment_ = increment;
    spinBox_->setSingleStep(static_cast<double>(increment));
    slider_->setSingleStep(10);
    setSpinBoxDecimals(increment);
}


void FloatSliderWidgetQt::updateFromSlider() {
    float normalizedValue = static_cast<float>(slider_->value())/static_cast<float>(slider_->maximum());
    float newValue = minValue_ + (normalizedValue * (maxValue_ - minValue_));

    if (newValue != value_) {
        value_ = newValue;
        //updateSpinBox();
        emit valueChanged(value_);
    }
}

void FloatSliderWidgetQt::updateSlider() {
    float normalizedValue = (value_-minValue_)/(maxValue_-minValue_);
    int newValue = static_cast<int>(normalizedValue * SLIDER_MAX);
    slider_->blockSignals(true);
    slider_->setValue(newValue);
    slider_->blockSignals(false);
}


void FloatSliderWidgetQt::updateFromSpinBox() {
    float newValue = static_cast<float>(spinBox_->value());

    if (newValue != value_) {
        value_ = newValue;
        //updateSlider();
        emit valueChanged(value_);
    }
}

void FloatSliderWidgetQt::updateSpinBox() {
    spinBox_->blockSignals(true);
    spinBox_->setValue(static_cast<double>(value_));
    spinBox_->blockSignals(false);
}

void FloatSliderWidgetQt::setSpinBoxDecimals(float increment) {
    std::ostringstream buff;
    buff<<increment;
    std::string str2 = buff.str().substr(buff.str().find(".")+1,buff.str().length());
    spinBox_->setDecimals(static_cast<int>(str2.length()));
}

} // namespace inviwo
