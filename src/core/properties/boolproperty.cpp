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

#include <inviwo/core/properties/boolproperty.h>

namespace inviwo {

    BoolProperty::BoolProperty(std::string identifier, std::string displayName, bool value,
                               PropertyOwner::InvalidationLevel invalidationLevel, 
                               PropertySemantics::Type semantics)
    : TemplateProperty<bool>(identifier, displayName, value, invalidationLevel, semantics)
{}

void BoolProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("value", get());
}

void BoolProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    bool value;
    d.deserialize("value", value);
    set(value);
}


int BoolProperty::getVariantType() {
    return Variant::VariantTypeBool;
}

Variant BoolProperty::getVariant() {
    return Variant(get());
}

void  BoolProperty::setVariant(const Variant& val) {
    if (val.canConvert(getVariantType())) {
        set(val.getBool());
    }
}

} // namespace
