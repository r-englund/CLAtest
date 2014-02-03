 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file authors: Timo Ropinski, Hans-Christian Helltegen
 *
 *********************************************************************************/

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
    virtual void invalidate(PropertyOwner::InvalidationLevel invalidationLevel, Property* modifiedProperty=0);

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
