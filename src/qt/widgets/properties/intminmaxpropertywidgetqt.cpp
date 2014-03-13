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
 * Main file authors: Erik Sundén, Alexander Johansson
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/properties/intminmaxpropertywidgetqt.h>
#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

IntMinMaxPropertyWidgetQt::IntMinMaxPropertyWidgetQt(IntMinMaxProperty* property) 
    : PropertyWidgetQt(property)
    , property_(property)
    , updatingFromProperty_(false) {

    generateWidget();
    updateFromProperty();
}

void IntMinMaxPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(7);

    label_ = new EditableLabelQt(this,property_->getDisplayName());
    hLayout->addWidget(label_);
    QHBoxLayout* hSliderLayout = new QHBoxLayout();
    QWidget* sliderWidget = new QWidget();
    sliderWidget->setLayout(hSliderLayout);
    hSliderLayout->setContentsMargins(0,0,0,0);
    spinBoxMin_ = new QSpinBox(this);
    spinBoxMin_->setKeyboardTracking(false); // don't emit the valueChanged() signal while typing
    spinBoxMin_->setFixedWidth(50);
    hSliderLayout->addWidget(spinBoxMin_);
    slider_ = new RangeSliderQt(Qt::Horizontal, this);
    slider_->setRange(0, 99);
    slider_->setMinSeperation(property_->getMinSeperation());
    hSliderLayout->addWidget(slider_);
    spinBoxMax_ = new QSpinBox(this);
    spinBoxMax_->setKeyboardTracking(false); // don't emit the valueChanged() signal while typing
    spinBoxMax_->setFixedWidth(50);
    hSliderLayout->addWidget(spinBoxMax_);
    hLayout->addWidget(sliderWidget);
    setLayout(hLayout);
    QSizePolicy labelPol = label_->sizePolicy();
    labelPol.setHorizontalStretch(1);
    labelPol.setControlType(QSizePolicy::Label);
    label_->setSizePolicy(labelPol);
    QSizePolicy slidersPol = sliderWidget->sizePolicy();
    slidersPol.setHorizontalStretch(3);
    sliderWidget->setSizePolicy(slidersPol);
    connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
    connect(slider_, SIGNAL(valuesChanged(int,int)), this, SLOT(updateFromSlider(int,int)));
    connect(spinBoxMin_, SIGNAL(valueChanged(int)), this, SLOT(updateFromSpinBoxMin(int)));
    connect(spinBoxMax_, SIGNAL(valueChanged(int)), this, SLOT(updateFromSpinBoxMax(int)));

    this->setEnabled(!property_->getReadOnly());

}

void IntMinMaxPropertyWidgetQt::updateFromProperty() {
    updatingFromProperty_ = true;
    valueVec_ = property_->get();

    slider_->setRange(property_->getRangeMin(), property_->getRangeMax());
    spinBoxMin_->setRange(property_->getRangeMin(), property_->getRangeMax());
    spinBoxMax_->setRange(property_->getRangeMin(), property_->getRangeMax());
    spinBoxMin_->setSingleStep(property_->getIncrement());
    spinBoxMax_->setSingleStep(property_->getIncrement());
    slider_->setValue(valueVec_.x, valueVec_.y);
    spinBoxMin_->setValue(valueVec_.x);
    spinBoxMax_->setValue(valueVec_.y);
    slider_->setMinSeperation(property_->getMinSeperation());
    updatingFromProperty_ = false;

}


void IntMinMaxPropertyWidgetQt::updateFromSlider(int valMin, int valMax) {
    if (!updatingFromProperty_) {
        bool changed = false;

        if (valMin != spinBoxMin_->value()) {
            spinBoxMin_->setValue(valMin);
            changed = true;
        }

        if (valMax != spinBoxMax_->value()) {
            spinBoxMax_->setValue(valMax);
            changed = true;
        }

        if (changed)
            setPropertyValue();
    }
}

void IntMinMaxPropertyWidgetQt::updateFromSpinBoxMin(int val) {
    if (!updatingFromProperty_) {
        slider_->setMinValue(val);
        setPropertyValue();
    }
}

void IntMinMaxPropertyWidgetQt::updateFromSpinBoxMax(int val) {
    if (!updatingFromProperty_) {
        slider_->setMaxValue(val);
        setPropertyValue();
    }
}

void IntMinMaxPropertyWidgetQt::setPropertyValue() {
    property_->set(ivec2(spinBoxMin_->value(), spinBoxMax_->value()));
    emit modified();
}

void IntMinMaxPropertyWidgetQt::setPropertyDisplayName() {
    property_->setDisplayName(label_->getText());
}

} //namespace