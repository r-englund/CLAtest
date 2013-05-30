#include <inviwo/core/properties/optionproperties.h>

namespace inviwo {

OptionPropertyInt::OptionPropertyInt(std::string identifier, std::string displayName,
                                     PropertyOwner::InvalidationLevel invalidationLevel,
                                     PropertySemantics::Type semantics)
    : TemplateOptionProperty<int>(identifier, displayName, 0, invalidationLevel, semantics)
{}

OptionPropertyInt::OptionPropertyInt(std::string identifier, std::string displayName, int value,
                                     PropertyOwner::InvalidationLevel invalidationLevel,
                                     PropertySemantics::Type semantics)
    : TemplateOptionProperty<int>(identifier, displayName, value, invalidationLevel, semantics)
{}

void OptionPropertyInt::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("value", get());
}

void OptionPropertyInt::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    int value;
    d.deserialize("value", value);
    set(value);
}

OptionPropertyFloat::OptionPropertyFloat(std::string identifier, std::string displayName,
                                         PropertyOwner::InvalidationLevel invalidationLevel,
                                         PropertySemantics::Type semantics )
    : TemplateOptionProperty<float>(identifier, displayName, 0.0f, invalidationLevel, semantics)
{}

OptionPropertyFloat::OptionPropertyFloat(std::string identifier, std::string displayName, float value,
                                         PropertyOwner::InvalidationLevel invalidationLevel,
                                         PropertySemantics::Type semantics )
    : TemplateOptionProperty<float>(identifier, displayName, value, invalidationLevel, semantics)
{}

void OptionPropertyFloat::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("value", get());
}

void OptionPropertyFloat::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    float value;
    d.deserialize("value", value);
    set(value);
}


OptionPropertyDouble::OptionPropertyDouble(std::string identifier, std::string displayName,
                                           PropertyOwner::InvalidationLevel invalidationLevel,
                                           PropertySemantics::Type semantics)
    : TemplateOptionProperty<double>(identifier, displayName, 0.0, invalidationLevel, semantics)
{}

OptionPropertyDouble::OptionPropertyDouble(std::string identifier, std::string displayName, double value,
                                           PropertyOwner::InvalidationLevel invalidationLevel,
                                           PropertySemantics::Type semantics)
    : TemplateOptionProperty<double>(identifier, displayName, value, invalidationLevel, semantics)
{}

void OptionPropertyDouble::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("value", get());
}

void OptionPropertyDouble::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    double value;
    d.deserialize("value", value);
    set(value);
}


OptionPropertyString::OptionPropertyString(std::string identifier, std::string displayName,
                                           PropertyOwner::InvalidationLevel invalidationLevel,
                                           PropertySemantics::Type semantics)
                                           : TemplateOptionProperty<std::string>(identifier, displayName, identifier, invalidationLevel, semantics)
{}

OptionPropertyString::OptionPropertyString(std::string identifier, std::string displayName, std::string value,
                                           PropertyOwner::InvalidationLevel invalidationLevel,
                                           PropertySemantics::Type semantics)
                                           : TemplateOptionProperty<std::string>(identifier, displayName, value, invalidationLevel, semantics)
{}

void OptionPropertyString::addOption(std::string identifier, std::string displayName) {
    TemplateOptionProperty<std::string>::addOption(identifier, displayName, identifier);
}

void OptionPropertyString::addOption(std::string identifier, std::string displayName, std::string value) {
    TemplateOptionProperty<std::string>::addOption(identifier, displayName, value);
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
