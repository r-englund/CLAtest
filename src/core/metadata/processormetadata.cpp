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

#include <inviwo/core/metadata/processormetadata.h>

namespace inviwo {

ProcessorMetaData::ProcessorMetaData() {
    visiblityMetaData_.set(true);
    selectionMetaData_.set(false);
    /*addMetaData(positionMetaData_);
    addMetaData(visiblityMetaData_);*/
}

ProcessorMetaData::ProcessorMetaData(const ProcessorMetaData& rhs):
    positionMetaData_(rhs.positionMetaData_)
    , visiblityMetaData_(rhs.visiblityMetaData_)
    , selectionMetaData_(rhs.selectionMetaData_){
}

ProcessorMetaData& ProcessorMetaData::operator=(const ProcessorMetaData& that) {
    if (this != &that) {
        delete this;
        positionMetaData_ = that.positionMetaData_;
        visiblityMetaData_ = that.visiblityMetaData_;
        selectionMetaData_ = that.selectionMetaData_;
    }

    return *this;
}

ProcessorMetaData::~ProcessorMetaData() {}

ProcessorMetaData* ProcessorMetaData::clone() const {
    return new ProcessorMetaData(*this);
}

void ProcessorMetaData::setPosition(ivec2 pos) {
    positionMetaData_.set(pos);
}

ivec2 ProcessorMetaData::getPosition() {
    return positionMetaData_.get();
}

void ProcessorMetaData::setVisibile(bool visibility) {
    visiblityMetaData_.set(visibility);
}

bool ProcessorMetaData::isVisible() {
    return visiblityMetaData_.get();
}

void ProcessorMetaData::setSelected(bool selection) {
    selectionMetaData_.set(selection);
}

bool ProcessorMetaData::isSelected() {
    return selectionMetaData_.get();
}

void ProcessorMetaData::serialize(IvwSerializer& s) const {
    //   CompositeMetaData::serialize(s);
    s.serialize("type", getClassName(), true);
    s.serialize("position", positionMetaData_.get());
    s.serialize("visibility", visiblityMetaData_.get());
    s.serialize("selection", selectionMetaData_.get());
}

void ProcessorMetaData::deserialize(IvwDeserializer& d) {
    std::string className;
    ivec2 position(0.0f), dimension(256.0f);
    bool visibility=false;
    bool selection=false;
    //   CompositeMetaData::deserialize(d);
    d.deserialize("type", className, true);
    d.deserialize("position", position);
    d.deserialize("visibility", visibility);
    d.deserialize("selection", selection);
    //TODO check if position is within current monitors
    positionMetaData_.set(position);
    visiblityMetaData_.set(visibility);
    selectionMetaData_.set(selection);
}



} // namespace
