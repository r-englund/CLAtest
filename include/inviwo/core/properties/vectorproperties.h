#ifndef IVW_VECTORPROPERTIES_H
#define IVW_VECTORPROPERTIES_H

#include "inviwo/core/inviwocoredefine.h"
#include "inviwo/core/properties/ordinalproperty.h"

namespace inviwo {

class IVW_CORE_API FloatVec3Property : public OrdinalProperty<vec3> {
public:
    FloatVec3Property(std::string identifier, std::string displayName, vec3 value, vec3 minValue=vec3(0.0f), vec3 maxValue=vec3(1.0f), vec3 increment=vec3(0.1f));
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& s);
};

class IntVec2Property : public OrdinalProperty<vec2> {
public:
	IntVec2Property(std::string identifier, std::string displayName, vec2 value, vec2 minValue=vec2(0), vec2 maxValue= vec2(10), vec2 increment=vec2(1));
	virtual void serialize(IvwSerializer& s) const;
	virtual void deserialize(IvwDeserializer& s);

};

} // namespace

#endif // IVW_VECTORPROPERTIES_H