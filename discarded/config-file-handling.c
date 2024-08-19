// #include "config-file-handling.h"
// #include "cpu/init.h"
// 
// #include <stdarg.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <stdbool.h>
// 
// config_file resolve_config_file() {
//     config_file config;
//     // config.config_file_location = CONFIG_FILE_PATH;
//     if (access(config.config_file_location, F_OK) != 0) {
//         fopen(config.config_file_location, "w");
//         fclose(config.config_file);
//     }
//     return config;
// }
// 
// void get_config_param(config_file config, ...) {
//     va_list va;
//     va_start(va, config);
// }