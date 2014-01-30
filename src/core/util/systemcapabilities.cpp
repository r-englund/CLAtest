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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include <inviwo/core/util/systemcapabilities.h>
#include <inviwo/core/util/formatconversion.h>
#include <inviwo/core/util/logcentral.h>
#include <sigar/include/sigar.h>

namespace inviwo {

#define SystemInfoNotFound(message) { LogInfo(message << " Info could not be retrieved"); }

    SystemCapabilities::SystemCapabilities() {}

    SystemCapabilities::~SystemCapabilities() {}

    bool SystemCapabilities::canAllocate(uint64_t dataSize, uint8_t percentageOfAvailableMemory){
        return getAvailableMemory()*percentageOfAvailableMemory/100 >= dataSize;
    }

    uvec3 SystemCapabilities::calculateOptimalBrickSize(uvec3 dimensions, size_t formatSizeInBytes, uint8_t percentageOfAvailableMemory){
        uvec3 currentBrickDimensions = dimensions;
        while(!canAllocate(getMemorySizeInBytes(currentBrickDimensions, formatSizeInBytes), percentageOfAvailableMemory)){
            int theMaxDim = (currentBrickDimensions.x > currentBrickDimensions.y ? (currentBrickDimensions.x > currentBrickDimensions.z ? 0 : 2) : (currentBrickDimensions.y > currentBrickDimensions.z ? 1 : 2));
            if (currentBrickDimensions[theMaxDim] % 2 != 0)
                currentBrickDimensions[theMaxDim]++; //Make the dim we are dividing even
            currentBrickDimensions[theMaxDim] /= 2;
        }
        return currentBrickDimensions;
    }

    void SystemCapabilities::retrieveStaticInfo(){
        successOSInfo_ = lookupOSInfo();
    }

    void SystemCapabilities::retrieveDynamicInfo(){
        successCPUInfo_ = lookupCPUInfo();
        successMemoryInfo_ = lookupMemoryInfo();
        successDiskInfo_ = lookupDiskInfo();
        //successProcessMemoryInfo_ = lookupProcessMemoryInfo();
    }

    bool SystemCapabilities::lookupOSInfo(){
        int status;
        sigar_t *sigar;
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

    bool SystemCapabilities::lookupCPUInfo(){
        infoCPUs_.clear();
        int status;
        sigar_t *sigar;
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
                infoCPUs_[i].mhz = static_cast<uint64_t>(cpu_info.mhz);
            }
        }
        sigar_cpu_info_list_destroy(sigar, &cpulinfolist);

        sigar_close(sigar);
        return SUCCESS;
    }

    bool SystemCapabilities::lookupMemoryInfo(){
        int status;
        sigar_t *sigar;
        sigar_open(&sigar);
        sigar_mem_t meminfo;

        status = sigar_mem_get(sigar, &meminfo);
        bool SUCCESS = (status == SIGAR_OK);
        if (SUCCESS) {
            infoRAM_.total = MEGABYTES_TO_BYTES(static_cast<uint64_t>(meminfo.ram));
            infoRAM_.available = static_cast<uint64_t>(meminfo.free);
        }

        sigar_close(sigar);
        return SUCCESS;
    }

    bool SystemCapabilities::lookupDiskInfo(){
        infoDisks_.clear();
        int status;
        sigar_t *sigar;
        sigar_open(&sigar);
        sigar_file_system_list_t diskinfolist;
        sigar_file_system_usage_t diskusageinfo;

        status = sigar_file_system_list_get(sigar, &diskinfolist);
        bool SUCCESS = (status == SIGAR_OK);
        if (SUCCESS) {
            for (unsigned long i=0; i<diskinfolist.number; i++) {
                sigar_file_system_t disk_info = diskinfolist.data[i];
                status = sigar_file_system_usage_get(sigar, disk_info.dir_name, &diskusageinfo);
                if (status == SIGAR_OK){
                    DiskInfo currentDiskInfo;
                    currentDiskInfo.diskType = std::string(disk_info.type_name);
                    currentDiskInfo.diskType[0] = toupper(currentDiskInfo.diskType[0]);
                    if (currentDiskInfo.diskType == "Local"){
                        currentDiskInfo.diskName = std::string(disk_info.dev_name);
                        currentDiskInfo.total = KILOBYTES_TO_BYTES(static_cast<uint64_t>(diskusageinfo.total));
                        currentDiskInfo.free = KILOBYTES_TO_BYTES(static_cast<uint64_t>(diskusageinfo.free));
                        infoDisks_.push_back(currentDiskInfo);
                    }
                }
            }
        }
        sigar_file_system_list_destroy(sigar, &diskinfolist);

        sigar_close(sigar);
        return SUCCESS;
    }

    bool SystemCapabilities::lookupProcessMemoryInfo(){
        int status;
        sigar_t *sigar;
        sigar_open(&sigar);
        sigar_proc_mem_t meminfo;

        status = sigar_proc_mem_get(sigar, sigar_pid_get(sigar), &meminfo);
        bool SUCCESS = (status == SIGAR_OK);
        if (SUCCESS) {
            infoProcRAM_.residentMem = static_cast<uint64_t>(meminfo.resident);
            infoProcRAM_.sharedMem = static_cast<uint64_t>(meminfo.share);
            infoProcRAM_.virtualMem = static_cast<uint64_t>(meminfo.size);
        }

        sigar_close(sigar);
        return SUCCESS;
    }

    void SystemCapabilities::printInfo(){
        retrieveDynamicInfo();

        // Try to retrieve operating system information
        if (successOSInfo_){
            LogInfo("OS: " << infoOS_.description << " " << infoOS_.platform << "-bit");
        }
        else{
            SystemInfoNotFound("OS:");
        }

        // Try to retrieve CPU information
        if (successCPUInfo_){
            for(unsigned long i=0; i<infoCPUs_.size(); i++){
                LogInfo("CPU " << i+1 << ": " << infoCPUs_[i].vendor << " " << infoCPUs_[i].model << " " << infoCPUs_[i].mhz << " Mhz");
            }
        }
        else{
            SystemInfoNotFound("CPU:");
        }

        // Try to retrieve memory information
        if (successMemoryInfo_){
            LogInfo("RAM: Total - " << formatBytesToString(infoRAM_.total) << ", Available - " << formatBytesToString(infoRAM_.available));
        }
        else{
            SystemInfoNotFound("RAM:");
        }

        // Try to retrieve Disk information
        if (successDiskInfo_){
            for(unsigned long i=0; i<infoDisks_.size(); i++){
                LogInfo("Disk: " << infoDisks_[i].diskName << " Total - " << formatBytesToString(infoDisks_[i].total) << ", Free - " << formatBytesToString(infoDisks_[i].free));
            }
        }
        else{
            SystemInfoNotFound("Disk:");
        }

        // Try to retrieve this process memory information
        /*if (successProcessMemoryInfo_){
        LogInfo("Processor Memory: Resident - " << formatBytes(infoProcRAM_.residentMem) << ", Shared - " << formatBytes(infoProcRAM_.sharedMem) << ", Virtual - " << formatBytes(infoProcRAM_.virtualMem));
        }
        else{
        SystemInfoNotFound("Processor Memory:");
        }*/
    }

    uint64_t SystemCapabilities::getAvailableMemory(){
        successMemoryInfo_ = lookupMemoryInfo();

        if (successMemoryInfo_){
            return infoRAM_.available;
        }

        return 0;
    }

} // namespace
