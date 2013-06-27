#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

FloatMinMaxProperty::FloatMinMaxProperty(std::string identifier, std::string displayName, 
                                        float valueMin, float valueMax, float rangeMin, float rangeMax, float increment,
                                        PropertyOwner::InvalidationLevel invalidationLevel,
                                        PropertySemantics::Type semantics )
    : MinMaxProperty<glm::mediump_float>(identifier, displayName, valueMin, valueMax, rangeMin, rangeMax, increment, invalidationLevel, semantics)
{}

FloatVec2Property::FloatVec2Property(std::string identifier, std::string displayName, vec2 value,
                                    vec2 minValue, vec2 maxValue, vec2 increment, PropertyOwner::InvalidationLevel invalidationLevel,
                                    PropertySemantics::Type semantics )
	: OrdinalProperty<vec2>(identifier, displayName, value, minValue, maxValue, increment, invalidationLevel, semantics)
{}

FloatVec3Property::FloatVec3Property(std::string identifier, std::string displayName, vec3 value,
                                     vec3 minValue, vec3 maxValue, vec3 increment,
                                     PropertyOwner::InvalidationLevel invalidationLevel,
                                     PropertySemantics::Type semantics )
    : OrdinalProperty<vec3>(identifier, displayName, value, minValue, maxValue, increment, invalidationLevel, semantics)
{}

FloatVec4Property::FloatVec4Property(std::string identifier, std::string displayName, vec4 value,
                                     vec4 minValue, vec4 maxValue, vec4 increment,
                                     PropertyOwner::InvalidationLevel invalidationLevel,
                                     PropertySemantics::Type semantics )
    : OrdinalProperty<vec4>(identifier, displayName, value, minValue, maxValue, increment, invalidationLevel, semantics)
{}

IntMinMaxProperty::IntMinMaxProperty(std::string identifier, std::string displayName, 
                                     int valueMin, int valueMax, int rangeMin, int rangeMax, int increment,
                                     PropertyOwner::InvalidationLevel invalidationLevel,
                                     PropertySemantics::Type semantics )
    : MinMaxProperty<int>(identifier, displayName, valueMin, valueMax, rangeMin, rangeMax, increment, invalidationLevel, semantics)
{}

IntVec2Property::IntVec2Property(std::string identifier, std::string displayName, ivec2 value,
								 ivec2 minValue, ivec2 maxValue, ivec2 increment,
                                 PropertyOwner::InvalidationLevel invalidationLevel,
                                 PropertySemantics::Type semantics )
	: OrdinalProperty<ivec2>(identifier, displayName, value, minValue, maxValue, increment, invalidationLevel, semantics)
{}

IntVec3Property::IntVec3Property(std::string identifier, std::string displayName, ivec3 value,
								 ivec3 minValue, ivec3 maxValue, ivec3 increment,
                                 PropertyOwner::InvalidationLevel invalidationLevel,
                                 PropertySemantics::Type semantics )
	: OrdinalProperty<ivec3>(identifier, displayName, value, minValue, maxValue, increment, invalidationLevel, semantics)
{}

IntVec4Property::IntVec4Property(std::string identifier, std::string displayName, ivec4 value,
								 ivec4 minValue, ivec4 maxValue, ivec4 increment,
                                 PropertyOwner::InvalidationLevel invalidationLevel,
                                 PropertySemantics::Type semantics )
	: OrdinalProperty<ivec4>(identifier, displayName, value, minValue, maxValue, increment, invalidationLevel, semantics)
{}

void FloatMinMaxProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s);
    s.serialize("value", get());
    s.serialize("range", MinMaxProperty<glm::mediump_float>::getRange());
    s.serialize("increment", MinMaxProperty<glm::mediump_float>::getIncrement());
}

void FloatMinMaxProperty::deserialize(IvwDeserializer& d){
    Property::deserialize(d);

    vec2 value;
    d.deserialize("range", value);
    MinMaxProperty<glm::mediump_float>::setRange(value);

    float increment;
    d.deserialize("increment", increment);
    MinMaxProperty<glm::mediump_float>::setIncrement(increment);

    d.deserialize("value",value);
    set(value);
}

void FloatVec2Property::serialize(IvwSerializer& s) const {
	Property::serialize(s);
	s.serialize("value", get());
	s.serialize("minvalue", OrdinalProperty<vec2>::getMinValue());
	s.serialize("maxvalue", OrdinalProperty<vec2>::getMaxValue());
	s.serialize("increment", OrdinalProperty<vec2>::getIncrement());
}

void FloatVec2Property::deserialize(IvwDeserializer& d){
	Property::deserialize(d);
	vec2 value;
	d.deserialize("value",value);
	set(value);

	d.deserialize("minvalue", value);
	OrdinalProperty<vec2>::setMinValue(value);

	d.deserialize("maxvalue",value);
	OrdinalProperty<vec2>::setMaxValue(value);

	d.deserialize("increment", value);
	OrdinalProperty<vec2>::setIncrement(value);
}

void FloatVec3Property::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("value", get());
    s.serialize("minvalue", OrdinalProperty<vec3>::getMinValue());
    s.serialize("maxvalue", OrdinalProperty<vec3>::getMaxValue());
    s.serialize("increment", OrdinalProperty<vec3>::getIncrement());
}

void FloatVec3Property::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    vec3 value;
    d.deserialize("value", value);
    set(value);

    d.deserialize("minvalue",  value);
    OrdinalProperty<vec3>::setMinValue(value);

    d.deserialize("maxvalue", value);
    OrdinalProperty<vec3>::setMaxValue(value);

    d.deserialize("increment", value);
    OrdinalProperty<vec3>::setIncrement(value);
}

void FloatVec4Property::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("value", get());
    s.serialize("minvalue", OrdinalProperty<vec4>::getMinValue());
    s.serialize("maxvalue", OrdinalProperty<vec4>::getMaxValue());
    s.serialize("increment", OrdinalProperty<vec4>::getIncrement());
}

void FloatVec4Property::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    vec4 value;
    d.deserialize("value", value);
    set(value);

    d.deserialize("minvalue",  value);
    OrdinalProperty<vec4>::setMinValue(value);

    d.deserialize("maxvalue", value);
    OrdinalProperty<vec4>::setMaxValue(value);

    d.deserialize("increment", value);
    OrdinalProperty<vec4>::setIncrement(value);
}

void IntMinMaxProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s);
    s.serialize("value", get());
    s.serialize("range", MinMaxProperty<int>::getRange());
    s.serialize("increment", MinMaxProperty<int>::getIncrement());
}

void IntMinMaxProperty::deserialize(IvwDeserializer& d){
    Property::deserialize(d);

    glm::detail::tvec2<int> value;
    d.deserialize("range", value);
    MinMaxProperty<int>::setRange(value);

    int increment;
    d.deserialize("increment", increment);
    MinMaxProperty<int>::setIncrement(increment);

    d.deserialize("value",value);
    set(value);
}

void IntVec2Property::serialize(IvwSerializer& s) const {
	Property::serialize(s);
	s.serialize("value", get());
	s.serialize("minvalue", OrdinalProperty<ivec2>::getMinValue());
	s.serialize("maxvalue", OrdinalProperty<ivec2>::getMaxValue());
	s.serialize("increment", OrdinalProperty<ivec2>::getIncrement());
}

void IntVec2Property::deserialize(IvwDeserializer& d){
	Property::deserialize(d);
	ivec2 value;
	d.deserialize("value",value);
	set(value);

	d.deserialize("minvalue", value);
	OrdinalProperty<ivec2>::setMinValue(value);

	d.deserialize("maxvalue",value);
	OrdinalProperty<ivec2>::setMaxValue(value);

	d.deserialize("increment", value);
	OrdinalProperty<ivec2>::setIncrement(value);
}

void IntVec3Property::serialize(IvwSerializer& s) const {
	Property::serialize(s);
	s.serialize("value", get());
	s.serialize("minvalue", OrdinalProperty<ivec3>::getMinValue());
	s.serialize("maxvalue", OrdinalProperty<ivec3>::getMaxValue());
	s.serialize("increment", OrdinalProperty<ivec3>::getIncrement());
}

void IntVec3Property::deserialize(IvwDeserializer& d){
	Property::deserialize(d);
	ivec3 value;
	d.deserialize("value",value);
	set(value);

	d.deserialize("minvalue", value);
	OrdinalProperty<ivec3>::setMinValue(value);

	d.deserialize("maxvalue",value);
	OrdinalProperty<ivec3>::setMaxValue(value);

	d.deserialize("increment", value);
	OrdinalProperty<ivec3>::setIncrement(value);
}


void IntVec4Property::serialize(IvwSerializer& s) const {
	Property::serialize(s);
	s.serialize("value", get());
	s.serialize("minvalue", OrdinalProperty<ivec4>::getMinValue());
	s.serialize("maxvalue", OrdinalProperty<ivec4>::getMaxValue());
	s.serialize("increment", OrdinalProperty<ivec4>::getIncrement());
}

void IntVec4Property::deserialize(IvwDeserializer& d){
	Property::deserialize(d);
	ivec4 value;
	d.deserialize("value",value);
	set(value);

	d.deserialize("minvalue", value);
	OrdinalProperty<ivec4>::setMinValue(value);

	d.deserialize("maxvalue",value);
	OrdinalProperty<ivec4>::setMaxValue(value);

	d.deserialize("increment", value);
	OrdinalProperty<ivec4>::setIncrement(value);
}

} // namespace
