#include "inviwo/core/properties/scalarproperties.h"

namespace inviwo {

FloatProperty::FloatProperty(std::string identifier, std::string displayName, float value,
    float minValue, float maxValue, float increment)
    : OrdinalProperty(identifier, displayName, value, minValue, maxValue, increment)
{}

void FloatProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("f_value", get());
    s.serialize("f_minvalue", OrdinalProperty<float>::getMinValue());
    s.serialize("f_maxvalue", OrdinalProperty<float>::getMaxValue());
    s.serialize("f_incrementvalue", OrdinalProperty<float>::getIncrement());
}

void FloatProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    float value;
    d.deserialize("f_value", value);
    set(value);

    d.deserialize("f_minvalue",  value);
    OrdinalProperty<float>::setMinValue(value);

    d.deserialize("f_maxvalue", value);
    OrdinalProperty<float>::setMaxValue(value);

    d.deserialize("f_incrementvalue", value);
    OrdinalProperty<float>::setIncrement(value);
}

IntProperty::IntProperty(std::string identifier, std::string displayName, int value,
    int minValue, int maxValue, int increment)
    : OrdinalProperty(identifier, displayName, value, minValue, maxValue, increment)
{}

void IntProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("i_value", get());
    s.serialize("i_minvalue", OrdinalProperty<int>::getMinValue());
    s.serialize("i_maxvalue", OrdinalProperty<int>::getMaxValue());
    s.serialize("i_incrementvalue", OrdinalProperty<int>::getIncrement());
}

void IntProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    int value;
    d.deserialize("i_value", value);
    set(value);

    d.deserialize("i_minvalue",  value);
    OrdinalProperty<int>::setMinValue(value);

    d.deserialize("i_maxvalue", value);
    OrdinalProperty<int>::setMaxValue(value);

    d.deserialize("i_incrementvalue", value);
    OrdinalProperty<int>::setIncrement(value);
}

} // namespace
