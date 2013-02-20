#include <inviwo/core/properties/optionproperties.h>

namespace inviwo {
    OptionPropertyDouble::OptionPropertyDouble(std::string identifier, std::string displayName, std::string value)
        : TemplatedOptionProperty<double>(identifier, displayName, value)
    {}

    void OptionPropertyDouble::serialize(IvwSerializer& s) const {
        Property::serialize(s) ;
        s.serialize("value", get());
    }

    void OptionPropertyDouble::deserialize(IvwDeserializer& d) {
        Property::deserialize(d) ;
        std::string value;
        d.deserialize("value", value);
        set(value);
    }

OptionPropertyFloat::OptionPropertyFloat(std::string identifier, std::string displayName, std::string value)
    : TemplatedOptionProperty<float>(identifier, displayName, value)
{}

void OptionPropertyFloat::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("value", get());
}

void OptionPropertyFloat::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    std::string value;
    d.deserialize("value", value);
    set(value);
}

OptionPropertyInt::OptionPropertyInt(std::string identifier, std::string displayName, std::string value)
: TemplatedOptionProperty<int>(identifier, displayName, value)
{}

void OptionPropertyInt::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("value", get());
}

void OptionPropertyInt::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    std::string value;
    d.deserialize("value", value);
    set(value);
}


OptionPropertyString::OptionPropertyString(std::string identifier, std::string displayName, std::string value)
    : TemplatedOptionProperty<std::string>(identifier, displayName, value)
{}

void OptionPropertyString::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("value", get());
}

void OptionPropertyString::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    std::string value;
    d.deserialize("value", value);
    set(value);
}




} // namespace
