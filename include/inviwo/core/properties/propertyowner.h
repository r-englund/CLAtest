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
    std::vector<Property*> getProperties() { return properties_; }

    virtual void invalidate();
    virtual void setValid();
    virtual bool isValid();

private:
    std::vector<Property*> properties_;
    bool invalid_;

};

} // namespace

#endif // IVW_PROPERTYOWNER_H
