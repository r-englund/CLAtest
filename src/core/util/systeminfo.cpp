#include <inviwo/core/util/systeminfo.h>
#include <sigar/include/sigar.h>
#include <inviwo/core/util/logdistributor.h>

namespace inviwo {

size_t getTotalRAM(){
    int status;
    sigar_t *sigar;

    sigar_open(&sigar);

    sigar_mem_t meminfo;

    status = sigar_mem_get(sigar, &meminfo);
    if (status != SIGAR_OK) {
        /*printf("mem_info error: %d (%s)\n",
            status, sigar_strerror(sigar, status));*/
        return 0;
    }

    return static_cast<size_t>(meminfo.ram);
}

void getSystemInfo(std::ostringstream& stream){
    stream << "Total RAM: " << getTotalRAM() << " MB";
}

void printSystemInfo(){
    std::ostringstream stream;
    getSystemInfo(stream);
    std::string systemInfoStr = stream.str();
    LogInfoS("SystemInfo", systemInfoStr);
}

} // namespace
