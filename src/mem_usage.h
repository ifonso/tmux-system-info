#ifndef MEM_USAGE_H
#define MEM_USAGE_H

#include <unistd.h>

/* structure to store memory information
 *
 * - total_mem: Total memory in MB
 * - used_mem: Used memory in MB
**/
struct mem_info {
    size_t total_mem;
    size_t used_mem;
};

typedef struct mem_info mem_info_t;

int get_mem_info(mem_info_t *mem_info);

#endif // MEM_USAGE_H