/*
 * Copyright (c) 2008 Hyperic, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#endif

#include <stdio.h>

#include "sigar.h"

int main(int argc, char **argv) {
    int status;
    unsigned long i;
    sigar_t *sigar;
    sigar_sys_info_t systeminfo;
    //sigar_resource_limit_t resinfo;
    sigar_mem_t meminfo;
    sigar_file_system_list_t diskinfolist;
    sigar_file_system_usage_t diskusageinfo;
    sigar_cpu_info_list_t cpulinfolist;

    sigar_open(&sigar);

    //Operating System Info
    status = sigar_sys_info_get(sigar, &systeminfo);
    if (status != SIGAR_OK) {
        printf("sys_info error: %d (%s)\n",
            status, sigar_strerror(sigar, status));
        return 1;
    }
    printf("OS Info: %s\n", systeminfo.description);

    //Resource Info
    /*status = sigar_resource_limit_get(sigar, &resinfo);
    if (status != SIGAR_OK) {
        printf("resource_limit error: %d (%s)\n",
            status, sigar_strerror(sigar, status));
        return 1;
    }
    printf("%i %i\n", resinfo.virtual_memory_cur, resinfo.virtual_memory_max);*/

    //Memory Info
    status = sigar_mem_get(sigar, &meminfo);
    if (status != SIGAR_OK) {
        printf("mem_info error: %d (%s)\n",
            status, sigar_strerror(sigar, status));
        return 1;
    }
    printf("Memory Info: %i %i %d %i %d %i\n", meminfo.ram, meminfo.free, meminfo.free_percent, meminfo.used, meminfo.used_percent, meminfo.total);

    //Disk Info
    status = sigar_file_system_list_get(sigar, &diskinfolist);
    if (status != SIGAR_OK) {
        printf("file_system_list error: %d (%s)\n",
            status, sigar_strerror(sigar, status));
        return 1;
    }
    for (i=0; i<diskinfolist.number; i++) {
        sigar_file_system_t disk_info = diskinfolist.data[i];

        status = sigar_file_system_usage_get(sigar, disk_info.dir_name, &diskusageinfo);

        if (status != SIGAR_OK) {
            printf("disk_usage error: %d (%s)\n",
                status, sigar_strerror(sigar, status));
            return 1;
        }

        printf("Disk Info: %s %i\n", disk_info.dev_name, diskusageinfo.total);
    }
    sigar_file_system_list_destroy(sigar, &diskinfolist);

    //Disk Usage
    /*status = sigar_disk_usage_get(sigar, &diskusageinfo);
    if (status != SIGAR_OK) {
        printf("disk_usage error: %d (%s)\n",
            status, sigar_strerror(sigar, status));
        return 1;
    }
    printf("%s\n", diskusageinfo.qtime);*/

    //CPU Info
    status = sigar_cpu_info_list_get(sigar, &cpulinfolist);
    if (status != SIGAR_OK) {
        printf("cpu_list error: %d (%s)\n",
            status, sigar_strerror(sigar, status));
        return 1;
    }
    for (i=0; i<cpulinfolist.number; i++) {
        sigar_cpu_info_t cpu_info = cpulinfolist.data[i];

        printf("CPU Info: %i %s\n", cpu_info.mhz, cpu_info.vendor);
    }
    sigar_cpu_info_list_destroy(sigar, &cpulinfolist);

    sigar_close(sigar);

    return 0;
}
