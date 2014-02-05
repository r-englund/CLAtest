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

#include <inviwo/qt/widgets/properties/stringpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/compositepropertywidgetqt.h>

namespace inviwo {

StringPropertyWidgetQt::StringPropertyWidgetQt(StringProperty* property) : property_(property) {
    PropertyWidgetQt::setProperty(property_);
    PropertyWidgetQt::generateContextMenu();
    generateWidget();
    updateFromProperty();
}

void StringPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    lineEdit_ = new QLineEdit;
    label_ = new EditableLabelQt(this,property_->getDisplayName(),PropertyWidgetQt::generatePropertyWidgetMenu());
    hLayout->addWidget(label_);
    connect(lineEdit_, SIGNAL(editingFinished()), this, SLOT(setPropertyValue()));
    connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
    hLayout->addWidget(lineEdit_);
    setLayout(hLayout);

    if (property_->getReadOnly())
        label_->finishEditing();
}

void StringPropertyWidgetQt::setPropertyValue() {
    std::string valueStr = lineEdit_->text().toLocal8Bit().constData();
    property_->set(valueStr);
    emit modified();
}

void StringPropertyWidgetQt::updateFromProperty() {
    lineEdit_->setText(QString::fromStdString(property_->get()));
}

void StringPropertyWidgetQt::setPropertyDisplayName() {
    property_->setDisplayName(label_->getText());
}

} // namespace