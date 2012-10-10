#include "inviwo/core/properties/property.h"
#include "inviwo/core/properties/propertyowner.h"
#include "inviwo/core/io/serialization/ivwserializable.h"

namespace inviwo {

PropertyOwner::PropertyOwner()
    : invalid_(true) {}

PropertyOwner::~PropertyOwner() {
    properties_.clear();
}

void PropertyOwner::addProperty(Property* property) {
    // TODO: check if property with same name has been added before
    properties_.push_back(property);
    property->setOwner(this);
}

void PropertyOwner::addProperty(Property& property) {
    addProperty(&property);
}

Property* PropertyOwner::getPropertyByIdentifier(std::string identifier) {
    for (size_t i=0; i<properties_.size(); i++)
        if ((properties_[i]->getIdentifier()).compare(identifier) == 0)
            return properties_[i];
    return 0;
}

void PropertyOwner::invalidate() {
    invalid_ = true;
}

void PropertyOwner::setValid() {
    invalid_ = false;
}

bool PropertyOwner::isValid() {
    return (invalid_ == false);
}

void PropertyOwner::serialize(IvwSerializer& s) const {
    s.serialize("Properties", properties_, "Property") ;
}

void PropertyOwner::deserialize(IvwDeserializer& d) {
    d.deserialize("Properties", properties_, "Property") ;
}


} // namespace
