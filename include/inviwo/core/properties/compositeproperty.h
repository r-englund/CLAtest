#ifndef IVW_COMPOSITEPROPERTY_H
#define IVW_COMPOSITEPROPERTY_H

#include "inviwo/core/inviwocoredefine.h"
#include "inviwo/core/inviwo.h"
#include "inviwo/core/properties/property.h"

namespace inviwo {

class IVW_CORE_API CompositeProperty : public Property {

public:
    CompositeProperty(std::string identifier, std::string displayName);
    virtual ~CompositeProperty();

    void addProperty(Property* property);
    void addProperty(Property& property);
    std::vector<Property*> getSubProperties() { return subProperties_; }

    void setOwner(PropertyOwner* owner);

private:
    std::vector<Property*> subProperties_;
    
};

} // namespace

#endif // IVW_COMPOSITEPROPERTY_H