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

namespace inviwo {

SystemSettings::SystemSettings(InviwoModule* module, std::string id) : Settings(module, id), allocTest_(0) {}

SystemSettings::~SystemSettings() {
}

void SystemSettings::initialize() {
    OptionPropertyInt* viewMode_ = new OptionPropertyInt("viewMode","",0);
    viewMode_->addOption("developerMode","developerMode",0);
    viewMode_->addOption("applicationMode","applicationMode",1);
    addProperty(viewMode_);
    viewMode_->setVisibility(INVISIBLE);
    addProperty(new BoolProperty("txtEditor", "Use system text editor", true));

    addProperty(new BoolProperty("shaderReloading", "Automatically reload shaders", true));

    addProperty(new BoolProperty("enablePortInspectors", "Enable port inspectors", true));

    addProperty(new BoolProperty("enableSound", "Enable sound", true));        

    addProperty(new IntProperty("useRAMPercent", "Max Use Mem %", 50, 1, 100));

    ButtonProperty* btnAllocTest = new ButtonProperty("allocTest", "Perform Allocation Test");
    btnAllocTest->onChange(this, &SystemSettings::allocationTest);
    addProperty(btnAllocTest);

    SystemCapabilities* sysInfo = getTypeFromVector<SystemCapabilities>(module_->getCapabilities());
    if (sysInfo){
        ButtonProperty* btnSysInfo = new ButtonProperty("printSysInfo", "Print System Info");
        btnSysInfo->onChange(sysInfo, &SystemCapabilities::printInfo);
        addProperty(btnSysInfo);  
    } 
}

void SystemSettings::deinitialize()  {}

void SystemSettings::allocationTest(){
    SystemCapabilities* sysInfo = getTypeFromVector<SystemCapabilities>(module_->getCapabilities());
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
