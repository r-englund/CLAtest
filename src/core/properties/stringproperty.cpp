#include <inviwo/core/properties/stringproperty.h>

namespace inviwo {

StringProperty::StringProperty(std::string identifier, std::string displayName, std::string value)
    : TemplateProperty<std::string>(identifier, displayName, value)
{}

void StringProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("value", get());
}

void StringProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    std::string value;
    d.deserialize("value", value);
    set(value);
}

} // namespace
