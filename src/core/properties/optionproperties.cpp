#include <inviwo/core/properties/optionproperties.h>

namespace inviwo {

OptionPropertyDouble::OptionPropertyDouble(std::string identifier, std::string displayName, std::string value,
                                       PropertyOwner::InvalidationLevel invalidationLevel,
                                       PropertySemantics::Type semantics )
    : TemplatedOptionProperty<double>(identifier, displayName,value, invalidationLevel, semantics)
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

OptionPropertyFloat::OptionPropertyFloat(std::string identifier, std::string displayName, std::string value,
                                         PropertyOwner::InvalidationLevel invalidationLevel,
                                         PropertySemantics::Type semantics )
    : TemplatedOptionProperty<float>(identifier, displayName, value, invalidationLevel, semantics)
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

OptionPropertyInt::OptionPropertyInt(std::string identifier, std::string displayName, std::string value,
                                     PropertyOwner::InvalidationLevel invalidationLevel,
                                     PropertySemantics::Type semantics )
: TemplatedOptionProperty<int>(identifier, displayName, value, invalidationLevel, semantics)
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


OptionPropertyString::OptionPropertyString(std::string identifier, std::string displayName, std::string value,
                                           PropertyOwner::InvalidationLevel invalidationLevel,
                                           PropertySemantics::Type semantics )
    : TemplatedOptionProperty<std::string>(identifier, displayName, value, invalidationLevel, semantics)
{}

void OptionPropertyString::addOption(std::string optionIdentifier, std::string optionDisplayName) {
    TemplatedOptionProperty<std::string>::addOption(optionIdentifier, optionDisplayName, optionIdentifier);
}

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
