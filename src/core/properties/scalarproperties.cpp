#include <inviwo/core/properties/scalarproperties.h>

namespace inviwo {

FloatProperty::FloatProperty(std::string identifier, std::string displayName, float value,
                             float minValue, float maxValue, float increment,
                             PropertyOwner::InvalidationLevel invalidationLevel,
                             PropertySemantics::Type semantics)
    : OrdinalProperty<float>(identifier, displayName, value, minValue, maxValue, increment,
                             invalidationLevel, semantics)
{}

int FloatProperty::getVariantType() {
    return Variant::VariantTypeFloat;
}

Variant FloatProperty::getVariant() {
    return Variant(get());
}

void  FloatProperty::setVariant(const Variant& val) {
    if (val.canConvert(getVariantType())) {
        set(val.getFloat());
    }
}

void FloatProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("value", get());
    s.serialize("minvalue", OrdinalProperty<float>::getMinValue());
    s.serialize("maxvalue", OrdinalProperty<float>::getMaxValue());
    s.serialize("increment", OrdinalProperty<float>::getIncrement());
}

void FloatProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    float value;
    d.deserialize("value", value);
    set(value);

    d.deserialize("minvalue",  value);
    OrdinalProperty<float>::setMinValue(value);

    d.deserialize("maxvalue", value);
    OrdinalProperty<float>::setMaxValue(value);

    d.deserialize("increment", value);
    OrdinalProperty<float>::setIncrement(value);
}

IntProperty::IntProperty(std::string identifier, std::string displayName, int value,
                         int minValue, int maxValue, int increment,
                         PropertyOwner::InvalidationLevel invalidationLevel,
                         PropertySemantics::Type semantics)
    : OrdinalProperty<int>(identifier, displayName, value, minValue, maxValue, increment,
                           invalidationLevel, semantics)
{}

int IntProperty::getVariantType() {
    return Variant::VariantTypeInteger;
}

Variant IntProperty::getVariant() {
    return Variant(get());
}

void  IntProperty::setVariant(const Variant& val) {
    if (val.canConvert(getVariantType())) {
        set(val.getInt());
    }
}

void IntProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("value", get());
    s.serialize("minvalue", OrdinalProperty<int>::getMinValue());
    s.serialize("maxvalue", OrdinalProperty<int>::getMaxValue());
    s.serialize("increment", OrdinalProperty<int>::getIncrement());
}

void IntProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    int value;
    d.deserialize("value", value);
    set(value);

    d.deserialize("minvalue",  value);
    OrdinalProperty<int>::setMinValue(value);

    d.deserialize("maxvalue", value);
    OrdinalProperty<int>::setMaxValue(value);

    d.deserialize("increment", value);
    OrdinalProperty<int>::setIncrement(value);
}

} // namespace
