#include "inviwo/core/metadata/positionmetadata.h"

namespace inviwo {

const std::string PositionMetaData::logSource_ = "PositionMetaData";

PositionMetaData::PositionMetaData() 
    : identifier_("undefined"),
      _xy(0,0)
{
}

PositionMetaData::PositionMetaData(int x, int y) 
    : identifier_("undefined"),
      _xy(x,y)
{
}

PositionMetaData::~PositionMetaData() {}

PositionMetaData* PositionMetaData::create() const {
    return new PositionMetaData();
}

ivec2 PositionMetaData::getXY() {
    return _xy;
}

void PositionMetaData::setXY(const ivec2 &xy) {
    _xy = xy;
}

int PositionMetaData::getX() {
    return _xy[0];
}

void PositionMetaData::setX(const int &x) {
    _xy[0] = x;
}

int PositionMetaData::getY() {
    return _xy[1];
}

void PositionMetaData::setY(const int &y) {
    _xy[1] = y;
}

void PositionMetaData::serialize(IvwSerializer& s) const {
    MetaData::serialize(s);
    s.serialize("type", getClassName(), true);
    s.serialize("position", _xy);
}

void PositionMetaData::deserialize(IvwDeserializer& d) {
    MetaData::deserialize(d);
    std::string className;
    d.deserialize("type", className, true);
    d.deserialize("position", _xy);
}


} // namespace
