#ifndef IVW_PROPERTYOWNER_H
#define IVW_PROPERTYOWNER_H

#include "inviwo/core/inviwo.h"

namespace inviwo {

class Property;

class PropertyOwner {

public:
    PropertyOwner();
    virtual ~PropertyOwner();

    void addProperty(Property* property);
    void addProperty(Property& property);

    Property* getPropertyByIdentifier(std::string identifier);

    virtual void invalidate();

private:
    std::vector<Property*> properties_;

};

} // namespace

#endif // IVW_PROPERTYOWNER_H
