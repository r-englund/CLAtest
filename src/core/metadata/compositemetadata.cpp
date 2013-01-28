#include <inviwo/core/metadata/compositemetadata.h>

namespace inviwo {

const std::string CompositeMetaData::logSource_ = "CompositeMetaData";

CompositeMetaData::CompositeMetaData()
    : MetaData()
{}

CompositeMetaData::~CompositeMetaData() {}

CompositeMetaData* CompositeMetaData::create() const {
    return new CompositeMetaData();
}

void CompositeMetaData::addMetaData(MetaData* metadata) {
    subMetaData_.push_back(metadata);
}

void CompositeMetaData::addMetaData(MetaData& metadata) {
    addMetaData(&metadata);
}

void  CompositeMetaData::serialize(IvwSerializer& s) const {
    MetaData::serialize(s);
}

void  CompositeMetaData::deserialize(IvwDeserializer& d) {
   MetaData::deserialize(d);
}

} // namespace
