#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

// #include "config-file-handling.h"
#include "include/string-split.h"
#include "include/6502-types.h"
#include "include/6502.h"
#include "cpu/init.h"

extern bool get_yes_no_response(char* question, bool default_value);

extern noreturn void assemble(int argc, char** argv);
extern bool cli(int argc, char** argv);

bool cli_mode = false;

int main(int argc, char** argv) {
    for (int i = 0; i < argc; i++) {
        for (int j = 0; j < strlen(argv[i]); i++) {
            argv[i][j] = tolower(argv[i][j]);
        }
    }
    
    if (argc < 2)
        cli_mode = true;
    else if (argc > 2 && strcmp(argv[1], "assemble"))
        assemble(argc - 1, &argv[1]);

    _6502_prepopulate_values(main_cpu);
    _6502_calculate_nanoseconds_per_cycle(main_cpu);
    if (get_yes_no_response("Would you like to read an external file into memory?", false) == 0)
        _6502_mount_external_file(main_cpu);
    main_cpu->PC = RESET_VECTOR;
    _6502_start_cpu(main_cpu);
    
    while (cli_mode == true) {
        char input[30];
        printf("> ");
        scanf("%s", input);
        const char** result = split_string(input, " ");
        cli(count_num_entries(result), (char**)result);
        free_split_result(result);
        printf("\n");
    }
    _6502_un_start_cpu();
    syscall(SYS_exit, 0);
}