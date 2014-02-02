/**********************************************************************
 * Copyright (C) 2014 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#ifndef IVW_OPENCL_SETTINGS_H
#define IVW_OPENCL_SETTINGS_H

#include <inviwo/core/util/settings/settings.h>
#include <inviwo/core/properties/optionproperties.h>
#include <inviwo/core/properties/boolproperty.h>
#include <modules/opencl/openclmoduledefine.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API OpenCLSettings : public Settings {

public:
    OpenCLSettings(std::string id="OpenCL Settings");
    virtual ~OpenCLSettings();
    virtual void initialize();
    virtual void deinitialize();

protected:
    void changeDevice();

private:
    OptionPropertyInt openCLDeviceProperty_; // List of devices
    BoolProperty enableOpenGLSharing_;

};

} // namespace

#endif // IVW_OPENCL_SETTINGS_H
