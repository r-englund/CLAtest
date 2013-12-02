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

#ifndef IVW_BOOLPROPERTY_H
#define IVW_BOOLPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/templateproperty.h>

namespace inviwo {
    /** class BoolProperty
    *\brief The BoolProperty class provides a property holding boolean values
    * 
    * A bool property are normally used by a processor.
    * The bool property is created and added in the constructor of the processor.
    * The propertys widget consist of a checkbox that sets the value of the property.
    * @see TemplateProperty
    * @see BoolPropertyWidgetQt
    */

class IVW_CORE_API BoolProperty : public TemplateProperty<bool> {

public:
    /** 
     * \brief Constructs a bool property
     *
     * <DESCRIBE THE METHOD>
     * 
     * @param std::string identifier <DESCRIBE ME>
     * @param std::string displayName <DESCRIBE ME>
     * @param bool value <DESCRIBE ME>
     * @param PropertyOwner::InvalidationLevel invalidationLevel <DESCRIBE ME>
     * @param PropertySemantics::Type semantics <DESCRIBE ME>
     * @return  <DESCRIBE ME>
     */BoolProperty(std::string identifier, std::string displayName, bool value,
                 PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                 PropertySemantics::Type semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName()  const { return "BoolProperty"; }
    virtual Variant getVariant();
    virtual void setVariant(const Variant&);
    virtual int getVariantType();
};

} // namespace

#endif // IVW_BOOLPROPERTY_H