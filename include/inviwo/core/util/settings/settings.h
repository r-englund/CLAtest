/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_SETTINGS_H
#define IVW_SETTINGS_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwomodule.h>
#include <inviwo/core/properties/propertyowner.h>
#include <inviwo/core/properties/property.h>

namespace inviwo {

class IVW_CORE_API Settings : public PropertyOwner {

public:
    Settings(std::string id);
    virtual ~Settings();

    virtual void initialize()=0;
    virtual void deinitialize()=0;

    virtual void invalidate(PropertyOwner::InvalidationLevel invalidationLevel,
                            Property* modifiedProperty=0);
    virtual void invalidate();
    virtual bool isValid() { return PropertyOwner::isValid(); }
    virtual void setValid() { PropertyOwner::setValid(); }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

    virtual std::string getIdentifier();
protected:
    std::string identifier_;
};

} // namespace

#endif // IVW_SETTINGS_H
