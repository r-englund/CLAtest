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

#ifndef IVW_INVIWOCORE_H
#define IVW_INVIWOCORE_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/common/inviwomodule.h>

namespace inviwo {

class IVW_CORE_API InviwoCore : public InviwoModule {

public:
    InviwoCore();

protected:
    virtual void setupModuleSettings();
};

} // namespace

#endif // IVW_INVIWOCORE_H
