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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#ifndef IVW_SYSTEMSETTINGS_H
#define IVW_SYSTEMSETTINGS_H

#include <inviwo/core/util/settings/settings.h>

namespace inviwo {

class IVW_CORE_API SystemSettings : public Settings {

public:
    SystemSettings(InviwoModule* module, std::string id="System Settings");
    virtual ~SystemSettings();
    virtual void initialize();
    virtual void deinitialize();

protected:
    void allocationTest();
    uint32_t* allocTest_;
};

} // namespace

#endif // IVW_SYSTEMSETTINGS_H
