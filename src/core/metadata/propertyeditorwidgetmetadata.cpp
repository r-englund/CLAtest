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

PropertyEditorWidgetMetaData::PropertyEditorWidgetMetaData() 
    : positionMetaData_(ivec2(0))
    , dimensionMetaData_(ivec2(256))
    , visiblityMetaData_(false) {
}

PropertyEditorWidgetMetaData::PropertyEditorWidgetMetaData( const PropertyEditorWidgetMetaData& rhs ) 
    : positionMetaData_(rhs.positionMetaData_)
    , dimensionMetaData_(rhs.dimensionMetaData_)
    , visiblityMetaData_(rhs.visiblityMetaData_){
}

PropertyEditorWidgetMetaData& PropertyEditorWidgetMetaData::operator=( const PropertyEditorWidgetMetaData& that ){
    if(this != &that){
        delete this;
        positionMetaData_ = that.positionMetaData_;
        dimensionMetaData_ = that.dimensionMetaData_;
        visiblityMetaData_ = that.visiblityMetaData_;
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

void PropertyEditorWidgetMetaData::serialize(IvwSerializer& s) const {
    s.serialize("type", getClassName(), true);
    s.serialize("position", positionMetaData_.get());
    s.serialize("dimension", dimensionMetaData_.get());
    s.serialize("visibility", visiblityMetaData_.get());
}

void PropertyEditorWidgetMetaData::deserialize(IvwDeserializer& d) {
    std::string className;
    ivec2 position, dimension;
    bool visibility;

    d.deserialize("type", className, true);
    d.deserialize("position", position);
    d.deserialize("dimension", dimension);
    d.deserialize("visibility", visibility);

    positionMetaData_.set(position);
    dimensionMetaData_.set(dimension);
    visiblityMetaData_.set(visibility);    
}





} // namespace
