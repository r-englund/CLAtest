#include <inviwo/core/util/systeminfo.h>
#include <sigar/include/sigar.h>
#include <inviwo/core/util/logdistributor.h>
#include <iomanip>

namespace inviwo {

#define SystemInfoNotFound(message) { LogInfo(message << " Info could not be retrieved"); }

const std::string SystemInfo::logSource_ = "SystemInfo";

SystemInfo::SystemInfo() {}

SystemInfo::~SystemInfo() {}

void SystemInfo::retrieveStaticInfo(){
    successOSInfo_ = lookupOSInfo();
}

void SystemInfo::retrieveDynamicInfo(){
    successCPUInfo_ = lookupCPUInfo();
    successMemoryInfo_ = lookupMemoryInfo();
    successDiskInfo_ = lookupDiskInfo();
    //successProcessMemoryInfo_ = lookupProcessMemoryInfo();
}

bool SystemInfo::lookupOSInfo(){
    int status;
    sigar_t *sigar;
    sigar_open(&sigar);
    sigar_sys_info_t systeminfo;

    status = sigar_sys_info_get(sigar, &systeminfo);
    bool SUCCESS = (status == SIGAR_OK);
    if(SUCCESS) {
        infoOS_.description = std::string(systeminfo.description);
        if(systeminfo.arch == "x86")
            infoOS_.platform = 32;
        else
            infoOS_.platform = 64;
    }

    sigar_close(sigar);
    return SUCCESS;
}

bool SystemInfo::lookupCPUInfo(){
    infoCPUs_.clear();
    int status;
    sigar_t *sigar;
    sigar_open(&sigar);
    sigar_cpu_info_list_t cpulinfolist;
    status = sigar_cpu_info_list_get(sigar, &cpulinfolist);

    bool SUCCESS = (status == SIGAR_OK);
    if(SUCCESS) {
        infoCPUs_.resize(cpulinfolist.number);
        for (unsigned long i=0; i<cpulinfolist.number; i++) {
            sigar_cpu_info_t cpu_info = cpulinfolist.data[i];
            infoCPUs_[i].vendor = std::string(cpu_info.vendor);
            infoCPUs_[i].model = std::string(cpu_info.model);
            infoCPUs_[i].mhz = static_cast<size_t>(cpu_info.mhz);
        }
    }
    sigar_cpu_info_list_destroy(sigar, &cpulinfolist);

    sigar_close(sigar);
    return SUCCESS;
}

bool SystemInfo::lookupMemoryInfo(){
    int status;
    sigar_t *sigar;
    sigar_open(&sigar);
    sigar_mem_t meminfo;
    
    status = sigar_mem_get(sigar, &meminfo);
    bool SUCCESS = (status == SIGAR_OK);
    if(SUCCESS) {
        infoRAM_.total = static_cast<size_t>(meminfo.ram);
        infoRAM_.available = static_cast<size_t>(meminfo.actual_free/1000000);
    }

    sigar_close(sigar);
    return SUCCESS;
}

bool SystemInfo::lookupDiskInfo(){
    infoDisks_.clear();
    int status;
    sigar_t *sigar;
    sigar_open(&sigar);
    sigar_file_system_list_t diskinfolist;
    sigar_file_system_usage_t diskusageinfo;

    status = sigar_file_system_list_get(sigar, &diskinfolist);
    bool SUCCESS = (status == SIGAR_OK);
    if(SUCCESS) {
        for (unsigned long i=0; i<diskinfolist.number; i++) {
            sigar_file_system_t disk_info = diskinfolist.data[i];
            status = sigar_file_system_usage_get(sigar, disk_info.dir_name, &diskusageinfo);
            if(status == SIGAR_OK){
                DiskInfo currentDiskInfo;
                currentDiskInfo.diskType = std::string(disk_info.type_name);
                currentDiskInfo.diskType[0] = toupper(currentDiskInfo.diskType[0]);
                if(currentDiskInfo.diskType == "Local"){
                    currentDiskInfo.diskName = std::string(disk_info.dev_name);
                    currentDiskInfo.total = static_cast<size_t>(diskusageinfo.total/1000);
                    currentDiskInfo.free = static_cast<size_t>(diskusageinfo.free/1000);
                    infoDisks_.push_back(currentDiskInfo);
                }
            }
        }
    }
    sigar_file_system_list_destroy(sigar, &diskinfolist);

    sigar_close(sigar);
    return SUCCESS;
}

bool SystemInfo::lookupProcessMemoryInfo(){
    int status;
    sigar_t *sigar;
    sigar_open(&sigar);
    sigar_proc_mem_t meminfo;

    status = sigar_proc_mem_get(sigar, sigar_pid_get(sigar), &meminfo);
    bool SUCCESS = (status == SIGAR_OK);
    if(SUCCESS) {
        infoProcMem_.residentMem = static_cast<size_t>(meminfo.resident/1000);
        infoProcMem_.sharedMem = static_cast<size_t>(meminfo.share/1000);
        infoProcMem_.virtualMem = static_cast<size_t>(meminfo.size/1000);
    }

    sigar_close(sigar);
    return SUCCESS;
}

void SystemInfo::printInfo(){
    // Try to retrieve operating system information
    if(successOSInfo_){
        LogInfo("(OS) " << infoOS_.description << " " << infoOS_.platform << "-bit");
    }
    else{
        SystemInfoNotFound("(OS)");
    }

    // Try to retrieve CPU information
    if(successCPUInfo_){
        for(unsigned long i=0; i<infoCPUs_.size(); i++){
            LogInfo("(CPU " << i+1 << ") " << infoCPUs_[i].vendor << " " << infoCPUs_[i].model << " " << infoCPUs_[i].mhz << " Mhz");
        }
    }
    else{
        SystemInfoNotFound("(CPU)");
    }

    // Try to retrieve memory information
    if(successMemoryInfo_){
        LogInfo("(RAM) Total - " << infoRAM_.total << " MB, Free - " << infoRAM_.available << " MB");
    }
    else{
        SystemInfoNotFound("(RAM)");
    }

    // Try to retrieve Disk information
    if(successDiskInfo_){
        for(unsigned long i=0; i<infoDisks_.size(); i++){
            LogInfo("(Disk " << infoDisks_[i].diskName << ") Total - " << infoDisks_[i].total/1000 << " GB, Free - " << infoDisks_[i].free/1000 << " GB");
        }
    }
    else{
        SystemInfoNotFound("(Disk)");
    }

    // Try to retrieve this process memory information
    /*if(successProcessMemoryInfo_){
        LogInfo("(Processor Memory) Resident - " << infoProcRAM_.residentMem << " MB, Shared - " << infoProcRAM_.sharedMem << " MB, Virtual - " << infoProcRAM_.virtualMem << " MB");
    }
    else{
        SystemInfoNotFound("(Processor Memory)");
    }*/
}

} // namespace
