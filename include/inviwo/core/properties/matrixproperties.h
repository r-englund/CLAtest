#ifndef  IVW_MATRIXPROPERTIES_H
#define IVW_MATRIXPROPERTIES_H

#include "inviwo/core/common/inviwocoredefine.h"
#include "inviwo/core/properties/ordinalproperty.h"

namespace inviwo {

class IVW_CORE_API FloatMat2Property : public OrdinalProperty<mat2> {
public:
	FloatMat2Property(std::string identifier, std::string displayName,mat2 value,
                      mat2 minValue=mat2(0.0f), mat2 maxValue= mat2(1.0f), mat2 increment=mat2(0.1f),
                      PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                      PropertySemantics::Type semantics = PropertySemantics::Default);
	virtual void serialize(IvwSerializer& s) const;
	virtual void deserialize(IvwDeserializer& s);
	};

class IVW_CORE_API FloatMat3Property : public OrdinalProperty<mat3> {
public:
    FloatMat3Property(std::string identifier, std::string displayName,mat3 value,
                      mat3 minValue=mat3(0.0f), mat3 maxValue= mat3(1.0f), mat3 increment=mat3(0.1f),
                      PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                      PropertySemantics::Type semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& s);
};

class IVW_CORE_API FloatMat4Property : public OrdinalProperty<mat4> {
public:
    FloatMat4Property(std::string identifier, std::string displayName, mat4 value,
                      mat4 minValue=mat4(0.0f), mat4 maxValue= mat4(1.0f), mat4 increment=mat4(0.1f),
                      PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                      PropertySemantics::Type semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& s);
};
 
}//namespace

#endif