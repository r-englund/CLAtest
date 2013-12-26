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

class IVW_CORE_API SimpleLinkCondition {
public:
   SimpleLinkCondition() {}
   static bool canLink(Property* src, Property *dst);
};

class IVW_CORE_API AutoLinkCondition {
public:
   AutoLinkCondition() {}
   static bool canLink(Property* src, Property *dst);  
};

} // namespace

#endif // IVW_LINKCONDITIONS_H