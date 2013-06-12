#include <inviwo/core/properties/property.h>

namespace inviwo {

Property::Property(std::string identifier, std::string displayName, PropertyOwner::InvalidationLevel invalidationLevel, PropertySemantics::Type semantics)
: identifier_(identifier),
  displayName_(displayName),
  invalidationLevel_(invalidationLevel),
  semantics_(semantics),
  owner_(0),
  visible_(true),
  groupID_(""),
  VoidObservable()
{}

Property::Property()
    : identifier_(""),
      displayName_("")
{}

std::string Property::getIdentifier() const {
    return identifier_;
}

void Property::setIdentifier(const std::string& identifier) {
    identifier_ = identifier;
}

std::string Property::getDisplayName() const {
    return displayName_;
}

void Property::setDisplayName(const std::string& displayName) {
    displayName_ = displayName;
}

void Property::setSemantics( const PropertySemantics::Type& semantics) {
    semantics_ = semantics;
}

inviwo::PropertySemantics::Type Property::getSemantics() const {
    return semantics_;
}

PropertyOwner* Property::getOwner() {
    return owner_;
}

void Property::setOwner(PropertyOwner* owner) {
    owner_ = owner;
}

void Property::registerPropertyWidget(PropertyWidget* propertyWidget) {
    propertyWidgets_.push_back(propertyWidget);
}

void Property::updatePropertyWidgets() {
    for (size_t i=0; i<propertyWidgets_.size(); i++)
        propertyWidgets_[i]->updateFromProperty();
}

void Property::invalidate() {
    invalid_ = true;
}

void Property::setValid() {
    invalid_ = false;
}

bool Property::isValid() {
    return (invalid_ == false);
}

Variant Property::getVariant() {
    return Variant(0);
}

void Property::setVariant(const Variant&) {}

int Property::getVariantType() {
    return Variant::VariantTypeInvalid;
}

void Property::serialize(IvwSerializer& s) const {
    s.serialize("type", getClassName(), true);
    s.serialize("identifier", identifier_, true);
    s.serialize("displayName", displayName_, true);
}

void Property::deserialize(IvwDeserializer& d) {
    std::string className;
    d.deserialize("type", className, true);
    d.deserialize("identifier", identifier_, true);
    d.deserialize("displayName", displayName_, true);
}

} // namespace
