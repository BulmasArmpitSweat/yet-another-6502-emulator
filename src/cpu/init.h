#ifndef JFJKJ_DJSJ3N_DJ3UN_38UMNVII
#define JFJKJ_DJSJ3N_DJ3UN_38UMNVII

#include "../include/6502-types.h"

#define CONFIG_FILE_PATH "./config.toml"

static inline void _6502_start_cpu(cpu* cpu) {
    cpu->PC = RESET_VECTOR;
}

static inline void _6502_prepopulate_values(cpu* cpu) {
    /* Basic registers */
    cpu->A = 0;
    cpu->PC = 0;
    cpu->SP = 0;
    cpu->SR = 0;
    cpu->X = 0;
    cpu->Y = 0;
    
    /* Function stack | For internal use only */
    cpu->FSP = 0;
    cpu->f_stack = (byte_raw* )calloc(MAX_FUNCTION_STACK_LEN, sizeof(byte_raw));
    (cpu->f_stack == NULL) ? FATAL_ERROR(ERR_MEMORY_INITIALIZATION_FAILED):((void)0);

    /* For when we actually have a software tty using SDL */
    cpu->quit = false;

    /* Will probably be removed at a later date */
    cpu->last_accessed_memory_location = 0;
    cpu->microsecondsPerCycle = 0;
    
    /* VV Fuck you I don't care that this is obselete code VV */
    cpu->mem = (byte_raw *)malloc(MAX_MEM_LEN * sizeof(byte_raw));
    (cpu->mem == NULL) ? FATAL_ERROR(ERR_MEMORY_INITIALIZATION_FAILED):((void)0);
}

static inline void _6502_set_reset_vec(cpu* cpu, byte_raw lo, byte_raw hi) {
    cpu->mem[RESET_VECTOR] = lo;
    cpu->mem[RESET_VECTOR+1] = hi;
}

static inline void _6502_set_brk_vec(cpu* cpu, byte_raw lo, byte_raw hi) {
    cpu->mem[MAX_MEM_LEN-1] = lo;
    cpu->mem[MAX_MEM_LEN] = hi;
}

#endif /* JFJKJ_DJSJ3N_DJ3UN_38UMNVII */