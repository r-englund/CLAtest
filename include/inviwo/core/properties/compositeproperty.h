#ifndef IVW_COMPOSITEPROPERTY_H
#define IVW_COMPOSITEPROPERTY_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/properties/property.h"

namespace inviwo {

class CompositeProperty : public Property {

public:
    CompositeProperty(std::string identifier, std::string displayName);
    virtual ~CompositeProperty();

    void addProperty(Property* property);
    void addProperty(Property& property);

    void setOwner(PropertyOwner* owner);

private:
    std::vector<Property*> properties_;
    
};

} // namespace

#endif // IVW_COMPOSITEPROPERTY_H