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
#include <inviwo/core/properties/optionproperties.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/scalarproperties.h>
#include <inviwo/core/properties/buttonproperty.h>

namespace inviwo {

class IVW_CORE_API SystemSettings : public Settings {

public:
    SystemSettings(std::string id="System Settings");
    virtual ~SystemSettings();
    virtual void initialize();
    virtual void deinitialize();

protected:
    void allocationTest();
    uint32_t* allocTest_;

private:
    OptionPropertyInt viewModeProperty_;
    BoolProperty txtEditorProperty_;
    BoolProperty shaderReloadingProperty_;
    BoolProperty enablePortInspectorsProperty_;
    BoolProperty enableSoundProperty_;
    IntProperty  useRAMPercentProperty_;

    ButtonProperty btnAllocTestProperty_;
    ButtonProperty btnSysInfoProperty_;
};

} // namespace

#endif // IVW_SYSTEMSETTINGS_H
