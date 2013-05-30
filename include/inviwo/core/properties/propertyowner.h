#ifndef IVW_PROPERTYOWNER_H
#define IVW_PROPERTYOWNER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class Property;

class IVW_CORE_API PropertyOwner : public IvwSerializable {

public:
    PropertyOwner();
    virtual ~PropertyOwner();

    // invalidation level must be sorted based on their complexity,
    // whereby higher numbers (later entry in teh list) invole more
    // expensive update operations
    enum InvalidationLevel {
        VALID,
        INVALID_OUTPUT,
        INVALID_RESOURCES,

        INVALID_PROGRAM=INVALID_RESOURCES, // deprecated
        INVALID_RENDERING=INVALID_OUTPUT // deprecated
    };

    void addProperty(Property* property);
    void addProperty(Property& property);

    Property* getPropertyByIdentifier(std::string identifier);
    std::vector<Property*> getProperties() { return properties_; }

    bool isValid() { return (invalidationLevel_ == PropertyOwner::VALID); }
    void setValid() { invalidationLevel_ = PropertyOwner::VALID; }
    InvalidationLevel getInvalidationLevel() { return invalidationLevel_; }
    virtual void invalidate(PropertyOwner::InvalidationLevel invalidationLevel);
       
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

protected:
    std::vector<Property*> properties_;

private:    
    InvalidationLevel invalidationLevel_;
};

} // namespace

#endif // IVW_PROPERTYOWNER_H
