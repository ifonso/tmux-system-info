#include "mem_usage.h"

#include <stdlib.h>
#include <unistd.h>

#include <mach/mach.h>
#include <sys/sysctl.h>

int get_mem_info(mem_info_t *mem_info) {
    int64_t total_mem;
    int64_t used_mem;

    /* OPTIONS
    *   - {CTL_HW, HW_PHYSMEM}: Get the amount of physical memory.
    *   - {CTL_HW, HW_USERMEM}: Get the amount of non-kernel memory.
    *   - {CTL_HW, HW_PAGESIZE}: Get the system page size.
    *   - {CTL_HW, HW_NCPU}: Get the number of CPUs.
    */

    /// `mib`: array of integers where each integer specifies a node in a tree of system information
    /// `CTL_HW`: represents the node for hardware information
    /// `HW_MEMSIZE`: represents the node for the total amount of physical memory in the system
    int mib[2] = {CTL_HW, HW_MEMSIZE};

    size_t length = sizeof(total_mem);

    /*
    *   sysctl -> is a function provided by Unix-like operating systems for retrieving and setting 
    *   system configuration values. (sys/sysctl.h)
    * 
    *   - mib: array of integers where each integer specifies a node in a tree of system information
    *   - 2: specifies the number of elements in the mib array
    *   - &total_mem: pointer to a buffer where the result will be stored
    *   - &length: pointer to a variable that specifies the size of the buffer
    *   - NULL: reserved for future use
    *   - 0: reserved for future use
    */
    sysctl(mib, 2, &total_mem, &length, NULL, 0);

    vm_size_t page_size;
    mach_port_t host;
    mach_msg_type_number_t count;
    vm_statistics_data_t vm_stats;

    /// Mach port is a communication channel that is used for interprocess communication (IPC). `mach_port_t` is 
    /// an integer that identifies a Mach port.
    host = mach_host_self();
    /// Get the size of the `vm_stats` structure in natural_t units.
    /// `vm_statistics_data_t` is used to store various statistics about virtual memory usage.
    count = sizeof(vm_stats) / sizeof(natural_t);

    if (
        KERN_SUCCESS == host_page_size(host, &page_size) &&
        KERN_SUCCESS == host_statistics(host, HOST_VM_INFO, (host_info_t)&vm_stats, &count)
    ) {
        used_mem = (
            (int64_t)vm_stats.active_count +
            (int64_t)vm_stats.inactive_count +
            (int64_t)vm_stats.wire_count
        ) * (int64_t)page_size;
    } else {
        return -1;
    }

    // Passing to MB
    total_mem /= 1024 * 1024;
    used_mem /= 1024 * 1024;

    *mem_info = (mem_info_t) {
        .total_mem = total_mem,
        .used_mem = used_mem
    };

    return 0;
}