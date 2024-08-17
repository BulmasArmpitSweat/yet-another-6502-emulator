#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "include/6502.h"
#include "config.h"
#include "instruction-table.c"
#include "cpu/init.h"

int main() {
    if (access(CONFIG_FILE_PATH, F_OK) != 0) {
        fprintf(stderr, "WARNING: Config file doesn't exist. Would you like to create a new one? (default: yes): ");
        bool create_new_conf_file;
        char* input;
        scanf("%s", input);
        for(int i = 0; input[i]; i++){
            input[i] = tolower(input[i]);
        }
        if (strcmp(input, "yes") || strcmp(input, "ye") || strcmp(input, "y") || strcmp(input, ""))
            create_new_conf_file = true;
        else if (strcmp(input, "no") || strcmp(input, "n"))
            create_new_conf_file = false;
    }

    FILE* config = fopen(CONFIG_FILE_PATH, "rw");
    cpu* main;
    _6502_prepopulate_values(main);
    /*
    TODO:
    _6502_set_reset_vec(main, ...);
    _6502_set_brk_vec(main, ...);
    _6502_start_cpu(main)
    */
    
    instructionTable[main->mem[main->PC]].InstructionPointer
    (instructionTable[main->mem[main->PC]].mode, 
     instructionTable[main->mem[main->PC]].cycles, 
     main, 
     instructionTable[main->mem[main->PC]].page_crossed_cycle_exception);
    // TODO: Do shit

    free(main->f_stack);
    free(main->mem);
    return 0;
}