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

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/common/inviwomodule.h>
#include <inviwo/core/properties/propertyfactory.h>


namespace inviwo {

PropertyFactory::PropertyFactory() {}

PropertyFactory::~PropertyFactory() {}


void PropertyFactory::registeryObject(PropertyFactoryObject* property){
    std::string className = property->getClassName();

    PropertyClassMap::const_iterator it = propertyClassMap_.find(className);
    if(it == propertyClassMap_.end()) {
        propertyClassMap_.insert(std::make_pair(className, property));

    } else {
        LogWarn("Property with class name: " << className << " already registed");
    }
}

IvwSerializable* PropertyFactory::create(std::string className) const {
    return NULL;
}

bool PropertyFactory::isValidType(std::string className) const {
    PropertyClassMap::const_iterator it = propertyClassMap_.find(className);
    if(it != propertyClassMap_.end()) {
        return true;
    } else {
        return false;
    }
}

Property* PropertyFactory::getProperty(std::string className, std::string identifier, std::string displayName) {
    PropertyClassMap::const_iterator it = propertyClassMap_.find(className);
    if(it != propertyClassMap_.end()) {
        return it->second->create(identifier, displayName);
    } else {
        return NULL;
    }
}

std::vector<std::string> PropertyFactory::getRegistedPropertyClassNames() {
    std::vector<std::string> classNames;
    for(PropertyClassMap::iterator it = propertyClassMap_.begin(); it != propertyClassMap_.end(); ++it) {
        classNames.push_back(it->first);
    }
    return classNames;
}

} // namespace
