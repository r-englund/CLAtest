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

#ifndef IVW_LINKCONDITIONS_H
#define IVW_LINKCONDITIONS_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/property.h>

namespace inviwo {

enum LinkingConditions { NoLinkCondition=0,
                         LinkMatchingTypes=1<<1,
                         LinkMatchingId=1<<2,
                         ApplyAllConditions=~0,
                         DefaultLinkingCondition=LinkMatchingTypes
                       };


class IVW_CORE_API SimpleCondition {
public:
    SimpleCondition() {}
    static bool canLink(Property* src, Property* dst);
    static LinkingConditions conditionType() { return LinkMatchingTypes;}
    static std::string conditionName() { return "Matching Type";}
};

class IVW_CORE_API PartiallyMatchingIdCondition {
public:
    PartiallyMatchingIdCondition() {}
    static bool canLink(Property* src, Property* dst);
    static LinkingConditions conditionType() { return LinkMatchingId;}
    static std::string conditionName() { return "Matching Id";}
};

class IVW_CORE_API AutoLinker {
public:
    AutoLinker() {}
    static bool canLink(Property* src, Property* dst, LinkingConditions);

};

} // namespace

#endif // IVW_LINKCONDITIONS_H