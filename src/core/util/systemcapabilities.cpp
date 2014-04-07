/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

#include <inviwo/core/util/systemcapabilities.h>
#include <inviwo/core/util/formatconversion.h>
#include <inviwo/core/util/logcentral.h>
#include <sigar/include/sigar.h>

namespace inviwo {

#define SystemInfoNotFound(message) { LogInfoCustom("SystemInfo",message << " Info could not be retrieved"); }

SystemCapabilities::SystemCapabilities() {}

SystemCapabilities::~SystemCapabilities() {}

bool SystemCapabilities::canAllocate(glm::u64 dataSize, glm::u8 percentageOfAvailableMemory) {
    return getAvailableMemory()*percentageOfAvailableMemory/100 >= dataSize;
}

uvec3 SystemCapabilities::calculateOptimalBrickSize(uvec3 dimensions, size_t formatSizeInBytes, glm::u8 percentageOfAvailableMemory) {
    uvec3 currentBrickDimensions = dimensions;

    while (!canAllocate(getMemorySizeInBytes(currentBrickDimensions, formatSizeInBytes), percentageOfAvailableMemory)) {
        int theMaxDim = (currentBrickDimensions.x > currentBrickDimensions.y ? (currentBrickDimensions.x > currentBrickDimensions.z ? 0 : 2) :
                         (currentBrickDimensions.y > currentBrickDimensions.z ? 1 : 2));

        if (currentBrickDimensions[theMaxDim] % 2 != 0)
            currentBrickDimensions[theMaxDim]++; //Make the dim we are dividing even

        currentBrickDimensions[theMaxDim] /= 2;
    }

    return currentBrickDimensions;
}

void SystemCapabilities::retrieveStaticInfo() {
    successOSInfo_ = lookupOSInfo();
}

void SystemCapabilities::retrieveDynamicInfo() {
    successCPUInfo_ = lookupCPUInfo();
    successMemoryInfo_ = lookupMemoryInfo();
    successDiskInfo_ = lookupDiskInfo();
    //successProcessMemoryInfo_ = lookupProcessMemoryInfo();
}

bool SystemCapabilities::lookupOSInfo() {
    int status;
    sigar_t* sigar;
    sigar_open(&sigar);
    sigar_sys_info_t systeminfo;
    status = sigar_sys_info_get(sigar, &systeminfo);
    bool SUCCESS = (status == SIGAR_OK);

    if (SUCCESS) {
        infoOS_.description = std::string(systeminfo.description);

        if (strcmp(systeminfo.arch, "x86") == 0)
            infoOS_.platform = 32;
        else
            infoOS_.platform = 64;
    }

    sigar_close(sigar);
    return SUCCESS;
}

bool SystemCapabilities::lookupCPUInfo() {
    infoCPUs_.clear();
    int status;
    sigar_t* sigar;
    sigar_open(&sigar);
    sigar_cpu_info_list_t cpulinfolist;
    status = sigar_cpu_info_list_get(sigar, &cpulinfolist);
    bool SUCCESS = (status == SIGAR_OK);

    if (SUCCESS) {
        infoCPUs_.resize(cpulinfolist.number);

        for (unsigned long i=0; i<cpulinfolist.number; i++) {
            sigar_cpu_info_t cpu_info = cpulinfolist.data[i];
            infoCPUs_[i].vendor = std::string(cpu_info.vendor);
            infoCPUs_[i].model = std::string(cpu_info.model);
            infoCPUs_[i].mhz = static_cast<glm::u64>(cpu_info.mhz);
        }
    }

    sigar_cpu_info_list_destroy(sigar, &cpulinfolist);
    sigar_close(sigar);
    return SUCCESS;
}

bool SystemCapabilities::lookupMemoryInfo() {
    int status;
    sigar_t* sigar;
    sigar_open(&sigar);
    sigar_mem_t meminfo;
    status = sigar_mem_get(sigar, &meminfo);
    bool SUCCESS = (status == SIGAR_OK);

    if (SUCCESS) {
        infoRAM_.total = MEGABYTES_TO_BYTES(static_cast<glm::u64>(meminfo.ram));
        infoRAM_.available = static_cast<glm::u64>(meminfo.free);
    }

    sigar_close(sigar);
    return SUCCESS;
}

bool SystemCapabilities::lookupDiskInfo() {
    infoDisks_.clear();
    int status;
    sigar_t* sigar;
    sigar_open(&sigar);
    sigar_file_system_list_t diskinfolist;
    sigar_file_system_usage_t diskusageinfo;
    status = sigar_file_system_list_get(sigar, &diskinfolist);
    bool SUCCESS = (status == SIGAR_OK);

    if (SUCCESS) {
        for (unsigned long i=0; i<diskinfolist.number; i++) {
            sigar_file_system_t disk_info = diskinfolist.data[i];
            status = sigar_file_system_usage_get(sigar, disk_info.dir_name, &diskusageinfo);

            if (status == SIGAR_OK) {
                DiskInfo currentDiskInfo;
                currentDiskInfo.diskType = std::string(disk_info.type_name);
                currentDiskInfo.diskType[0] = toupper(currentDiskInfo.diskType[0]);

                if (currentDiskInfo.diskType == "Local") {
                    currentDiskInfo.diskName = std::string(disk_info.dev_name);
                    currentDiskInfo.total = KILOBYTES_TO_BYTES(static_cast<glm::u64>(diskusageinfo.total));
                    currentDiskInfo.free = KILOBYTES_TO_BYTES(static_cast<glm::u64>(diskusageinfo.free));
                    infoDisks_.push_back(currentDiskInfo);
                }
            }
        }
    }

    sigar_file_system_list_destroy(sigar, &diskinfolist);
    sigar_close(sigar);
    return SUCCESS;
}

bool SystemCapabilities::lookupProcessMemoryInfo() {
    int status;
    sigar_t* sigar;
    sigar_open(&sigar);
    sigar_proc_mem_t meminfo;
    status = sigar_proc_mem_get(sigar, sigar_pid_get(sigar), &meminfo);
    bool SUCCESS = (status == SIGAR_OK);

    if (SUCCESS) {
        infoProcRAM_.residentMem = static_cast<glm::u64>(meminfo.resident);
        infoProcRAM_.sharedMem = static_cast<glm::u64>(meminfo.share);
        infoProcRAM_.virtualMem = static_cast<glm::u64>(meminfo.size);
    }

    sigar_close(sigar);
    return SUCCESS;
}

void SystemCapabilities::printInfo() {
    retrieveDynamicInfo();

    // Try to retrieve operating system information
    if (successOSInfo_) {
        LogInfoCustom("SystemInfo","OS: " << infoOS_.description << " " << infoOS_.platform << "-bit");
    } else {
        SystemInfoNotFound("OS:");
    }

    // Try to retrieve CPU information
    if (successCPUInfo_) {
        for (unsigned long i=0; i<infoCPUs_.size(); i++)
            LogInfoCustom("SystemInfo","CPU " << i+1 << ": " << infoCPUs_[i].vendor << " " << infoCPUs_[i].model << " " << infoCPUs_[i].mhz << " Mhz");
    }
    else
        SystemInfoNotFound("CPU:");

    // Try to retrieve memory information
    if (successMemoryInfo_) {
        LogInfoCustom("SystemInfo","RAM: Total - " << formatBytesToString(infoRAM_.total) << ", Available - " << formatBytesToString(infoRAM_.available));
    } else {
        SystemInfoNotFound("RAM:");
    }

    // Try to retrieve Disk information
    if (successDiskInfo_) {
        for (unsigned long i=0; i<infoDisks_.size(); i++)
            LogInfoCustom("SystemInfo","Disk: " << infoDisks_[i].diskName << " Total - " << formatBytesToString(infoDisks_[i].total) << ", Free - " << formatBytesToString(
                        infoDisks_[i].free));
    }
    else
        SystemInfoNotFound("Disk:");

    // Try to retrieve this process memory information
    /*if (successProcessMemoryInfo_){
    LogInfoCustom("SystemInfo","Processor Memory: Resident - " << formatBytes(infoProcRAM_.residentMem) << ", Shared - " << formatBytes(infoProcRAM_.sharedMem) << ", Virtual - " << formatBytes(infoProcRAM_.virtualMem));
    }
    else{
    SystemInfoNotFound("Processor Memory:");
    }*/
}

glm::u64 SystemCapabilities::getAvailableMemory() {
    successMemoryInfo_ = lookupMemoryInfo();

    if (successMemoryInfo_)
        return infoRAM_.available;

    return 0;
}

} // namespace
