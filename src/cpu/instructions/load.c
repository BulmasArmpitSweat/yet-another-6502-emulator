#include "load.h"
#include "mem.h"

void LDA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMMEDIATE: {
            cpu->A = f_stack_pull(cpu);
            break;
        }
        case ABSOLUTE: {
            cpu->A = GET_MEM_ADDR(cpu, 
                        f_stack_pull_16(cpu));
            break;
        }
        case X_INDEXED_ABSOLUTE: {
            cpu->A = GET_MEM_ADDR(cpu, 
                        f_stack_pull_16(cpu) + cpu->X);
            break;
        }
        case Y_INDEXED_ABSOLUTE: {
            cpu->A = GET_MEM_ADDR(cpu, 
                        f_stack_pull_16(cpu) + cpu->Y);
            break;
        }
        case ZERO_PAGE: {
            cpu->A = GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu));
        break;
        }
        case X_INDEXED_ZERO_PAGE: {
            cpu->A = GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu) + cpu->X);
            break;
        }
        case X_INDEXED_ZERO_PAGE_INDIRECT: {
            cpu->A = GET_MEM_ADDR(cpu, 
                        GET_MEM_ADDR(cpu, 
                            f_stack_pull(cpu) + cpu->X));
            break;
        }
    }
}