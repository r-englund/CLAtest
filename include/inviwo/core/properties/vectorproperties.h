#ifndef IVW_VECTORPROPERTIES_H
#define IVW_VECTORPROPERTIES_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/properties/ordinalproperty.h>

namespace inviwo {

class IVW_CORE_API FloatVec2Property : public OrdinalProperty<vec2> {
public:	
    FloatVec2Property(std::string identifier, std::string displayName, vec2 value, 
		vec2 minValue=vec2(0.0f), vec2 maxValue= vec2(1.0f), vec2 increment=vec2(0.1f),
        PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
        PropertySemantics::Type semantics = PropertySemantics::Default);
	virtual void serialize(IvwSerializer& s) const;
	virtual void deserialize(IvwDeserializer& s);

};

class IVW_CORE_API FloatVec3Property : public OrdinalProperty<vec3> {
public:
    FloatVec3Property(std::string identifier, std::string displayName, vec3 value, 
		vec3 minValue=vec3(0.0f), vec3 maxValue=vec3(1.0f), vec3 increment=vec3(0.1f),
        PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
        PropertySemantics::Type semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& s);
};

class IVW_CORE_API FloatVec4Property : public OrdinalProperty<vec4> {
public:	
	FloatVec4Property(std::string identifier, std::string displayName, vec4 value, 
		vec4 minValue=vec4(0.0f), vec4 maxValue= vec4(1.0f), vec4 increment=vec4(0.1f),
        PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
        PropertySemantics::Type semantics = PropertySemantics::Default);
	virtual void serialize(IvwSerializer& s) const;
	virtual void deserialize(IvwDeserializer& s);

};

class IVW_CORE_API IntVec2Property : public OrdinalProperty<ivec2> {
public:
	IntVec2Property(std::string identifier, std::string displayName, ivec2 value, 
		ivec2 minValue=ivec2(0), ivec2 maxValue= ivec2(10), ivec2 increment=ivec2(1),
        PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
        PropertySemantics::Type semantics = PropertySemantics::Default);
	virtual void serialize(IvwSerializer& s) const;
	virtual void deserialize(IvwDeserializer& s);

};

class IVW_CORE_API IntVec3Property : public OrdinalProperty<ivec3> {
public:
	IntVec3Property(std::string identifier, std::string displayName, ivec3 value, 
		ivec3 minValue=ivec3(0), ivec3 maxValue= ivec3(10), ivec3 increment=ivec3(1),
        PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
        PropertySemantics::Type semantics = PropertySemantics::Default);
	virtual void serialize(IvwSerializer& s) const;
	virtual void deserialize(IvwDeserializer& s);

};

class IVW_CORE_API IntVec4Property : public OrdinalProperty<ivec4> {
public:
	IntVec4Property(std::string identifier, std::string displayName, ivec4 value, 
		ivec4 minValue=ivec4(0), ivec4 maxValue= ivec4(10), ivec4 increment=ivec4(1),
        PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
        PropertySemantics::Type semantics = PropertySemantics::Default);
	virtual void serialize(IvwSerializer& s) const;
	virtual void deserialize(IvwDeserializer& s);

};



} // namespace

#endif // IVW_VECTORPROPERTIES_H