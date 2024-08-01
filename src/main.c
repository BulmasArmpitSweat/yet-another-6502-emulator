#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "include/6502.h"
#include "config.h"
#include "instruction-table.c"

int main() {
    cpu* main;
    memset(main, 0, sizeof(&main));

    /* Allocate memory block and registers */
    /* 6502 could access up to 65535 bytes (64K) of memory */
    /* This is not the 6510, shut the fuck up */
    main->mem = calloc(MAX_MEM_LEN, 1);
    main->f_stack = calloc(MAX_FUNCTION_STACK_LEN, sizeof(byte_raw));

    if (main->mem == NULL) return -1;
    main->PC = main->mem[RESET_VECTOR];
    
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