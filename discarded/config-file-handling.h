#ifndef ZDMFKDDK_DKEJFDK_09FDKJDKF
#define ZDMFKDDK_DKEJFDK_09FDKJDKF

#include "include/6502-types.h"
#include <stdint.h>
#include <stdio.h>

typedef struct config_file {
    FILE* config_file;
    char* config_file_location;
    uint32_t nanoseconds_per_cycle;
} config_file;

config_file resolve_config_file(void);
void get_config_param(config_file config, ...);
void set_config_param(config_file config, ...);


#endif /* ZDMFKDDK_DKEJFDK_09FDKJDKF */