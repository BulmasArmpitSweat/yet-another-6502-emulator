#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/syscall.h>
#include <unistd.h>

// #include "config-file-handling.h"
#include "cli/menu.h"
#include "include/6502-types.h"
#include "include/6502.h"
#include "cpu/init.h"

int main() {
    cpu* main;
    _6502_prepopulate_values(main);
    _6502_calculate_nanoseconds_per_cycle(main);
    if (get_yes_no_response("Would you like to read an external file into memory?", false) == 0)
        _6502_mount_external_file(main);
    main->PC = RESET_VECTOR;
    _6502_start_cpu(main);
    while (true) {
        char input[30];
        scanf("%s", input);
        if (resolve_cli_input(input, main_cpu) == 127)
            break;
    }
    
    _6502_un_start_cpu();
    syscall(SYS_exit, 0);
}