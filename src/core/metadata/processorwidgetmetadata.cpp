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

#include <inviwo/core/metadata/processorwidgetmetadata.h>

namespace inviwo {

ProcessorWidgetMetaData::ProcessorWidgetMetaData() 
    : positionMetaData_(ivec2(0)),
      dimensionMetaData_(ivec2(256)),
      visiblityMetaData_(false),
      CompositeMetaData()
{
    addMetaData(positionMetaData_);
    addMetaData(dimensionMetaData_);
    addMetaData(visiblityMetaData_);
}

ProcessorWidgetMetaData::~ProcessorWidgetMetaData() {}

ProcessorWidgetMetaData* ProcessorWidgetMetaData::create() const {
    return new ProcessorWidgetMetaData();
}

void ProcessorWidgetMetaData::setWidgetPosition(ivec2 pos) {
    positionMetaData_.set(pos);
}

ivec2 ProcessorWidgetMetaData::getWidgetPosition() {
    return positionMetaData_.get();
}

void ProcessorWidgetMetaData::setDimension(ivec2 dim) {
    dimensionMetaData_.set(dim); 
}

ivec2 ProcessorWidgetMetaData::getDimension() const{
    return dimensionMetaData_.get();
}

void ProcessorWidgetMetaData::setVisibile(bool visibility) {
    visiblityMetaData_.set(visibility);
}

bool ProcessorWidgetMetaData::isVisible() const{
    return visiblityMetaData_.get();
}

void ProcessorWidgetMetaData::serialize(IvwSerializer& s) const {
    CompositeMetaData::serialize(s);    
    s.serialize("type", getClassName(), true);
    s.serialize("position", positionMetaData_.get());
    s.serialize("dimension", dimensionMetaData_.get());
    s.serialize("visibility", visiblityMetaData_.get());
}

void ProcessorWidgetMetaData::deserialize(IvwDeserializer& d) {
    std::string className;
    ivec2 position, dimension;
    bool visibility;

    CompositeMetaData::deserialize(d);
    d.deserialize("type", className, true);
    d.deserialize("position", position);
    d.deserialize("dimension", dimension);
    d.deserialize("visibility", visibility);

    positionMetaData_.set(position);
    dimensionMetaData_.set(dimension);
    visiblityMetaData_.set(visibility);    
}

} // namespace
