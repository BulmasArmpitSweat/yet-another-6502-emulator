#include "../mem.h"
#include "../../include/6502.h"
#include "../flags.h"
#include "../../config.h"

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
                        GET_MEM_ADDR_ZP(cpu, 
                            f_stack_pull(cpu) + cpu->X));
            break;
        }
        case ZERO_PAGE_INDIRECT_Y_INDEXED: {
            cpu->A = GET_MEM_ADDR(cpu, 
                        GET_MEM_ADDR_ZP(cpu, 
                            f_stack_pull(cpu)) + cpu->X);
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
    resolve_flags_NZ(cpu, cpu->A);
}

void LDX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch(addressingMode) {
        case IMMEDIATE: {
            cpu->X = f_stack_pull(cpu);
            break;
        }
        case ABSOLUTE: {
            cpu->X = GET_MEM_ADDR(cpu, 
                        f_stack_pull_16(cpu));
            break;
        }
        case Y_INDEXED_ABSOLUTE: {
            cpu->X = GET_MEM_ADDR(cpu, 
                        f_stack_pull_16(cpu) + cpu->Y);
            break;
        }
        case ZERO_PAGE: {
            cpu->X = GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu));
        break;
        }
        case Y_INDEXED_ZERO_PAGE: {
            cpu->X = GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu) + cpu->Y);
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
    resolve_flags_NZ(cpu, cpu->X);
}

void LDY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch(addressingMode) {
        case IMMEDIATE: {
            cpu->Y = f_stack_pull(cpu);
            break;
        }
        case ABSOLUTE: {
            cpu->Y = GET_MEM_ADDR(cpu, 
                        f_stack_pull_16(cpu));
            break;
        }
        case X_INDEXED_ABSOLUTE: {
            cpu->Y = GET_MEM_ADDR(cpu, 
                        f_stack_pull_16(cpu) + cpu->X);
            break;
        }
        case ZERO_PAGE: {
            cpu->Y = GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu));
        break;
        }
        case X_INDEXED_ZERO_PAGE: {
            cpu->Y = GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu) + cpu->X);
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
    resolve_flags_NZ(cpu, cpu->Y);
}

void STA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case ABSOLUTE: {
            PUT_MEM_ADDR(cpu, 
                f_stack_pull_16(cpu), cpu->A);
                break;
        }
        case X_INDEXED_ABSOLUTE: {
            PUT_MEM_ADDR(cpu, 
                f_stack_pull_16(cpu) + cpu->X, cpu->A);
                break;
        }
        case Y_INDEXED_ABSOLUTE: {
            PUT_MEM_ADDR(cpu, 
                f_stack_pull_16(cpu) + cpu->Y, cpu->A);
                break;
        }
        case ZERO_PAGE: {
            PUT_MEM_ADDR(cpu, 
                f_stack_pull(cpu), cpu->A);
                break;
        }
        case X_INDEXED_ZERO_PAGE: {
            PUT_MEM_ADDR(cpu, 
                f_stack_pull(cpu) + cpu->X, cpu->A);
                break;
        }
        case X_INDEXED_ZERO_PAGE_INDIRECT: {
            PUT_MEM_ADDR(cpu, 
                GET_MEM_ADDR_ZP(cpu, 
                    f_stack_pull(cpu) + cpu->X), cpu->A);
        }
        case ZERO_PAGE_INDIRECT_Y_INDEXED: {
            PUT_MEM_ADDR(cpu,
                GET_MEM_ADDR_ZP(cpu, 
                    f_stack_pull(cpu)) + cpu->Y, cpu->A);
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
}

void STX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case ABSOLUTE: {
            PUT_MEM_ADDR(cpu, 
                f_stack_pull_16(cpu), cpu->X);
            break;
        }
        case ZERO_PAGE: {
            PUT_MEM_ADDR(cpu,
                f_stack_pull(cpu), cpu->X);
            break;
        }
        case Y_INDEXED_ZERO_PAGE: {
            PUT_MEM_ADDR(cpu,
                f_stack_pull(cpu) + cpu->Y, cpu->X);
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
}

void STY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case ABSOLUTE: {
            PUT_MEM_ADDR(cpu, 
                f_stack_pull_16(cpu), cpu->Y);
                break;
        }
        case ZERO_PAGE: {
            PUT_MEM_ADDR(cpu,
                f_stack_pull(cpu), cpu->Y);
            break;
        }
        case X_INDEXED_ZERO_PAGE: {
            PUT_MEM_ADDR(cpu,
                f_stack_pull(cpu) + cpu->X, cpu->Y);
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
}

#if defined (INCLUDE_ILLEGAL_OPCODES)

/* ###################################################### */
/* ################### Illegal opcodes ################## */
/* ###################################################### */

void LAS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case Y_INDEXED_ABSOLUTE: {
            cpu->A =
            cpu->X =
            cpu->SP = GET_MEM_ADDR(cpu, 
                          f_stack_pull_16(cpu) + cpu->Y);
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }    
    }
    resolve_flags_NZ(cpu, cpu->A);
}

void LAX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMMEDIATE: {
            cpu->A =
            cpu->X =
            f_stack_pull(cpu);
            break;
        }
        case ABSOLUTE: {
            cpu->A =
            cpu->X =
            GET_MEM_ADDR(cpu, 
                f_stack_pull_16(cpu));
            break;
        }
        case Y_INDEXED_ABSOLUTE: {
            cpu->A =
            cpu->X =
            GET_MEM_ADDR(cpu, 
                f_stack_pull_16(cpu) + cpu->Y);
            break;
        }
        case ZERO_PAGE: {
            cpu->A =
            cpu->X =
            GET_MEM_ADDR(cpu, 
                f_stack_pull(cpu));
            break;
        }
        case Y_INDEXED_ZERO_PAGE: {
            cpu->A =
            cpu->X =
            GET_MEM_ADDR(cpu, 
                f_stack_pull(cpu) + cpu->Y);
            break;
        }
        case X_INDEXED_ZERO_PAGE_INDIRECT: {
            cpu->A =
            cpu->X =
            GET_MEM_ADDR(cpu, 
                GET_MEM_ADDR_ZP(cpu, 
                    f_stack_pull(cpu) + cpu->X));
            break;
        }
        case ZERO_PAGE_INDIRECT_Y_INDEXED: {
            cpu->A =
            cpu->X =
            GET_MEM_ADDR(cpu, 
                GET_MEM_ADDR_ZP(cpu, 
                    f_stack_pull(cpu)) + cpu->Y);
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
    resolve_flags_NZ(cpu, cpu->A);
}

void SAX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case ABSOLUTE: {
            PUT_MEM_ADDR(cpu,
                f_stack_pull_16(cpu), cpu->A & cpu->X);
            break;
        }
        case ZERO_PAGE: {
            PUT_MEM_ADDR(cpu,
                f_stack_pull(cpu), cpu->A & cpu->X);
            break;
        }
        case Y_INDEXED_ZERO_PAGE: {
            PUT_MEM_ADDR(cpu,
                f_stack_pull(cpu) + cpu->Y, cpu->A & cpu->X);
            break;
        }
        case X_INDEXED_ZERO_PAGE_INDIRECT: {
            PUT_MEM_ADDR(cpu,
                GET_MEM_ADDR_ZP(cpu, 
                    f_stack_pull(cpu) + cpu->X), cpu->A & cpu->X);
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
}

void SHA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case Y_INDEXED_ABSOLUTE: {
            ushort addr = f_stack_pull_16(cpu);
            PUT_MEM_ADDR(cpu, addr + cpu->Y, cpu->A & cpu->X & ((byte_raw)addr >> 8) + 1);
        break;
        }
        case ZERO_PAGE_INDIRECT_Y_INDEXED: {
            byte_raw addr = f_stack_pull(cpu);
            PUT_MEM_ADDR(cpu, 
                GET_MEM_ADDR_ZP(cpu, addr), cpu->A & cpu->X & GET_MEM_ADDR(cpu, addr));
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
}

void SHX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case Y_INDEXED_ABSOLUTE: {
            ushort addr = f_stack_pull_16(cpu);
            PUT_MEM_ADDR(cpu, 
                addr + cpu->Y, cpu->Y & (byte_raw)(addr >> 8) + 1);
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
}

void SHY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case X_INDEXED_ABSOLUTE: {
            ushort addr = f_stack_pull_16(cpu);
            PUT_MEM_ADDR(cpu, 
                addr + cpu->Y, cpu->X & (byte_raw)(addr >> 8) + 1);
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
}

#endif