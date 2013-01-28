#include <inviwo/core/properties/compositeproperty.h>

namespace inviwo {

CompositeProperty::CompositeProperty(std::string identifier, std::string displayName)
    : Property(identifier, displayName)
{}

CompositeProperty::~CompositeProperty() {}


void CompositeProperty::addProperty(Property* property) {
    subProperties_.push_back(property);
}

void CompositeProperty::addProperty(Property& property) {
    addProperty(&property);
}

void CompositeProperty::setOwner(PropertyOwner* owner) {
    Property::setOwner(owner);
    for (size_t i=0; i<subProperties_.size(); i++)
        subProperties_[i]->setOwner(owner);
}

} // namespace
