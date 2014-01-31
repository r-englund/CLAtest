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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <inviwo/core/metadata/propertyeditorwidgetmetadata.h>

namespace inviwo {


PropertyEditorWidgetDockStatus::PropertyEditorWidgetDockStatus() : dockStatus_("Floating") {}
PropertyEditorWidgetDockStatus::PropertyEditorWidgetDockStatus(std::string dockStatus="Floating") : dockStatus_(dockStatus) {}
PropertyEditorWidgetDockStatus::PropertyEditorWidgetDockStatus(const PropertyEditorWidgetDockStatus& rhs) : dockStatus_(rhs.dockStatus_) {};
PropertyEditorWidgetDockStatus& PropertyEditorWidgetDockStatus::operator=(const PropertyEditorWidgetDockStatus& that){
    if (this != &that) {
        dockStatus_ = that.dockStatus_;
    }
    return *this;
}
bool PropertyEditorWidgetDockStatus::operator==(const PropertyEditorWidgetDockStatus& that){
    if (this->getString() ==  that.getString())
        return true;    
    return false;
}
const std::string& PropertyEditorWidgetDockStatus::getString() const{
    return dockStatus_;
}
const PropertyEditorWidgetDockStatus PropertyEditorWidgetDockStatus::Floating("Floating");
const PropertyEditorWidgetDockStatus PropertyEditorWidgetDockStatus::DockedLeft("DockedLeft");
const PropertyEditorWidgetDockStatus PropertyEditorWidgetDockStatus::DockedRight("DockedRight");


//////////////////////////////////////////////////////////////////////////

PropertyEditorWidgetMetaData::PropertyEditorWidgetMetaData() 
    : positionMetaData_(ivec2(0))
    , dimensionMetaData_(ivec2(256))
    , visiblityMetaData_(false)
    , dockStatusMetaData_(PropertyEditorWidgetDockStatus::Floating.getString()){
}

PropertyEditorWidgetMetaData::PropertyEditorWidgetMetaData( const PropertyEditorWidgetMetaData& rhs ) 
    : positionMetaData_(rhs.positionMetaData_)
    , dimensionMetaData_(rhs.dimensionMetaData_)
    , visiblityMetaData_(rhs.visiblityMetaData_)
    , dockStatusMetaData_(PropertyEditorWidgetDockStatus::Floating.getString()){
}

PropertyEditorWidgetMetaData& PropertyEditorWidgetMetaData::operator=( const PropertyEditorWidgetMetaData& that ){
    if(this != &that){
        delete this;
        positionMetaData_ = that.positionMetaData_;
        dimensionMetaData_ = that.dimensionMetaData_;
        visiblityMetaData_ = that.visiblityMetaData_;
        dockStatusMetaData_ = that.dockStatusMetaData_;
    }
    return *this;
}

PropertyEditorWidgetMetaData::~PropertyEditorWidgetMetaData() {}

PropertyEditorWidgetMetaData* PropertyEditorWidgetMetaData::clone() const{
    return new PropertyEditorWidgetMetaData(*this);
}

void PropertyEditorWidgetMetaData::setWidgetPosition(ivec2 pos) {
    positionMetaData_.set(pos);
}

ivec2 PropertyEditorWidgetMetaData::getWidgetPosition() {
    return positionMetaData_.get();
}

void PropertyEditorWidgetMetaData::setDimension(ivec2 dim) {
    dimensionMetaData_.set(dim); 
}

ivec2 PropertyEditorWidgetMetaData::getDimension() const{
    return dimensionMetaData_.get();
}

void PropertyEditorWidgetMetaData::setVisibile(bool visibility) {
    visiblityMetaData_.set(visibility);
}

bool PropertyEditorWidgetMetaData::isVisible() const{
    return visiblityMetaData_.get();
}

void PropertyEditorWidgetMetaData::setDockStatus(PropertyEditorWidgetDockStatus& dockStatus) {
    dockStatusMetaData_.set(dockStatus.getString());
}

const PropertyEditorWidgetDockStatus PropertyEditorWidgetMetaData::getDocStatus() const {
    return PropertyEditorWidgetDockStatus(dockStatusMetaData_.get());
}

void PropertyEditorWidgetMetaData::serialize(IvwSerializer& s) const {
    s.serialize("type", getClassName(), true);
    s.serialize("position", positionMetaData_.get());
    s.serialize("dimension", dimensionMetaData_.get());
    s.serialize("visibility", visiblityMetaData_.get());
    s.serialize("dockstatus", dockStatusMetaData_.get());
}

void PropertyEditorWidgetMetaData::deserialize(IvwDeserializer& d) {
    std::string className;
    ivec2 position, dimension;
    bool visibility;
    std::string dockStatus("");

    d.deserialize("type", className, true);
    d.deserialize("position", position);
    d.deserialize("dimension", dimension);
    d.deserialize("visibility", visibility);
    d.deserialize("dockstatus", dockStatus);

    if (dockStatus.empty())
        dockStatus="Floating";

    positionMetaData_.set(position);
    dimensionMetaData_.set(dimension);
    visiblityMetaData_.set(visibility);
    dockStatusMetaData_.set(dockStatus);
}





} // namespace
