#include <inviwo/core/metadata/positionmetadata.h>

namespace inviwo {

const std::string PositionMetaData::logSource_ = "PositionMetaData";

PositionMetaData::PositionMetaData() 
    : IVec2MetaData(ivec2(0,0))
{}

PositionMetaData::PositionMetaData(int x, int y) 
    : IVec2MetaData(ivec2(x,y))
{}

PositionMetaData::~PositionMetaData() {}

PositionMetaData* PositionMetaData::create() const {
    return new PositionMetaData();
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
    MetaData::serialize(s);
    s.serialize("type", getClassName(), true);
    s.serialize("position", value_);
}

void PositionMetaData::deserialize(IvwDeserializer& d) {
    MetaData::deserialize(d);
    std::string className;
    d.deserialize("type", className, true);
    d.deserialize("position", value_);
}


} // namespace
