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

#ifndef IVW_PROCESSORFACTORY_H
#define IVW_PROCESSORFACTORY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/property.h>
#include <inviwo/core/util/inviwofactorybase.h>
#include <inviwo/core/util/singleton.h>

namespace inviwo {

class IVW_CORE_API PropertyFactory : public Factory,
                                     public Singleton<PropertyFactory> {

public:
    PropertyFactory();
    ~PropertyFactory();

    virtual void initialize();
    virtual void deinitialize();

    void registerFactoryObject(Property* property);
    virtual IvwSerializable* create(std::string className) const;

private:
    mutable std::map<std::string, Property*> processorClassMap_;
};

} // namespace

#endif // IVW_PROCESSORFACTORY_H
