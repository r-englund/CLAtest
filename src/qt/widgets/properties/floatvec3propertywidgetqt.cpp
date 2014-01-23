/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Alexander Johansson
 *
 **********************************************************************/

#include <inviwo/qt/widgets/properties/floatvec3propertywidgetqt.h>

#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

FloatVec3PropertyWidgetQt::FloatVec3PropertyWidgetQt(FloatVec3Property* property)
    : property_(property)
    , valueVec3Max_(property->getMaxValue())
    , valueVec3Min_(property->getMinValue())
    , valueIncrement_(property->getIncrement()){
        
    PropertyWidgetQt::setProperty(property_);
    PropertyWidgetQt::generateContextMenu();
    generateWidget();
    updateFromProperty();
    generatesSettingsWidget();
}

void FloatVec3PropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    if (property_->getReadOnly()) {
        hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
        readOnlyLabel_ = new QLabel();
        hLayout->addWidget(readOnlyLabel_);
        setLayout(hLayout);
    }
    else {
        label_ = new EditableLabelQt(property_->getDisplayName(),PropertyWidgetQt::generatePropertyWidgetMenu());
        hLayout->addWidget(label_);

        QWidget* sliderWidget = new QWidget();
        QVBoxLayout* vLayout = new QVBoxLayout();
        sliderWidget->setLayout(vLayout);
        vLayout->setContentsMargins(0,0,0,0);
        vLayout->setSpacing(0);

        sliderX_ = new FloatSliderWidgetQt();
        sliderY_ = new FloatSliderWidgetQt();
        sliderZ_ = new FloatSliderWidgetQt();

        vLayout->addWidget(sliderX_);
        vLayout->addWidget(sliderY_);
        vLayout->addWidget(sliderZ_);
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
        connect(sliderX_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));
        connect(sliderY_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));
        connect(sliderZ_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));

    }  
}

void FloatVec3PropertyWidgetQt::updateFromProperty() {
    if (property_->getReadOnly()) {
        readOnlyLabel_->setText(QString::number(property_->get().x)+","
                                +QString::number(property_->get().y)+","
                                +QString::number(property_->get().z));


        readOnlyLabel_->setToolTip("Min: [" +QString::number((property_->getMinValue()).x)+
            ","+QString::number((property_->getMinValue()).y)+
            ","+QString::number((property_->getMinValue()).z)+
            "] Max: [" +QString::number((property_->getMaxValue()).x)+
            ","+QString::number((property_->getMaxValue()).y)+
            ","+QString::number((property_->getMinValue()).z)+"]");


    }
    else {
        valueVec3Max_ = property_->getMaxValue();
        valueVec3Min_ = property_->getMinValue();
        valueIncrement_ = property_->getIncrement();


        sliderX_->initValue(property_->get().x);
        sliderY_->initValue(property_->get().y);
        sliderZ_->initValue(property_->get().z);

        sliderX_->setRange(valueVec3Min_.x,valueVec3Max_.x);
        sliderY_->setRange(valueVec3Min_.y,valueVec3Max_.y);
        sliderZ_->setRange(valueVec3Min_.z,valueVec3Max_.z);

        sliderX_->setValue(property_->get().x);
        sliderY_->setValue(property_->get().y);
        sliderZ_->setValue(property_->get().z);

        sliderX_->setIncrement(valueIncrement_.x);
        sliderY_->setIncrement(valueIncrement_.y);
        sliderZ_->setIncrement(valueIncrement_.z);
    }
}


void FloatVec3PropertyWidgetQt::generatesSettingsWidget() {
    settingsWidget_ = new PropertySettingsWidgetQt(property_);

    settingsMenu_ = PropertyWidgetQt::generatePropertyWidgetMenu();
    settingsMenu_->addAction("Property settings");
    settingsMenu_->addAction("Set as Min");
    settingsMenu_->addAction("Set as Max");

    sliderX_->setContextMenuPolicy(Qt::CustomContextMenu);
    sliderY_->setContextMenuPolicy(Qt::CustomContextMenu);
    sliderZ_->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(sliderX_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenuX(const QPoint&)));
    connect(sliderY_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenuY(const QPoint&)));
    connect(sliderZ_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenuZ(const QPoint&)));
    connect(developerViewModeAction_,SIGNAL(triggered(bool)),this, SLOT(setDeveloperViewMode(bool)));
    connect(applicationViewModeAction_,SIGNAL(triggered(bool)),this, SLOT(setApplicationViewMode(bool)));

    updateContextMenu();
}

void FloatVec3PropertyWidgetQt::showContextMenuX( const QPoint& pos ) {
    PropertyVisibilityMode appVisibilityMode  = getApplicationViewMode();
    if (appVisibilityMode == DEVELOPMENT) {
        updateContextMenu();
        QPoint globalPos = sliderX_->mapToGlobal(pos);
        QAction* selecteditem = settingsMenu_->exec(globalPos);
        if (selecteditem && selecteditem->text() == "Property settings") {
            settingsWidget_->reload();
            settingsWidget_->show();
        }
        else if (selecteditem && selecteditem->text() == "Set as Min") {
            //Set current value of the slider to min value of the property
            valueVec3Min_ = property_->getMinValue();
            valueVec3Min_.x = sliderX_->getValue();
            property_->setMinValue(valueVec3Min_);
            updateFromProperty();
        }
        else if (selecteditem && selecteditem->text() == "Set as Max"){
            //Set current value of the slider to max value of the property
            valueVec3Max_ = property_->getMaxValue();
            valueVec3Max_.x = sliderX_->getValue();
            property_->setMaxValue(valueVec3Max_);
            updateFromProperty();
        }
    }
}

void FloatVec3PropertyWidgetQt::showContextMenuY( const QPoint& pos ) {
    PropertyVisibilityMode appVisibilityMode  = getApplicationViewMode();
    if (appVisibilityMode == DEVELOPMENT) {
        updateContextMenu();
        QPoint globalPos = sliderY_->mapToGlobal(pos);
        QAction* selecteditem = settingsMenu_->exec(globalPos);
        if (selecteditem && selecteditem->text() == "Property settings") {
            settingsWidget_->reload();
            settingsWidget_->show();
        }
        else if (selecteditem && selecteditem->text() == "Set as Min") {
            //Set current value of the slider to min value of the property
            valueVec3Min_ = property_->getMinValue();
            valueVec3Min_.y = sliderY_->getValue();
            property_->setMinValue(valueVec3Min_);
            updateFromProperty();
        }
        else if (selecteditem && selecteditem->text() == "Set as Max"){
            //Set current value of the slider to max value of the property
            valueVec3Max_ = property_->getMaxValue();
            valueVec3Max_.y = sliderY_->getValue();
            property_->setMaxValue(valueVec3Max_);
            updateFromProperty();
        }
    }
}

void FloatVec3PropertyWidgetQt::showContextMenuZ( const QPoint& pos ) {
    PropertyVisibilityMode appVisibilityMode  = getApplicationViewMode();
    if (appVisibilityMode == DEVELOPMENT) {
        updateContextMenu();
        QPoint globalPos = sliderZ_->mapToGlobal(pos);

        QAction* selecteditem = settingsMenu_->exec(globalPos);
        if (selecteditem && selecteditem->text() == "Property settings") {
            settingsWidget_->reload();
            settingsWidget_->show();
        }
        else if (selecteditem && selecteditem->text() == "Set as Min") {
            //Set current value of the slider to min value of the property
            valueVec3Min_ = property_->getMinValue();
            valueVec3Min_.z = sliderZ_->getValue();
            property_->setMinValue(valueVec3Min_);
            updateFromProperty();
        }
        else if (selecteditem && selecteditem->text() == "Set as Max"){
            //Set current value of the slider to max value of the property
            valueVec3Max_ = property_->getMaxValue();
            valueVec3Max_.z = sliderZ_->getValue();
            property_->setMaxValue(valueVec3Max_);
            updateFromProperty();
        }
    }
}

void FloatVec3PropertyWidgetQt::setPropertyValue() {
    property_->set(vec3(sliderX_->getValue(), sliderY_->getValue(), sliderZ_->getValue()));
    emit modified();
}

void FloatVec3PropertyWidgetQt::setPropertyDisplayName(){
    property_->setDisplayName(label_->getText());
}

} // namespace