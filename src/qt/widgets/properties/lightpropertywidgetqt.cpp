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

#include <inviwo/qt/widgets/properties/lightpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/compositepropertywidgetqt.h>

namespace inviwo {

LightPropertyWidgetQt::LightPropertyWidgetQt(IntVec3Property* property) : property_(property) {
    PropertyWidgetQt::setProperty(property_);
    PropertyWidgetQt::generateContextMenu();
    generateWidget();
    updateFromProperty();
}

void LightPropertyWidgetQt::generateWidget() {
    
    setObjectName("LightPropertyWidgetQt");
    valueVec_ = new ivec3();
    lightWidget_ = new LightPositionWidgetQt();
    QHBoxLayout* hLayout = new QHBoxLayout();
    label_ = new EditableLabelQt(property_->getDisplayName(),PropertyWidgetQt::generatePropertyWidgetMenu());
    hLayout->addWidget(label_);
    connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
    connect(lightWidget_,SIGNAL(positionChanged()),this,SLOT(setPropertyValue()));
    hLayout->addWidget(lightWidget_);
    setLayout(hLayout);
    if (property_->getReadOnly()) 
        label_->editingOff();

    lightWidget_->setPosition(property_->get());

}

void LightPropertyWidgetQt::setPropertyValue() {
    
    double x = lightWidget_->getX();
    double y = lightWidget_->getY();
    double r = lightWidget_->getR();
    double z = qPow(r,2)-qPow(x,2)-qPow(y,2);
    double result = sqrt(z <= 0 ? 0 : z);

    valueVec_->x = x;
    valueVec_->y = y;
    valueVec_->z = result;
    property_->set(*valueVec_);
    emit modified();
}

void LightPropertyWidgetQt::updateFromProperty() {


    lightWidget_->setPosition(property_->get());

}

void LightPropertyWidgetQt::setPropertyDisplayName(){
    property_->setDisplayName(label_->getText());
}

} // namespace