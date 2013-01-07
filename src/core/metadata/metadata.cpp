#include "inviwo/core/metadata/metadata.h"

namespace inviwo {

const std::string MetaData::logSource_ = "MetaData";

MetaData::MetaData() {
}

MetaData::~MetaData() {}

MetaData* MetaData::create() const {
    return new MetaData();
}

void MetaData::serialize(IvwSerializer& ) const {    
}

void MetaData::deserialize(IvwDeserializer& ) {
}

} // namespace
