#include "config-file-handling.h"
#include "cpu/init.h"
#include "include/6502-types.h"
#include "../lib/tomlc99/toml.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

FILE* resolve_config_file() {
    FILE* config;
    if (access(CONFIG_FILE_PATH, F_OK) != 0) {
        fprintf(stderr, "WARNING: Config file doesn't exist. Would you like to create a new one? (default: yes): ");
        char* input;
        scanf("%s", input);
        for(int i = 0; input[i]; i++){
            input[i] = tolower(input[i]);
        }
        if (strcmp(input, "yes") || strcmp(input, "ye") || strcmp(input, "y") || strcmp(input, ""))
            config = fopen(CONFIG_FILE_PATH, "w");
    }
    return config;
}