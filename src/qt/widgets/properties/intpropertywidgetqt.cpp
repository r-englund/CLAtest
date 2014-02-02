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

#include <inviwo/qt/widgets/properties/intpropertywidgetqt.h>

#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

IntPropertyWidgetQt::IntPropertyWidgetQt(IntProperty* property) : property_(property) , settingsWidget_(0) {
    PropertyWidgetQt::setProperty(property_);
    PropertyWidgetQt::generateContextMenu();
    generateWidget();
    updateFromProperty();
}


IntPropertyWidgetQt::~IntPropertyWidgetQt() {
    delete settingsWidget_;//->deleteLater();
}

void IntPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();

    if (property_->getReadOnly()) {
        hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
        readOnlyLabel_ = new QLabel();
        hLayout->addWidget(readOnlyLabel_);
        setLayout(hLayout);
    }
    else {
        label_ = new EditableLabelQt(this,property_->getDisplayName(),PropertyWidgetQt::generatePropertyWidgetMenu());
        hLayout->addWidget(label_);
        sliderWidget_ = new IntSliderWidgetQt(property_->getMinValue(), property_->getMaxValue(), property_->getIncrement());
        connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
        connect(sliderWidget_, SIGNAL(valueChanged(int)), this, SLOT(setPropertyValue()));
        hLayout->addWidget(sliderWidget_);
        setLayout(hLayout);
        QSizePolicy labelPol = label_->sizePolicy();
        labelPol.setHorizontalStretch(1);
        label_->setSizePolicy(labelPol);
        QSizePolicy sliderPol = sliderWidget_->sizePolicy();
        sliderPol.setHorizontalStretch(3);
        sliderWidget_->setSizePolicy(sliderPol);
        generatesSettingsWidget();
    }
}

void IntPropertyWidgetQt::updateFromProperty() {
    if (property_->getReadOnly()) {
        readOnlyLabel_->setText(QString::number(property_->get()));
        readOnlyLabel_->setToolTip("Min: " +QString::number(property_->getMinValue())+
                                   "  Max: " +QString::number(property_->getMaxValue()));
    }
    else {
        sliderWidget_->setRange(property_->getMinValue(), property_->getMaxValue());
        sliderWidget_->setValue(property_->get());
        sliderWidget_->setIncrement(property_->getIncrement());
    }
}


void IntPropertyWidgetQt::showContextMenu(const QPoint& pos) {
    PropertyVisibilityMode appVisibilityMode  = getApplicationViewMode();

    if (appVisibilityMode == DEVELOPMENT) {
        updateContextMenu();
        QPoint globalPos = sliderWidget_->mapToGlobal(pos);
        QAction* selecteditem = settingsMenu_->exec(globalPos);

        if (selecteditem && selecteditem->text() == "Property settings") {
            settingsWidget_->reload();
            settingsWidget_->show();
        }
        else if (selecteditem && selecteditem->text() == "Property settings") {
            //Set current value of the slider to min value of the property
            property_->setMinValue(sliderWidget_->getValue());
            updateFromProperty();
        }
        else if (selecteditem && selecteditem->text() == "Set as Max") {
            //Set current value of the slider to max value of the property
            property_->setMaxValue(sliderWidget_->getValue());
            updateFromProperty();
        }
    }
}

void IntPropertyWidgetQt::generatesSettingsWidget() {
    settingsWidget_ = new PropertySettingsWidgetQt(property_);
    settingsMenu_ = PropertyWidgetQt::generatePropertyWidgetMenu();
    settingsMenu_->addAction("Property settings");
    settingsMenu_->addAction("Set as Min");
    settingsMenu_->addAction("Set as Max");
    sliderWidget_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(sliderWidget_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenu(const QPoint&)));
    connect(developerViewModeAction_,SIGNAL(triggered(bool)),this, SLOT(setDeveloperViewMode(bool)));
    connect(applicationViewModeAction_,SIGNAL(triggered(bool)),this, SLOT(setApplicationViewMode(bool)));
    updateContextMenu();
}

void IntPropertyWidgetQt::setPropertyValue() {
    property_->set(sliderWidget_->getValue());
    emit modified();
}

void IntPropertyWidgetQt::setPropertyDisplayName() {
    property_->setDisplayName(label_->getText());
}

} // namespace
