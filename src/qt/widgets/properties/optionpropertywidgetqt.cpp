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
 * Primary author : Alexander Johansson
 *
 **********************************************************************/

#include <inviwo/qt/widgets/properties/optionpropertywidgetqt.h>
#include <QComboBox>
#include <typeinfo>
namespace inviwo {
OptionPropertyWidgetQt::OptionPropertyWidgetQt(BaseOptionProperty* property) : property_(property),
    updating_(false) {
    PropertyWidgetQt::setProperty(property_);
    PropertyWidgetQt::generateContextMenu();
    generateWidget();
    updateFromProperty();
}

void OptionPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    comboBox_ = new QComboBox();
    fillComboBox();
    updateFromProperty();

    if (property_->getReadOnly()) {
        comboBox_->setDisabled(true);
        hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
        hLayout->addWidget(comboBox_);
        setLayout(hLayout);
    }
    else {
        label_ = new EditableLabelQt(this,property_->getDisplayName(),PropertyWidgetQt::generatePropertyWidgetMenu());
        hLayout->addWidget(label_);
        hLayout->addWidget(comboBox_);
        setLayout(hLayout);
        connect(comboBox_, SIGNAL(currentIndexChanged(int)),this, SLOT(optionChanged()));
        connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
    }
}

void OptionPropertyWidgetQt::fillComboBox() {
    comboBox_->clear();

    for (int i=0; i<property_->numOptions(); i++) {
        QString option = QString::fromStdString(property_->getOptionDisplayNames()[i]);
        comboBox_->addItem(option);
    }
}
void OptionPropertyWidgetQt::optionChanged() {
    if (!updating_) {
        if (comboBox_->count() && comboBox_->currentIndex()>=0) {
            property_->setSelectedOption(comboBox_->currentIndex());
            emit modified();
        }
    }
}

void OptionPropertyWidgetQt::updateFromProperty() {
    updating_ = true;
    int index = property_->getSelectedOption();
    fillComboBox();
    comboBox_->setCurrentIndex(index);
    updating_ = false;
}

void OptionPropertyWidgetQt::setPropertyDisplayName() {
    property_->setDisplayName(label_->getText());
}

} // namespace
