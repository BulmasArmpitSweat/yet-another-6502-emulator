#include "../../include/nop.h"
#include "../../include/stack-manip.h"
#include <stdio.h>

void NOP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    FILE* null = fopen("/dev/null", "w");    
    switch (addressingMode) {
        case IMPLIED: {
            break;
        }
        case IMMEDIATE: {
            fputc(f_stack_pull(cpu), null);
            break;
        }
        case ABSOLUTE: {
            ushort result = f_stack_pull_16(cpu);
            byte_raw lo = (byte_raw)result, hi = (byte_raw)(result >> 8);
            fputc(lo, null);
            fputc(hi, null);
            break;
        }
        case X_INDEXED_ABSOLUTE: {
            ushort result = f_stack_pull_16(cpu) + cpu->X;
            byte_raw lo = (byte_raw)result, hi = (byte_raw)(result >> 8);
            fputc(lo, null);
            fputc(hi, null);
            break;
        }
        case ZERO_PAGE: {
            fputc(f_stack_pull(cpu), null);
            break;
        }
        case X_INDEXED_ZERO_PAGE: {
            fputc(f_stack_pull(cpu) + cpu->X, null);
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
        }
    }
    // Implement when cycle system is working
    // NOTE: I might not actually have to add anything, since handling the cycles it takes up will be external. I just need to extract the required data from the function stack, and discard it.
    // NOTE 2: I won't have to implement it
}