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
 * Main file authors: Timo Ropinski, Alexander Johansson
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/properties/compositepropertywidgetqt.h>

#include <inviwo/core/properties/property.h>

namespace inviwo {

CompositePropertyWidgetQt::CompositePropertyWidgetQt(CompositeProperty* property)
    : PropertyWidgetQt(property), property_(property) {
    generateWidget();
    updateFromProperty();
}

void CompositePropertyWidgetQt::generateWidget() {
    QVBoxLayout* vLayout = new QVBoxLayout();
    collapsiveGroupBoxWidget_ =
        new CollapsibleGroupBoxWidgetQt(property_->getIdentifier(), property_->getDisplayName());
    std::vector<Property*> subProperties = property_->getProperties();

    for (size_t i = 0; i < subProperties.size(); i++) {
        Property* curProperty = subProperties[i];
        collapsiveGroupBoxWidget_->addProperty(curProperty);
    }
    vLayout->addWidget(collapsiveGroupBoxWidget_);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    setLayout(vLayout);
    connect(collapsiveGroupBoxWidget_, SIGNAL(visibilityModified()),
            SLOT(setDisplayModeFromGroupBox()));
    connect(collapsiveGroupBoxWidget_, SIGNAL(labelChanged(QString)),
            SLOT(labelDidChange(QString)));
}

void CompositePropertyWidgetQt::updateFromProperty() {
    for (size_t i = 0; i < subPropertyWidgets_.size(); i++)
        subPropertyWidgets_[i]->updateFromProperty();
}

void CompositePropertyWidgetQt::propertyModified() { emit modified(); }

void CompositePropertyWidgetQt::showWidget() { this->show(); }

void CompositePropertyWidgetQt::hideWidget() { this->hide(); }

void CompositePropertyWidgetQt::setDisplayModeFromGroupBox() {
    property_->setVisibilityMode(collapsiveGroupBoxWidget_->getVisibilityMode());
}
void CompositePropertyWidgetQt::labelDidChange(QString text){
    property_->setDisplayName(text.toStdString());
}

void CompositePropertyWidgetQt::resetPropertyToDefaultState() {
    collapsiveGroupBoxWidget_->resetPropertyToDefaultState();
}



} // namespace
