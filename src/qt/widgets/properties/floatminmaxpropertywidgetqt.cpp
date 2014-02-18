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

#include <inviwo/qt/widgets/properties/floatminmaxpropertywidgetqt.h>
#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

FloatMinMaxPropertyWidgetQt::FloatMinMaxPropertyWidgetQt(FloatMinMaxProperty* property) : property_(property),
    updatingFromProperty_(false) {
    PropertyWidgetQt::setProperty(property_);
    PropertyWidgetQt::generateContextMenu();
    generateWidget();
    updateFromProperty();
}

void FloatMinMaxPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();

    if (property_->getReadOnly()) {
        valueVec_ = property_->get();
        valueVec_ = property_->get();
        hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
        readOnlyLabel_ = new QLabel(QString::number(valueVec_.x)+","+QString::number(valueVec_.y));
        hLayout->addWidget(readOnlyLabel_);
        setLayout(hLayout);
    }
    else {
        label_ = new EditableLabelQt(this,property_->getDisplayName(),PropertyWidgetQt::generatePropertyWidgetMenu());
        hLayout->addWidget(label_);
        QHBoxLayout* hSliderLayout = new QHBoxLayout();
        QWidget* sliderWidget = new QWidget();
        sliderWidget->setLayout(hSliderLayout);
        hSliderLayout->setContentsMargins(0,0,0,0);
        spinBoxMin_ = new CustomDoubleSpinBoxQt(this);
        spinBoxMin_->setKeyboardTracking(false); // don't emit the valueChanged() signal while typing
        spinBoxMin_->setFixedWidth(50);
        hSliderLayout->addWidget(spinBoxMin_);
        slider_ = new RangeSliderQt(Qt::Horizontal, this);
        hSliderLayout->addWidget(slider_);
        spinBoxMax_ = new CustomDoubleSpinBoxQt(this);
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
        connect(spinBoxMin_, SIGNAL(valueChanged(double)), this, SLOT(updateFromSpinBoxMin(double)));
        connect(spinBoxMax_, SIGNAL(valueChanged(double)), this, SLOT(updateFromSpinBoxMax(double)));
    }
}

void FloatMinMaxPropertyWidgetQt::updateFromProperty() {
    updatingFromProperty_ = true;
    valueVec_ = property_->get();

    if (property_->getReadOnly()) {
        readOnlyLabel_->setText(QString::number(valueVec_.x)+","+QString::number(valueVec_.y));
        updatingFromProperty_ = false;
    }
    else {
        maxNumberOfValues_ = (property_->getRangeMax()-property_->getRangeMin())/property_->getIncrement();
        slider_->setRange(0, static_cast<int>(maxNumberOfValues_));
        spinBoxMin_->setRange(property_->getRangeMin(), property_->getRangeMax());
        spinBoxMax_->setRange(property_->getRangeMin(), property_->getRangeMax());
        spinBoxMin_->setSingleStep(property_->getIncrement());
        spinBoxMax_->setSingleStep(property_->getIncrement());
        setSpinBoxDecimals(property_->getIncrement());
        sliderMin_ = static_cast<int>((valueVec_.x-property_->getRangeMin())*maxNumberOfValues_);
        sliderMax_ = static_cast<int>((valueVec_.y-property_->getRangeMin())*maxNumberOfValues_);
        slider_->setValue(sliderMin_, sliderMax_);
        blockSignals(true);
        spinBoxMin_->setValue(valueVec_.x);
        spinBoxMax_->setValue(valueVec_.y);
        blockSignals(false);
        updatingFromProperty_ = false;
    }
}

void FloatMinMaxPropertyWidgetQt::updateFromSlider(int valMin, int valMax) {
    if (!updatingFromProperty_) {
        bool changed = false;
        blockSignals(true);
        double valMinDouble = static_cast<double>(valMin)/static_cast<double>(maxNumberOfValues_);
        double valMaxDouble = static_cast<double>(valMax)/static_cast<double>(maxNumberOfValues_);

        if (sliderMin_ != valMin) {
            sliderMin_ = valMin;
            spinBoxMin_->setValue(valMinDouble);
            changed = true;
        }

        if (sliderMax_ != valMax) {
            sliderMax_ = valMax;
            spinBoxMax_->setValue(valMaxDouble);
            changed = true;
        }

        blockSignals(false);

        if (changed)
            setPropertyValue(valMinDouble, valMaxDouble);
    }
}

void FloatMinMaxPropertyWidgetQt::updateFromSpinBoxMin(double val) {
    if (!updatingFromProperty_) {
        slider_->setMinValue(static_cast<int>((val-property_->getRangeMin())*maxNumberOfValues_));
        setPropertyValue(static_cast<float>(val), static_cast<float>(spinBoxMax_->value()));
    }
}

void FloatMinMaxPropertyWidgetQt::updateFromSpinBoxMax(double val) {
    if (!updatingFromProperty_) {
        slider_->setMaxValue(static_cast<int>((val-property_->getRangeMin())*maxNumberOfValues_));
        setPropertyValue(static_cast<float>(spinBoxMin_->value()), static_cast<float>(val));
    }
}

void FloatMinMaxPropertyWidgetQt::setPropertyValue(float minVal, float maxVal) {
    property_->set(vec2(minVal, maxVal));
    emit modified();
}

void FloatMinMaxPropertyWidgetQt::setSpinBoxDecimals(float increment) {
    std::ostringstream buff;
    buff<<increment;
    std::string str2 = buff.str().substr(buff.str().find(".")+1,buff.str().length());
    spinBoxMin_->setDecimals(static_cast<int>(str2.length()));
    spinBoxMax_->setDecimals(static_cast<int>(str2.length()));
}

void FloatMinMaxPropertyWidgetQt::setPropertyDisplayName() {
    property_->setDisplayName(label_->getText());
}

} //namespace