/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

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
    // whereby higher numbers (later entry in the list) invole more
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
	template<class T> std::vector<T*> getPropertiesByType() const;

    bool isValid() { return (invalidationLevel_ == PropertyOwner::VALID); }
    virtual void setValid();
    InvalidationLevel getInvalidationLevel() { return invalidationLevel_; }
    virtual void invalidate(PropertyOwner::InvalidationLevel invalidationLevel);
       
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

protected:
    std::vector<Property*> properties_;

private:    
    InvalidationLevel invalidationLevel_;
};

template<class T>
std::vector<T*> PropertyOwner::getPropertiesByType() const {
	std::vector<T*> foundProperties;
	for (size_t i=0; i<properties_.size(); i++) {
		T* property_ = dynamic_cast<T*>(properties_[i]);
		if (property_) foundProperties.push_back(property_);
	}
	return foundProperties;
}

} // namespace

#endif // IVW_PROPERTYOWNER_H
