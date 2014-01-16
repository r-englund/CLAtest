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

#ifndef IVW_INVIWOFACTORYBASE_H
#define IVW_INVIWOFACTORYBASE_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <string>

namespace inviwo {

class IvwSerializable;

class IVW_CORE_API Factory {

public:
    virtual ~Factory() {}

    virtual void initialize()=0;
    virtual void deinitialize()=0;

    virtual IvwSerializable* create(std::string className) const { return 0; }
    virtual bool isValidType(std::string className) const {return false; }
};

} // namespace

#endif // IVW_INVIWOFACETORYBASE_H
