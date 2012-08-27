#include "inviwo/core/properties/property.h"

namespace inviwo {

Property::Property(std::string identifier, std::string displayName)
    : identifier_(identifier),
      displayName_(displayName)
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

} // namespace
