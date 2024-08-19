#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

// #include "config-file-handling.h"
#include "include/6502.h"
#include "instruction-table.c"
#include "cpu/init.h"

int main() {
    
    // FILE* config = resolve_config_file();
    cpu* main;
    _6502_prepopulate_values(main);

    
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