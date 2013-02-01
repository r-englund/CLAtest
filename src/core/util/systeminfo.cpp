#include <inviwo/core/util/systeminfo.h>
#include <sigar/include/sigar.h>
#include <inviwo/core/util/logdistributor.h>
#include <iomanip>

namespace inviwo {

#define SystemInfo(message) { LogInfoS("SystemInfo", message); }
#define SystemInfoNotFound(message) { LogInfoS("SystemInfo", message << " Info could not be retrieved"); }

bool lookupOSInfo(OSInfo& infoOS){
    int status;
    sigar_t *sigar;
    sigar_open(&sigar);
    sigar_sys_info_t systeminfo;

    status = sigar_sys_info_get(sigar, &systeminfo);
    bool SUCCESS = (status == SIGAR_OK);
    if(SUCCESS) {
        infoOS.description = std::string(systeminfo.description);
        if(systeminfo.arch == "x86")
            infoOS.platform = 32;
        else
            infoOS.platform = 64;
    }

    sigar_close(sigar);
    return SUCCESS;
}

bool lookupCPUInfo(std::vector<CPUInfo> &infoCPUs){
    infoCPUs.clear();
    int status;
    sigar_t *sigar;
    sigar_open(&sigar);
    sigar_cpu_info_list_t cpulinfolist;
    status = sigar_cpu_info_list_get(sigar, &cpulinfolist);

    bool SUCCESS = (status == SIGAR_OK);
    if(SUCCESS) {
        infoCPUs.resize(cpulinfolist.number);
        for (unsigned long i=0; i<cpulinfolist.number; i++) {
            sigar_cpu_info_t cpu_info = cpulinfolist.data[i];
            infoCPUs[i].vendor = std::string(cpu_info.vendor);
            infoCPUs[i].model = std::string(cpu_info.model);
            infoCPUs[i].mhz = static_cast<size_t>(cpu_info.mhz);
        }
    }
    sigar_cpu_info_list_destroy(sigar, &cpulinfolist);

    sigar_close(sigar);
    return SUCCESS;
}

bool lookupMemoryInfo(MemoryInfo& infoRAM){
    int status;
    sigar_t *sigar;
    sigar_open(&sigar);
    sigar_mem_t meminfo;
    
    status = sigar_mem_get(sigar, &meminfo);
    bool SUCCESS = (status == SIGAR_OK);
    if(SUCCESS) {
        infoRAM.total = static_cast<size_t>(meminfo.ram);
        infoRAM.available = static_cast<size_t>(meminfo.free/1000000);
    }

    sigar_close(sigar);
    return SUCCESS;
}

bool lookupDiskInfo(std::vector<DiskInfo> &infoDisks){
    infoDisks.clear();
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
                    infoDisks.push_back(currentDiskInfo);
                }
            }
        }
    }
    sigar_file_system_list_destroy(sigar, &diskinfolist);

    sigar_close(sigar);
    return SUCCESS;
}

void printSystemInfo(){
    // Try to retrieve operating system information
    OSInfo infoOS;
    if(lookupOSInfo(infoOS)){
        SystemInfo("(OS) " << infoOS.description << " " << infoOS.platform << "-bit");
    }
    else{
        SystemInfoNotFound("(OS)");
    }

    // Try to retrieve CPU information
    std::vector<CPUInfo> infoCPUs;
    if(lookupCPUInfo(infoCPUs)){
        for(unsigned long i=0; i<infoCPUs.size(); i++){
            SystemInfo("(CPU " << i+1 << ") " << infoCPUs[i].vendor << " " << infoCPUs[i].model << " " << infoCPUs[i].mhz << " Mhz");
        }
    }
    else{
        SystemInfoNotFound("(CPU)");
    }

    // Try to retrieve memory information
    MemoryInfo infoRAM;
    if(lookupMemoryInfo(infoRAM)){
        SystemInfo("(RAM) Total - " << infoRAM.total << " MB, Free - " << infoRAM.available << " MB");
    }
    else{
        SystemInfoNotFound("(RAM)");
    }

    // Try to retrieve CPU information
    std::vector<DiskInfo> infoDisks;
    if(lookupDiskInfo(infoDisks)){
        for(unsigned long i=0; i<infoDisks.size(); i++){
            SystemInfo("(Disk " << infoDisks[i].diskName << ") Total - " << infoDisks[i].total/1000 << " GB, Free - " << infoDisks[i].free/1000 << " GB");
        }
    }
    else{
        SystemInfoNotFound("(Disk)");
    }
}

} // namespace
