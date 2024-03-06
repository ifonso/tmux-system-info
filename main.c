#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/mem_usage.h"
#include "src/cpu_usage.h"

char *get_cpu_tick(double percentage) {
    const char *ticks = "▁▂▃▄▅▆▇█";
    char *result = (char *)malloc(4);
    
    int ticks_count = strlen(ticks) / 3;
    int tick_pos = (ticks_count * (int)percentage) / 101;

    // sprintf(result, "#[fg=red]");
    strncat(result, &ticks[tick_pos * 3], 3);

    return result;
}

char *get_ram_formated_string(mem_info_t *mem_info) {
    char *result = (char *)malloc(40);

    double total_in_gb = (double)mem_info->total_mem / 1024;
    double used_in_gb = (double)mem_info->used_mem / 1024;
    // "#[fg=white]%.1f/#[fg=white]%.0f GB"
    sprintf(result, "%.1f/%.0fGB", used_in_gb, total_in_gb);

    return result;
}

int main(int argc, char ** argv) {
    #if !(defined(BSD_BASED) || (defined(__APPLE__) && defined(__MACH__)))
    perror("This program is only supported on BSD-based systems");
    exit(EXIT_FAILURE);
    #endif

    mem_info_t mem_info;
    double cpu_usage;

    if (get_mem_info(&mem_info) == -1) {
        perror("Failed to get memory info");
        exit(EXIT_FAILURE);
    }

    if (get_cpu_percent_usage(1, &cpu_usage) == -1) {
        perror("Failed to get CPU usage");
        exit(EXIT_FAILURE);
    }

    printf("%s %.2f%% %s", get_ram_formated_string(&mem_info), cpu_usage, get_cpu_tick(cpu_usage));

    return 0;
}
