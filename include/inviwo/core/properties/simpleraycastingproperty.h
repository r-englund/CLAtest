#ifndef IVW_SIMPLERAYCASTINGPROPERTY_H
#define IVW_SIMPLERAYCASTINGPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/properties/baseoptionproperty.h>

namespace inviwo {

class IVW_CORE_API SimpleRaycastingProperty : public CompositeProperty { 
public:
    SimpleRaycastingProperty(std::string identifier, std::string displayName,
                             PropertyOwner::InvalidationLevel = PropertyOwner::INVALID_OUTPUT,
                             PropertySemantics semantics = PropertySemantics::Default);
    virtual ~SimpleRaycastingProperty(){}

    virtual void invalidate(PropertyOwner::InvalidationLevel invalidationLevel,
                            Property* modifiedProperty = 0);

    OptionPropertyString classificationMode_;
    OptionPropertyString compositingMode_;
    OptionPropertyString gradientComputationMode_;

    FloatProperty samplingRate_;
    FloatProperty isoValue_;
};

} // namespace

#endif // IVW_SIMPLERAYCASTINGPROPERTY_H

