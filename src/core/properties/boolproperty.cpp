#include <inviwo/core/properties/boolproperty.h>

namespace inviwo {

    BoolProperty::BoolProperty(std::string identifier, std::string displayName, bool value,
                               PropertyOwner::InvalidationLevel invalidationLevel, 
                               PropertySemantics::Type semantics)
    : TemplateProperty<bool>(identifier, displayName, value, invalidationLevel, semantics)
{}

void BoolProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("value", get());
}

void BoolProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    bool value;
    d.deserialize("value", value);
    set(value);
}


int BoolProperty::getVariantType() {
    return Variant::VariantTypeBool;
}

Variant BoolProperty::getVariant() {
    return Variant(get());
}

void  BoolProperty::setVariant(const Variant& val) {
    if (val.canConvert(getVariantType())) {
        set(val.getBool());
    }
}


} // namespace
