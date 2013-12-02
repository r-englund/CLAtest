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

#ifndef IVW_SCALARPROPERTIES_H
#define IVW_SCALARPROPERTIES_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/ordinalproperty.h>

namespace inviwo {

class IVW_CORE_API FloatProperty : public OrdinalProperty<float> {
public:
    FloatProperty(std::string identifier, std::string displayName, float value,
                  float minValue=0.0f, float maxValue=1.0f, float increment=0.01f,
                  PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                  PropertySemantics::Type semantics=PropertySemantics::Default);
    virtual std::string getClassName()  const { return "FloatProperty"; }
    virtual Variant getVariant();
    virtual void setVariant(const Variant&);
    virtual int getVariantType();
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

class IVW_CORE_API IntProperty : public OrdinalProperty<int> {
public:
    IntProperty(std::string identifier, std::string displayName, int value,
                int minValue=0, int maxValue=4096, int increment=1,
                PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                PropertySemantics::Type semantics=PropertySemantics::Default);
    virtual std::string getClassName()  const { return "IntProperty"; }
    virtual Variant getVariant();
    virtual void setVariant(const Variant&);
    virtual int getVariantType();
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

} // namespace

#endif // IVW_SCALARPROPERTIES_H