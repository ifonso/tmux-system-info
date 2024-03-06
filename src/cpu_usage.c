#include "cpu_usage.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <mach/mach.h>

int get_cpu_percent_usage(unsigned int time_stamp_in_seconds, double *cpu_usage) {
    host_cpu_load_info_data_t cpuinfo_i, cpuinfo_f;
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    mach_port_t host_self = mach_host_self();

    kern_return_t result = host_statistics(host_self,
                                           HOST_CPU_LOAD_INFO,
                                           (host_info_t)&cpuinfo_i,
                                           &count);

    if (result != KERN_SUCCESS) {
        return -1;
    }

    sleep(time_stamp_in_seconds);

    result = host_statistics(host_self,
                             HOST_CPU_LOAD_INFO,
                             (host_info_t)&cpuinfo_f,
                             &count);

    if (result != KERN_SUCCESS) {
        return -1;
    }

    unsigned long long total_i = 0, total_f = 0;

    for (int i = 0; i < CPU_STATE_MAX; i++) {
        total_i += cpuinfo_i.cpu_ticks[i];
        total_f += cpuinfo_f.cpu_ticks[i];
    }

    unsigned long long used_i = total_i - cpuinfo_i.cpu_ticks[CPU_STATE_IDLE];
    unsigned long long used_f = total_f - cpuinfo_f.cpu_ticks[CPU_STATE_IDLE];

    *cpu_usage = ((double) (used_f - used_i) / (double) (total_f - total_i)) * 100.0;

    return 0;
}