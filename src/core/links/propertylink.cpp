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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <inviwo/core/links/propertylink.h>

namespace inviwo {

PropertyLink::PropertyLink()
    : srcProperty_(0),
      dstProperty_(0) {
}

PropertyLink::~PropertyLink() {}

PropertyLink::PropertyLink(Property* srcProperty, Property* destProperty)
    : srcProperty_(srcProperty), dstProperty_(destProperty) {
}

void PropertyLink::switchDirection() {
    Property* tempProperty = srcProperty_;
    srcProperty_ = dstProperty_;
    dstProperty_ = tempProperty;
}

void PropertyLink::serialize(IvwSerializer& s) const {
    std::vector<Property*> linkedProperties;
    linkedProperties.push_back(srcProperty_);
    linkedProperties.push_back(dstProperty_);
    s.serialize("Properties", linkedProperties, "Property");
}

void PropertyLink::deserialize(IvwDeserializer& d) {
    std::vector<Property*> linkedProperties;
    d.deserialize("Properties",linkedProperties, "Property");
    srcProperty_ = linkedProperties[0];
    dstProperty_ = linkedProperties[1];
}

} // namespace
