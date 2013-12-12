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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <inviwo/core/metadata/positionmetadata.h>

namespace inviwo {

PositionMetaData::PositionMetaData() 
    : IVec2MetaData(ivec2(0,0))
{}

PositionMetaData::PositionMetaData(int x, int y) 
    : IVec2MetaData(ivec2(x,y))
{}

PositionMetaData::~PositionMetaData() {}

PositionMetaData* PositionMetaData::clone() const {
    return new PositionMetaData(*this);
}

ivec2 PositionMetaData::getXY() {
    return get();
}

void PositionMetaData::setXY(const ivec2 &xy) {
    set(xy);
}

int PositionMetaData::getX() {
    return get().x;
}

void PositionMetaData::setX(const int &x) {
    ivec2 value = get();
    value.x = x;
    set(value);
}

int PositionMetaData::getY() {
    return get().y;
}

void PositionMetaData::setY(const int &y) {
    ivec2 value = get();
    value.y = y;
    set(value);
}

void PositionMetaData::serialize(IvwSerializer& s) const {
    s.serialize("type", getClassName(), true);
    s.serialize("position", value_);
}

void PositionMetaData::deserialize(IvwDeserializer& d) {
    std::string className;
    d.deserialize("type", className, true);
    d.deserialize("position", value_);
}


} // namespace
