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

#include <inviwo/core/util/settings/systemsettings.h>

#include <inviwo/core/util/systemcapabilities.h>
#include <inviwo/core/util/formatconversion.h>

#include <inviwo/core/common/inviwoapplication.h>

namespace inviwo {
SystemSettings::SystemSettings(std::string id) : 
    Settings(id)
    , allocTest_(0)
    , viewModeProperty_ ("viewMode","",0)
    , txtEditorProperty_("txtEditor", "Use system text editor", true)
    , shaderReloadingProperty_("shaderReloading", "Automatically reload shaders", true)
    , enablePortInspectorsProperty_("enablePortInspectors", "Enable port inspectors", true)
    , portInspectorSize_("portInspectorSize", "Port Inspector size", 128, 1, 1024)
    , enableSoundProperty_("enableSound", "Enable sound", true)
    , useRAMPercentProperty_("useRAMPercent", "Max Use Mem %", 50, 1, 100)
    , btnAllocTestProperty_("allocTest", "Perform Allocation Test")
    , btnSysInfoProperty_("printSysInfo", "Print System Info")
{}

SystemSettings::~SystemSettings() {
}

void SystemSettings::initialize() {
    viewModeProperty_.addOption("developerMode","developerMode",0);
    viewModeProperty_.addOption("applicationMode","applicationMode",1);
    addProperty(&viewModeProperty_);
    viewModeProperty_.setVisibility(INVISIBLE);

    addProperty(&txtEditorProperty_);
    addProperty(&shaderReloadingProperty_);
    addProperty(&enablePortInspectorsProperty_);
    addProperty(&portInspectorSize_);
    addProperty(&enableSoundProperty_);
    addProperty(&useRAMPercentProperty_);

    //btnAllocTestProperty_.onChange(this, &SystemSettings::allocationTest);
    //addProperty(&btnAllocTestProperty_);

    InviwoCore* module = InviwoApplication::getPtr()->getModuleByType<InviwoCore>();
    ivwAssert(module!=0, "Core module is not yet registered")
    SystemCapabilities* sysInfo = getTypeFromVector<SystemCapabilities>(module->getCapabilities());
    if (sysInfo){
        btnSysInfoProperty_.onChange(sysInfo, &SystemCapabilities::printInfo);
        addProperty(&btnSysInfoProperty_);  
    } 
}

void SystemSettings::deinitialize()  {}

void SystemSettings::allocationTest(){
    InviwoCore* module = InviwoApplication::getPtr()->getModuleByType<InviwoCore>();
    ivwAssert(module!=0, "Core module is not yet registered")
    SystemCapabilities* sysInfo = getTypeFromVector<SystemCapabilities>(module->getCapabilities());
    if (sysInfo){
        if (allocTest_){
            delete allocTest_;
            LogInfo("Deleted previous test allocation");
        }
        IntProperty* useRAMPercent = dynamic_cast<IntProperty*>(getPropertyByIdentifier("useRAMPercent"));
        uint64_t memBytesAlloc = sysInfo->getAvailableMemory(); //In Bytes
        LogInfo("Maximum Available Memory is " << formatBytesToString(memBytesAlloc));
        memBytesAlloc /= 100; //1% of total available memory
        memBytesAlloc *= useRAMPercent->get(); //?% of total available memory
        try
        {
            allocTest_ = new uint32_t[static_cast<uint32_t>(memBytesAlloc/4)];
            LogInfo("Allocated " << formatBytesToString(memBytesAlloc) << ", which is " << useRAMPercent->get() << "% of available memory");
        }
        catch(std::bad_alloc&)
        {
            LogError("Failed allocation of " << formatBytesToString(memBytesAlloc) << ", which is " << useRAMPercent->get() << "% of available memory");
        }
    }
}

} // namespace
