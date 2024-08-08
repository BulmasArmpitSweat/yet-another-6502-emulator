#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "include/6502.h"
#include "config.h"
#include "instruction-table.c"
#include "cpu/init.h"

int main() {
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