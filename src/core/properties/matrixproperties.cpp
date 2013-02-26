#include "inviwo/core/properties/matrixproperties.h"

namespace inviwo {

FloatMat2Property::FloatMat2Property(std::string identifier, std::string displayName, mat2 value,
                                     mat2 minValue, mat2 maxValue, mat2 increment, PropertySemantics::Type semantics /*= PropertySemantics::Default*/)
	: OrdinalProperty<mat2>(identifier,displayName,value, minValue, maxValue, increment,semantics) 
{}

void FloatMat2Property::serialize(IvwSerializer& s) const {
	Property::serialize(s);
	s.serialize("value",get());
    s.serialize("minvalue", OrdinalProperty<mat2>::getMinValue());
    s.serialize("maxvalue", OrdinalProperty<mat2>::getMaxValue());
    s.serialize("increment", OrdinalProperty<mat2>::getIncrement());
}

void FloatMat2Property::deserialize(IvwDeserializer& d) {
    Property::deserialize(d);
    mat2 value;
    d.deserialize("value",value);
    set(value);

    d.deserialize("minvalue", value);
    OrdinalProperty<mat2>::setMinValue(value);

    d.deserialize("maxvalue",value);
    OrdinalProperty<mat2>::setMaxValue(value);

    d.deserialize("increment", value);
    OrdinalProperty<mat2>::setIncrement(value);
}

FloatMat3Property::FloatMat3Property(std::string identifier, std::string displayName, mat3 value,
                                     mat3 minValue, mat3 maxValue, mat3 increment, PropertySemantics::Type semantics/*= PropertySemantics::Default*/)
    : OrdinalProperty<mat3>(identifier,displayName,value, minValue, maxValue, increment, semantics) 
{}

void FloatMat3Property::serialize(IvwSerializer& s) const {
    Property::serialize(s);
    s.serialize("value",get());
    s.serialize("minvalue", OrdinalProperty<mat3>::getMinValue());
    s.serialize("maxvalue", OrdinalProperty<mat3>::getMaxValue());
    s.serialize("increment", OrdinalProperty<mat3>::getIncrement());
}

void FloatMat3Property::deserialize(IvwDeserializer& d) {
    Property::deserialize(d);
    mat3 value;
    d.deserialize("value",value);
    set(value);

    d.deserialize("minvalue", value);
    OrdinalProperty<mat3>::setMinValue(value);

    d.deserialize("maxvalue",value);
    OrdinalProperty<mat3>::setMaxValue(value);

    d.deserialize("increment", value);
    OrdinalProperty<mat3>::setIncrement(value);
}

FloatMat4Property::FloatMat4Property(std::string identifier, std::string displayName, mat4 value,
    mat4 minValue, mat4 maxValue, mat4 increment, PropertySemantics::Type semantics/*= PropertySemantics::Default*/)
    : OrdinalProperty<mat4>(identifier,displayName,value, minValue, maxValue, increment, semantics) 
{}

void FloatMat4Property::serialize(IvwSerializer& s) const {
    Property::serialize(s);
    s.serialize("value",get());
    s.serialize("minvalue", OrdinalProperty<mat4>::getMinValue());
    s.serialize("maxvalue", OrdinalProperty<mat4>::getMaxValue());
    s.serialize("increment", OrdinalProperty<mat4>::getIncrement());
}

void FloatMat4Property::deserialize(IvwDeserializer& d) {
    Property::deserialize(d);
    mat4 value;
    d.deserialize("value",value);
    set(value);

    d.deserialize("minvalue", value);
    OrdinalProperty<mat4>::setMinValue(value);

    d.deserialize("maxvalue",value);
    OrdinalProperty<mat4>::setMaxValue(value);

    d.deserialize("increment", value);
    OrdinalProperty<mat4>::setIncrement(value);
}

}//namespace