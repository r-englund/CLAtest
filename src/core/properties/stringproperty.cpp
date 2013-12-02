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
 * Primary author : Alexander Johansson
 *
 **********************************************************************/

#include <inviwo/core/properties/stringproperty.h>

namespace inviwo {

StringProperty::StringProperty(std::string identifier, std::string displayName, std::string value, PropertyOwner::InvalidationLevel invalidationLevel, PropertySemantics::Type semantics )
: TemplateProperty<std::string>(identifier, displayName,value, invalidationLevel, semantics)
{}

void StringProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("value", get());
}

void StringProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    std::string value;
    d.deserialize("value", value);
    set(value);
}

} // namespace
