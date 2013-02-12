#include <inviwo/core/properties/stringoptionproperty.h>

namespace inviwo {

    StringOptionProperty::StringOptionProperty(std::string identifier, std::string displayName, std::string value, int selectedOption)
        : TemplatedOptionProperty<std::string>(identifier, displayName, value, selectedOption)
    {}

    void StringOptionProperty::serialize(IvwSerializer& s) const {
        Property::serialize(s) ;
        s.serialize("value", get());
    }

    void StringOptionProperty::deserialize(IvwDeserializer& d) {
        Property::deserialize(d) ;
        std::string value;
        d.deserialize("value", value);
        set(value);
    }

} // namespace
