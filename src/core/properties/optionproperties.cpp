#include <inviwo/core/properties/optionproperties.h>

namespace inviwo {
    OptionPropertyDouble::OptionPropertyDouble(std::string identifier, std::string displayName, std::string value, PropertySemantics::Type semantics /*= PropertySemantics::Default*/)
        : TemplatedOptionProperty<double>(identifier, displayName,value,semantics)
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

OptionPropertyFloat::OptionPropertyFloat(std::string identifier, std::string displayName, std::string value,PropertySemantics::Type semantics /*= PropertySemantics::Default*/)
    : TemplatedOptionProperty<float>(identifier, displayName,value,semantics)
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

OptionPropertyInt::OptionPropertyInt(std::string identifier, std::string displayName, std::string value, PropertySemantics::Type semantics /*= PropertySemantics::Default*/)
: TemplatedOptionProperty<int>(identifier, displayName,value,semantics)
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


OptionPropertyString::OptionPropertyString(std::string identifier, std::string displayName, std::string value, PropertySemantics::Type semantics /*= PropertySemantics::Default*/)
    : TemplatedOptionProperty<std::string>(identifier, displayName, value, semantics)
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
