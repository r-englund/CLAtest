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

#include <inviwo/core/properties/propertywidget.h>

namespace inviwo {

PropertyWidget::PropertyWidget()
    : property_(0), propertyEditor_(0) {
}

PropertyWidget::PropertyWidget(Property* property)
    : property_(property), propertyEditor_(0) {
}

PropertyWidget::~PropertyWidget() {
}

Property* PropertyWidget::getProperty() {
    return property_;
}

void PropertyWidget::setProperty(Property* property) {
    property_ = property;
}

void PropertyWidget::setEditorWidget(PropertyEditorWidget* propertyEditorWidget) {
    propertyEditor_ = propertyEditorWidget;
}

PropertyEditorWidget* PropertyWidget::getEditorWidget() const {
    return propertyEditor_;
}

bool PropertyWidget::hasEditorWidget() const {
    return (propertyEditor_ != 0);
}

//////////////////////////////////////////////////////////////////////////

//Additional widgets owned by property

PropertyEditorWidget::PropertyEditorWidget()
    : metaData_(0) {
}

PropertyEditorWidget::~PropertyEditorWidget() {
}

void PropertyEditorWidget::initialize(Property* property) {
    metaData_ = dynamic_cast<PropertyEditorWidgetMetaData*>(property->getMetaData("PropertyEditorWidgetMetaData"));
}

void PropertyEditorWidget::setEditorVisibility(bool visible) {
    metaData_->setVisibile(visible);
}

void PropertyEditorWidget::showEditor() {
    metaData_->setVisibile(true);
}

void PropertyEditorWidget::hideEditor() {
    metaData_->setVisibile(false);
}

void PropertyEditorWidget::setEditorDimension(ivec2 dimension) {
    metaData_->setDimension(dimension);
}

void PropertyEditorWidget::moveEditor(ivec2 pos) {
    metaData_->setWidgetPosition(pos);
}

void PropertyEditorWidget::setDockStatus(PropertyEditorWidgetDockStatus dockStatus) {
    metaData_->setDockStatus(dockStatus);
}
bool PropertyEditorWidget::getEditorVisibilityMetaData() {
    return metaData_->isVisible();
}

ivec2 PropertyEditorWidget::getEditorPositionMetaData() {
    return metaData_->getWidgetPosition();
}

ivec2 PropertyEditorWidget::getEditorDimensionMetaData() {
    return metaData_->getDimension();
}

PropertyEditorWidgetDockStatus PropertyEditorWidget::getEditorDockStatus() {
    return metaData_->getDocStatus();
}

} // namespace
