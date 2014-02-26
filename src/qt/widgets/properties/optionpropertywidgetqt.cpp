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
 * Main file authors: Alexander Johansson, Sathish Kottravel
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/properties/optionpropertywidgetqt.h>
#include <QComboBox>
#include <typeinfo>
namespace inviwo {
OptionPropertyWidgetQt::OptionPropertyWidgetQt(BaseOptionProperty* property) 
    : PropertyWidgetQt(property)
    , property_(property),
    updating_(false) {
    generateWidget();
    updateFromProperty();
}

void OptionPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(0);
    comboBox_ = new QComboBox();
    fillComboBox();
    updateFromProperty();

    comboBox_->setEnabled(!property_->getReadOnly());

    label_ = new EditableLabelQt(this,property_->getDisplayName(),PropertyWidgetQt::generatePropertyWidgetMenu());
    hLayout->addWidget(label_);
    hLayout->addWidget(comboBox_);
    setLayout(hLayout);
    connect(comboBox_, SIGNAL(currentIndexChanged(int)),this, SLOT(optionChanged()));
    connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));   
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
