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

#include <inviwo/qt/widgets/properties/intvec3propertywidgetqt.h>
#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

IntVec3PropertyWidgetQt::IntVec3PropertyWidgetQt(IntVec3Property* property) : property_(property) , settingsWidget_(0) {
    PropertyWidgetQt::setProperty(property_);
    PropertyWidgetQt::generateContextMenu();
    generateWidget();
    updateFromProperty();

    if (!property->getReadOnly())
        generatesSettingsWidget();
}


IntVec3PropertyWidgetQt::~IntVec3PropertyWidgetQt() {
    settingsWidget_->deleteLater();
}

void IntVec3PropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();

    if (property_->getReadOnly()) {
        valueVec_ = property_->get();
        hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
        readOnlyLabel_ = new QLabel();
        hLayout->addWidget(readOnlyLabel_);
        setLayout(hLayout);
    }
    else {
        label_ = new EditableLabelQt(this,property_->getDisplayName(),PropertyWidgetQt::generatePropertyWidgetMenu());
        hLayout->addWidget(label_);
        QWidget* sliderWidget = new QWidget();
        QVBoxLayout* vLayout = new QVBoxLayout();
        sliderWidget->setLayout(vLayout);
        vLayout->setContentsMargins(0,0,0,0);
        vLayout->setSpacing(0);
        valueVecMax_ = property_->getMaxValue();
        valueVecMin_ = property_->getMinValue();
        valueIncrement_ = property_->getIncrement();
        sliderX_ = new IntSliderWidgetQt(valueVecMin_.x,valueVecMax_.x,valueIncrement_.x);
        sliderY_ = new IntSliderWidgetQt(valueVecMin_.y,valueVecMax_.y,valueIncrement_.y);
        sliderZ_ = new IntSliderWidgetQt(valueVecMin_.z,valueVecMax_.z,valueIncrement_.z);
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
        connect(sliderX_, SIGNAL(valueChanged(int)), this, SLOT(setPropertyValue()));
        connect(sliderY_, SIGNAL(valueChanged(int)), this, SLOT(setPropertyValue()));
        connect(sliderZ_, SIGNAL(valueChanged(int)), this, SLOT(setPropertyValue()));
    }
}

void IntVec3PropertyWidgetQt::generatesSettingsWidget() {
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

void IntVec3PropertyWidgetQt::updateFromProperty() {
    valueVec_ = property_->get();

    if (property_->getReadOnly()) {
        readOnlyLabel_->setText(QString::number(valueVec_.x)+","
                                +QString::number(valueVec_.y)+","
                                +QString::number(valueVec_.z));
        readOnlyLabel_->setToolTip("Min: [" +QString::number((property_->getMinValue()).x)+
                                   ","+QString::number((property_->getMinValue()).y)+
                                   ","+QString::number((property_->getMinValue()).z)+
                                   "] Max: [" +QString::number((property_->getMaxValue()).x)+
                                   ","+QString::number((property_->getMaxValue()).y)+
                                   ","+QString::number((property_->getMinValue()).z)+"]");
    }
    else {
        valueVecMax_ = property_->getMaxValue();
        valueVecMin_ = property_->getMinValue();
        valueIncrement_ = property_->getIncrement();
        sliderX_->setRange(valueVecMin_.x,valueVecMax_.x);
        sliderY_->setRange(valueVecMin_.y,valueVecMax_.y);
        sliderZ_->setRange(valueVecMin_.z,valueVecMax_.z);
        sliderX_->setIncrement(valueIncrement_.x);
        sliderY_->setIncrement(valueIncrement_.y);
        sliderZ_->setIncrement(valueIncrement_.z);
        sliderX_->setValue(valueVec_.x);
        sliderY_->setValue(valueVec_.y);
        sliderZ_->setValue(valueVec_.z);
        sliderX_->updateValueSpinBox();
        sliderY_->updateValueSpinBox();
        sliderZ_->updateValueSpinBox();
    }
};


void IntVec3PropertyWidgetQt::showContextMenuX(const QPoint& pos) {
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
            valueVecMin_ = property_->getMinValue();
            valueVecMin_.x = sliderX_->getValue();
            property_->setMinValue(valueVecMin_);
            updateFromProperty();
        }
        else if (selecteditem && selecteditem->text() == "Set as Max") {
            //Set current value of the slider to max value of the property
            valueVecMax_ = property_->getMaxValue();
            valueVecMax_.x = sliderX_->getValue();
            property_->setMaxValue(valueVecMax_);
            updateFromProperty();
        }
    }
}

void IntVec3PropertyWidgetQt::showContextMenuY(const QPoint& pos) {
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
            valueVecMin_ = property_->getMinValue();
            valueVecMin_.y = sliderY_->getValue();
            property_->setMinValue(valueVecMin_);
            updateFromProperty();
        }
        else if (selecteditem && selecteditem->text() == "Set as Max") {
            //Set current value of the slider to max value of the property
            valueVecMax_ = property_->getMaxValue();
            valueVecMax_.y = sliderY_->getValue();
            property_->setMaxValue(valueVecMax_);
            updateFromProperty();
        }
    }
}

void IntVec3PropertyWidgetQt::showContextMenuZ(const QPoint& pos) {
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
            valueVecMin_ = property_->getMinValue();
            valueVecMin_.z = sliderZ_->getValue();
            property_->setMinValue(valueVecMin_);
            updateFromProperty();
        }
        else if (selecteditem && selecteditem->text() == "Set as Max") {
            //Set current value of the slider to max value of the property
            valueVecMax_ = property_->getMaxValue();
            valueVecMax_.z = sliderZ_->getValue();
            property_->setMaxValue(valueVecMax_);
            updateFromProperty();
        }
    }
}

void IntVec3PropertyWidgetQt::setPropertyValue() {
    valueVec_ = property_->get();
    valueVec_.x = sliderX_->getValue();
    valueVec_.y = sliderY_->getValue();
    valueVec_.z = sliderZ_->getValue();
    property_->set(valueVec_);
    emit modified();
}

void IntVec3PropertyWidgetQt::setPropertyDisplayName() {
    property_->setDisplayName(label_->getText());
}


} //namespace