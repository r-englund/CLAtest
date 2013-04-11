#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/properties/templateproperty.h>

namespace inviwo {

CompositeProperty::CompositeProperty(std::string identifier, std::string displayName,PropertySemantics::Type semantics /*= PropertySemantics::Default*/)
: Property(identifier, displayName,semantics)
{}

CompositeProperty::~CompositeProperty() {}


void CompositeProperty::addProperty(Property* property) {
    subProperties_.push_back(property);
    property->addObserver(this);
}

void CompositeProperty::addProperty(Property& property) {
    addProperty(&property);
}

void CompositeProperty::setOwner(PropertyOwner* owner) {
    Property::setOwner(owner);
    for (size_t i=0; i<subProperties_.size(); i++)
        subProperties_[i]->setOwner(owner);
}

void CompositeProperty::notify() {
    updatePropertyWidgets();
}


} // namespace
