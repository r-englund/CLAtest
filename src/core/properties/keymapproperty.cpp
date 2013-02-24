#include <inviwo/core/properties/keymapproperty.h>

namespace inviwo {

    KeyMapProperty::KeyMapProperty(std::string identifier, std::string displayName, std::string value)
        : TemplateProperty<std::string>(identifier, displayName, value)
    {}

    void KeyMapProperty::serialize(IvwSerializer& s) const {
        Property::serialize(s) ;
        s.serialize("value", get());
    }

    void KeyMapProperty::deserialize(IvwDeserializer& d) {
        Property::deserialize(d) ;
        std::string value;
        d.deserialize("value", value);
        set(value);
    }

} // namespace
