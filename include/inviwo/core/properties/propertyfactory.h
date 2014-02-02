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

#ifndef IVW_PROPERTYFACTORY_H
#define IVW_PROPERTYFACTORY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/property.h>
#include <inviwo/core/util/factory.h>
#include <inviwo/core/util/singleton.h>
#include <inviwo/core/properties/propertyfactoryobject.h>

namespace inviwo {

class IVW_CORE_API PropertyFactory : public Factory,
    public Singleton<PropertyFactory> {

public:
    PropertyFactory();
    ~PropertyFactory();

    void registeryObject(PropertyFactoryObject* property);

    virtual IvwSerializable* create(std::string className) const;

    virtual Property* getProperty(std::string className,
                                  std::string identifier,
                                  std::string displayName);

    virtual bool isValidType(std::string className) const;

    std::vector<std::string> getRegistedPropertyClassNames();

    typedef std::map<std::string, PropertyFactoryObject*> PropertyClassMap;

private:
    mutable PropertyClassMap propertyClassMap_;
};

} // namespace

#endif // IVW_PROPERTYFACTORY_H
