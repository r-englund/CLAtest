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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include <inviwo/qt/widgets/properties/intminmaxpropertywidgetqt.h>
#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

IntMinMaxPropertyWidgetQt::IntMinMaxPropertyWidgetQt(IntMinMaxProperty* property) : property_(property), updatingFromProperty_(false) {
    PropertyWidgetQt::setProperty(property_);
    PropertyWidgetQt::generateContextMenu();
    generateWidget();
    updateFromProperty();
}

void IntMinMaxPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();

    if (property_->getReadOnly()) {
        valueVec_ = property_->get();
        valueVec_ = property_->get();
        hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
        readOnlyLabel_ = new QLabel();
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
        spinBoxMin_ = new QSpinBox(this);
        spinBoxMin_->setKeyboardTracking(false); // don't emit the valueChanged() signal while typing
        spinBoxMin_->setFixedWidth(50);
        hSliderLayout->addWidget(spinBoxMin_);
        slider_ = new RangeSliderQt(Qt::Horizontal, this);
        slider_->setRange(0, 99);
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
    }
}

void IntMinMaxPropertyWidgetQt::updateFromProperty() {
    updatingFromProperty_ = true;
    valueVec_ = property_->get();

    if (property_->getReadOnly()) {
        readOnlyLabel_->setText(QString::number(valueVec_.x)+","+QString::number(valueVec_.y));
        updatingFromProperty_ = false;
    }
    else {
        slider_->setRange(property_->getRangeMin(), property_->getRangeMax());
        spinBoxMin_->setRange(property_->getRangeMin(), property_->getRangeMax());
        spinBoxMax_->setRange(property_->getRangeMin(), property_->getRangeMax());
        spinBoxMin_->setSingleStep(property_->getIncrement());
        spinBoxMax_->setSingleStep(property_->getIncrement());
        slider_->setValue(valueVec_.x, valueVec_.y);
        spinBoxMin_->setValue(valueVec_.x);
        spinBoxMax_->setValue(valueVec_.y);
        updatingFromProperty_ = false;
    }
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