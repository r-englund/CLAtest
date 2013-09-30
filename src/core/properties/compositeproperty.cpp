#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/properties/templateproperty.h>

namespace inviwo {

CompositeProperty::CompositeProperty(std::string identifier, std::string displayName,
                                     PropertyOwner::InvalidationLevel invalidationLevel,
                                     PropertySemantics::Type semantics)
: Property(identifier, displayName, invalidationLevel, semantics)
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
    updateVisibility();
}

void CompositeProperty::updateVisibility(){
    for (size_t i=0; i<subProperties_.size(); i++)
        subProperties_[i]->updateVisibility();
}

void CompositeProperty::setVisible( bool val ) {
    for (size_t i=0; i<subProperties_.size(); i++)
        subProperties_[i]->setVisible(val);
}

void CompositeProperty::setReadOnly( bool value ) {
    for (size_t i=0; i<subProperties_.size(); i++)
        subProperties_[i]->setReadOnly(value);
}


} // namespace
