#ifndef IVW_PROPERTYOWNER_H
#define IVW_PROPERTYOWNER_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>

namespace inviwo {

class Property;

class IVW_CORE_API PropertyOwner : public IvwSerializable {

public:
    PropertyOwner();
    virtual ~PropertyOwner();

    enum InvalidationLevel {
        VALID,
        INVALID_OUTPUT,
        INVALID_RESOURCES
    };

    void addProperty(Property* property);
    void addProperty(Property& property);

    Property* getPropertyByIdentifier(std::string identifier);
    std::vector<Property*> getProperties() { return properties_; }

    virtual void invalidate();
    virtual void setValid();
    virtual bool isValid();
       
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

private:
    std::vector<Property*> properties_;
    bool invalid_;

};

} // namespace

#endif // IVW_PROPERTYOWNER_H
