#include "../../include/logic.h"
#include "../mem.h"
#include "../../include/stack-manip.h"
#include "../flags.h"

void AND(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMMEDIATE: {
            cpu->A &= f_stack_pull(cpu);
            break;
        }
        case ABSOLUTE: {
            cpu->A &= GET_MEM_ADDR(cpu, 
                        f_stack_pull_16(cpu));
            break;
        }
        case X_INDEXED_ABSOLUTE: {
            cpu->A &= GET_MEM_ADDR(cpu, 
                        f_stack_pull_16(cpu) + cpu->X);
            break;
        }
        case Y_INDEXED_ABSOLUTE: {
            cpu->A &= GET_MEM_ADDR(cpu, 
                        f_stack_pull_16(cpu) + cpu->Y);
            break;
        }
        case ZERO_PAGE: {
            cpu->A &= GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu));
            break;
        }
        case X_INDEXED_ZERO_PAGE: {
            cpu->A &= GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu) + cpu->X);
            break;
        }
        case X_INDEXED_ZERO_PAGE_INDIRECT: {
            cpu->A &= GET_MEM_ADDR(cpu, 
                        GET_MEM_ADDR(cpu, 
                            f_stack_pull(cpu) + cpu->X));
            break;
        }
        case ZERO_PAGE_INDIRECT_Y_INDEXED: {
            cpu->A &= GET_MEM_ADDR(cpu, 
                        GET_MEM_ADDR(cpu, 
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

void BIT(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    byte_raw operand;
    switch (addressingMode) {
        case ABSOLUTE: {
            operand = GET_MEM_ADDR(cpu, 
                        f_stack_pull_16(cpu));
            break;
        }
        case ZERO_PAGE: {
            operand = GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu));
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
    (test_bit(operand, 6)) ? set_flag(cpu, OVERFLOW) : reset_flag(cpu, OVERFLOW);
    (test_bit(operand, 7)) ? set_flag(cpu, NEGATIVE) : reset_flag(cpu, NEGATIVE);
    !(operand & cpu->A) ? set_flag(cpu, ZERO) : set_flag(cpu, ZERO);
}

void EOR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMMEDIATE: {
            cpu->A ^= f_stack_pull(cpu);
            break;
        }
        case ABSOLUTE: {
            cpu->A ^= GET_MEM_ADDR(cpu, 
                        f_stack_pull_16(cpu));
            break;
        }
        case X_INDEXED_ABSOLUTE: {
            cpu->A ^= GET_MEM_ADDR(cpu, 
                        f_stack_pull_16(cpu) + cpu->X);
            break;
        }
        case Y_INDEXED_ABSOLUTE: {
            cpu->A ^= GET_MEM_ADDR(cpu, 
                        f_stack_pull_16(cpu) + cpu->Y);
            break;
        }
        case ZERO_PAGE: {
            cpu->A ^= GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu));
            break;
        }
        case X_INDEXED_ZERO_PAGE: {
            cpu->A ^= GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu) + cpu->X);
            break;
        }
        case X_INDEXED_ZERO_PAGE_INDIRECT: {
            cpu->A ^= GET_MEM_ADDR(cpu, 
                        GET_MEM_ADDR(cpu, 
                            f_stack_pull(cpu) + cpu->X));
            break;
        }
        case ZERO_PAGE_INDIRECT_Y_INDEXED: {
            cpu->A ^= GET_MEM_ADDR(cpu, 
                        GET_MEM_ADDR(cpu, 
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

void ORA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMMEDIATE: {
            cpu->A |= f_stack_pull(cpu);
            break;
        }
        case ABSOLUTE: {
            cpu->A |= GET_MEM_ADDR(cpu, 
                        f_stack_pull_16(cpu));
            break;
        }
        case X_INDEXED_ABSOLUTE: {
            cpu->A |= GET_MEM_ADDR(cpu, 
                        f_stack_pull_16(cpu) + cpu->X);
            break;
        }
        case Y_INDEXED_ABSOLUTE: {
            cpu->A |= GET_MEM_ADDR(cpu, 
                        f_stack_pull_16(cpu) + cpu->Y);
            break;
        }
        case ZERO_PAGE: {
            cpu->A |= GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu));
            break;
        }
        case X_INDEXED_ZERO_PAGE: {
            cpu->A |= GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu) + cpu->X);
            break;
        }
        case X_INDEXED_ZERO_PAGE_INDIRECT: {
            cpu->A |= GET_MEM_ADDR(cpu, 
                        GET_MEM_ADDR(cpu, 
                            f_stack_pull(cpu) + cpu->X));
            break;
        }
        case ZERO_PAGE_INDIRECT_Y_INDEXED: {
            cpu->A |= GET_MEM_ADDR(cpu, 
                        GET_MEM_ADDR(cpu, 
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