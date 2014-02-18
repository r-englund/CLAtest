/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Main file authors: Alexander Johansson, Timo Ropinski
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/properties/camerapropertywidgetqt.h>

#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

CameraPropertyWidgetQt::CameraPropertyWidgetQt(CameraProperty* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void CameraPropertyWidgetQt::generateWidget() {
    QVBoxLayout* vLayout = new QVBoxLayout();
    collapsiveGroupBoxWidget_ = new CollapsibleGroupBoxWidgetQt(property_->getIdentifier(),property_->getDisplayName());
    std::vector<Property*> subProperties = property_->getSubProperties();

    for (size_t i=0; i<subProperties.size(); i++) {
        Property* curProperty = subProperties[i];
        collapsiveGroupBoxWidget_->addProperty(curProperty);
    }

    collapsiveGroupBoxWidget_->generatePropertyWidgets();
    vLayout->addWidget(collapsiveGroupBoxWidget_);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(0);
    setLayout(vLayout);
}

void CameraPropertyWidgetQt::setPropertyValue() {
}

void CameraPropertyWidgetQt::updateFromProperty() {
};

void CameraPropertyWidgetQt::setPropertyDisplayName() {
    property_->setDisplayName(label_->getText());
}

void CameraPropertyWidgetQt::visibilityModified(int mode) {
    property_->setVisibility(static_cast<PropertyVisibilityMode>(mode));
    std::vector<Property*> subProperties = property_->getSubProperties();

    for (size_t i=0; i<subProperties.size(); i++)
        subProperties[i]->setVisibility(static_cast<PropertyVisibilityMode>(mode));
}

} // namespace
