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

#ifndef IVW_COMPOSITEPROPERTY_H
#define IVW_COMPOSITEPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/property.h>
#include <inviwo/core/properties/propertyowner.h>
#include <inviwo/core/util/observer.h>

namespace inviwo {

class IVW_CORE_API CompositeProperty : public Property, public PropertyOwner {

public:
    CompositeProperty(std::string identifier, std::string displayName,
                      PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                      PropertySemantics semantics = PropertySemantics::Default);
    virtual ~CompositeProperty();

    void addProperty(Property* property);
    void addProperty(Property& property);
    std::vector<Property*> getSubProperties() const { return subProperties_; }
    void setOwner(PropertyOwner* owner);
    void notify();
    virtual std::string getClassName()  const { return "CompositeProperty"; }
    void updateVisibility();
    void setVisible(bool val);
    void setReadOnly(bool value);

    //override original function in property
    virtual void set(const Property* src);
    virtual void setPropertyModified(bool modified);
    virtual bool isPropertyModified() const;

private:
    std::vector<Property*> subProperties_;
    
};

} // namespace

#endif // IVW_COMPOSITEPROPERTY_H