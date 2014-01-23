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

#include <inviwo/qt/widgets/properties/floatminmaxpropertywidgetqt.h>
#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

FloatMinMaxPropertyWidgetQt::FloatMinMaxPropertyWidgetQt(FloatMinMaxProperty *property) : property_(property), updatingFromProperty_(false) {
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
    else{
 
    label_ = new EditableLabelQt(property_->getDisplayName(),PropertyWidgetQt::generatePropertyWidgetMenu());
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
    else{
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

void FloatMinMaxPropertyWidgetQt::updateFromSlider(int valMin, int valMax){
    if(!updatingFromProperty_){
        bool changed = false;
        blockSignals(true);
        double valMinDouble = static_cast<double>(valMin)/static_cast<double>(maxNumberOfValues_);
        double valMaxDouble = static_cast<double>(valMax)/static_cast<double>(maxNumberOfValues_);
        if(sliderMin_ != valMin){
            sliderMin_ = valMin;
            spinBoxMin_->setValue(valMinDouble);
            changed = true;
        }
        if(sliderMax_ != valMax){
            sliderMax_ = valMax;
            spinBoxMax_->setValue(valMaxDouble);
            changed = true;
        }
        blockSignals(false);
        if(changed)
            setPropertyValue(valMinDouble, valMaxDouble);
    }
}

void FloatMinMaxPropertyWidgetQt::updateFromSpinBoxMin(double val){
    if(!updatingFromProperty_){
        slider_->setMinValue(static_cast<int>((val-property_->getRangeMin())*maxNumberOfValues_));
        setPropertyValue(static_cast<float>(val), static_cast<float>(spinBoxMax_->value()));
    }
}

void FloatMinMaxPropertyWidgetQt::updateFromSpinBoxMax(double val){
    if(!updatingFromProperty_){
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

void FloatMinMaxPropertyWidgetQt::setPropertyDisplayName(){
    property_->setDisplayName(label_->getText());
}

} //namespace