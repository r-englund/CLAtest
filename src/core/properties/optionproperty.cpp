#include <inviwo/core/properties/optionproperty.h>

namespace inviwo {

OptionProperty::OptionProperty(std::string identifier, std::string displayName, bool value)
    : Property(identifier, displayName)
{}

void OptionProperty::serialize(IvwSerializer& s) const {
}

void OptionProperty::deserialize(IvwDeserializer& d) {
}

} // namespace
