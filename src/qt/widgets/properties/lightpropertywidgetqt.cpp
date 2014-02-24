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

#include <inviwo/qt/widgets/properties/lightpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/compositepropertywidgetqt.h>

namespace inviwo {

LightPropertyWidgetQt::LightPropertyWidgetQt(FloatVec3Property* property) : property_(property) {
    PropertyWidgetQt::setProperty(property_);
    PropertyWidgetQt::generateContextMenu();
    generateWidget();
    updateFromProperty();
}

LightPropertyWidgetQt::~LightPropertyWidgetQt() {
    delete lightWidget_;
}

void LightPropertyWidgetQt::generateWidget() {
    lightWidget_ = new LightPositionWidgetQt();

    label_ = new EditableLabelQt(this,property_->getDisplayName(),PropertyWidgetQt::generatePropertyWidgetMenu());
    radiusLabel_ = new QLabel(this);
    radiusLabel_->setText("Radius");
    radiusSpinBox_ = new CustomDoubleSpinBoxQt(this);
    radiusSpinBox_->setSingleStep(0.1);
    radiusSpinBox_->setKeyboardTracking(false); // don't emit the valueChanged() signal while typing
    connect(label_, SIGNAL(textChanged()), this, SLOT(setPropertyDisplayName()));
    connect(lightWidget_,SIGNAL(positionChanged()), this, SLOT(onPositionLightWidgetChanged()));
    connect(radiusSpinBox_, SIGNAL(valueChanged(double)), this, SLOT(onRadiusSpinBoxChanged(double)));

    QGridLayout* layout = new QGridLayout();
    layout->addWidget(label_, 0, 0);
    layout->addWidget(lightWidget_, 0, 1);
    layout->addWidget(radiusLabel_, 1, 0);
    layout->addWidget(radiusSpinBox_, 1, 1);
    setLayout(layout);


    if (property_->getReadOnly())
        label_->finishEditing();
}

void LightPropertyWidgetQt::onPositionLightWidgetChanged() {
    property_->set(lightWidget_->getPosition());
    emit modified();
}

void LightPropertyWidgetQt::onRadiusSpinBoxChanged(double radius) {
    lightWidget_->setRadius(radius);
}

void LightPropertyWidgetQt::updateFromProperty() {
    // Prevent widgets from signaling changes
    // just after setting them
    lightWidget_->blockSignals(true);
    radiusSpinBox_->blockSignals(true);

    lightWidget_->setPosition(property_->get());
    radiusSpinBox_->setValue(glm::length(property_->get()));

    lightWidget_->blockSignals(false);
    radiusSpinBox_->blockSignals(false);
}

void LightPropertyWidgetQt::setPropertyDisplayName() {
    property_->setDisplayName(label_->getText());
}

} // namespace