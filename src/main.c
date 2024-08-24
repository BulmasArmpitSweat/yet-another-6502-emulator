#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>

// #include "config-file-handling.h"
#include "include/6502.h"
#include "cpu/init.h"
#include "cpu/mem.h"

int main() {
    char hertz_level;
    // FILE* config = resolve_config_file();
    cpu* main;
    _6502_prepopulate_values(main);
    _6502_calculate_nanoseconds_per_cycle(main);
    char confirm;
    while (true) {
        printf("Would you like to read an external file into memory? (default : no): ");
        scanf("%c", &confirm);
        confirm = tolower(confirm);
        char tests[3] = {'n', 'y', '\n'};
        if (test_char(confirm, tests) == false) {
            printf("That's not a valid input, kiddo\n");
            continue;
        }
        if (confirm == 'n' || confirm == '\n')
            break;
    }
    _6502_start_cpu(main);

    free(main->f_stack);
    free(main->mem);
    return 0;
}