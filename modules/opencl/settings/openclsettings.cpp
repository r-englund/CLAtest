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

#include <inviwo/core/util/settings/systemsettings.h>

#include <modules/opencl/settings/openclsettings.h>
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/kernelmanager.h>

namespace inviwo {

OpenCLSettings::OpenCLSettings(std::string id) : 
    Settings(id)
    , openCLDeviceProperty_ ("openCLDevice","Default device",0)
    , enableOpenGLSharing_("glsharing", "Enable OpenGL sharing", true)
{}

OpenCLSettings::~OpenCLSettings() {
}

void OpenCLSettings::initialize() {

    std::vector<cl::Device> devices = OpenCL::getAllDevices();
    for (size_t i = 0; i < devices.size(); ++i) {
        std::string name = devices[i].getInfo<CL_DEVICE_NAME>();
        openCLDeviceProperty_.addOption(name, name, static_cast<int>(i));
    }
    addProperty(openCLDeviceProperty_);
    addProperty(enableOpenGLSharing_);
    openCLDeviceProperty_.onChange(this, &OpenCLSettings::changeDevice);
    enableOpenGLSharing_.onChange(this, &OpenCLSettings::changeDevice);
}

void OpenCLSettings::deinitialize()  {}

void OpenCLSettings::changeDevice() {
    // TODO: Close network before changing device. Load it again after changing
    //InviwoApplication::getPtr()->getProcessorNetwork()->
    KernelManager::getPtr()->clear();
    std::vector<cl::Device> devices = OpenCL::getAllDevices();
    // Assuming that the number of devices did not change since initialize()
    // was called
    if (openCLDeviceProperty_.get() < static_cast<int>(devices.size())) {
        OpenCL::instance()->setDevice(devices[openCLDeviceProperty_.get()], enableOpenGLSharing_.get());
    }


}

} // namespace
