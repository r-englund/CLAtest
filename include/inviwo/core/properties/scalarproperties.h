#ifndef IVW_SCALARPROPERTIES_H
#define IVW_SCALARPROPERTIES_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/properties/ordinalproperty.h>

namespace inviwo {

class IVW_CORE_API FloatProperty : public OrdinalProperty<float> {
public:
    FloatProperty(std::string identifier, std::string displayName, float value,
                  float minValue=0.0f, float maxValue=1.0f, float increment=0.1f,
                  PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                  PropertySemantics::Type semantics=PropertySemantics::Default);
    virtual Variant getVariant();
    virtual void setVariant(const Variant&);
    virtual int getVariantType();
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& s);
};

class IVW_CORE_API IntProperty : public OrdinalProperty<int> {
public:
    IntProperty(std::string identifier, std::string displayName, int value,
                int minValue=0, int maxValue=4096, int increment=1,
                PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                PropertySemantics::Type semantics=PropertySemantics::Default);
    virtual Variant getVariant();
    virtual void setVariant(const Variant&);
    virtual int getVariantType();
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& s);
};

} // namespace

#endif // IVW_SCALARPROPERTIES_H