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

#include <inviwo/core/properties/property.h>
#include <inviwo/core/properties/propertyowner.h>
#include <inviwo/core/io/serialization/ivwserializable.h>

namespace inviwo {

PropertyOwner::PropertyOwner()
    : invalidationLevel_(PropertyOwner::VALID) {}

PropertyOwner::~PropertyOwner() {
    properties_.clear();
}

void PropertyOwner::addProperty(Property* property) {
    ivwAssert(getPropertyByIdentifier(property->getIdentifier())==0, "Property already exist");
    properties_.push_back(property);
    property->setOwner(this);
}

void PropertyOwner::addProperty(Property& property) {
    addProperty(&property);
}

Property* PropertyOwner::getPropertyByIdentifier(std::string identifier) {
    for (size_t i=0; i<properties_.size(); i++)
        if ((properties_[i]->getIdentifier()).compare(identifier) == 0)
            return properties_[i];
    return NULL;
}

void PropertyOwner::setValid() { 
    for (size_t i=0; i<properties_.size(); i++)
       properties_[i]->setPropertyModified(false);
    invalidationLevel_ = PropertyOwner::VALID; 
}

void PropertyOwner::invalidate(PropertyOwner::InvalidationLevel invalidationLevel, Property* modifiedProperty) {
    IVW_UNUSED_PARAM(modifiedProperty);
    invalidationLevel_ = std::max(invalidationLevel_, invalidationLevel);
}

void PropertyOwner::serialize(IvwSerializer& s) const {

    std::map<std::string, Property*> propertyMap;
    for (std::vector<Property*>::const_iterator it = properties_.begin(); it != properties_.end(); ++it) {
        propertyMap[(*it)->getIdentifier()] = *it; 
    }
    s.serialize("Properties", propertyMap, "Property" );
}

void PropertyOwner::deserialize(IvwDeserializer& d) {
    /* 1) Vector deserialization does not allow
    *     specification of comparison attribute string.
    *  2) But Map deserialization does allow 
    *     specification of comparision attribute string.
    *     (eg. "identifier" in this case).
    *  3) Hence map deserialization is preferred here.
    *  4) TODO: Vector can be made to behave like Map.
    *           But then it necessitates passing of two extra arguments.
    *           And they are list of attribute values, comparison attribute string.
    *           eg., list of identifier for each property and "identifier"
    *                                                 
    */

    std::map<std::string, Property*> propertyMap;
    for (std::vector<Property*>::const_iterator it = properties_.begin(); it != properties_.end(); ++it) {
        propertyMap[(*it)->getIdentifier()] = *it; 
    }
    d.deserialize("Properties", propertyMap, "Property", "identifier") ;
}


} // namespace
