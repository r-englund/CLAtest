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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include <inviwo/qt/widgets/properties/compositepropertywidgetqt.h>

#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>

namespace inviwo {

CompositePropertyWidgetQt::CompositePropertyWidgetQt(CompositeProperty* property) : property_(property) {
    PropertyWidgetQt::setProperty(property_);
    PropertyWidgetQt::generateContextMenu();
    generateWidget();
    updateFromProperty();

}



void CompositePropertyWidgetQt::generateWidget() {
    QVBoxLayout* vLayout = new QVBoxLayout();
    collapsiveGroupBoxWidget_ = new CollapsiveGroupBoxWidgetQt(property_->getIdentifier(),property_->getDisplayName());
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
    connect(collapsiveGroupBoxWidget_,SIGNAL(visibilityModified()),SLOT(setDisplayModeFromGroupBox()));
}

void CompositePropertyWidgetQt::updateFromProperty() {
    for (size_t i=0; i<subPropertyWidgets_.size(); i++)
        subPropertyWidgets_[i]->updateFromProperty();
}

void CompositePropertyWidgetQt::propertyModified() {
    emit modified();
}

void CompositePropertyWidgetQt::showWidget() {
    this->show();
}

void CompositePropertyWidgetQt::hideWidget() {
    this->hide();
}

void CompositePropertyWidgetQt::setDisplayModeFromGroupBox() {
    property_->setVisibility(collapsiveGroupBoxWidget_->getVisibilityMode());
}



} // namespace
