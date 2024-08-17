#include "../../include/inc.h"
#include "../../include/6502-types.h"
#include "../../include/stack-manip.h"
#include "../mem.h"

void DEC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    u_byte address;
    switch (addressingMode) {
        case ABSOLUTE: {
            address = f_stack_pull_16(cpu);
            break;
        }
        case X_INDEXED_ABSOLUTE: {
            address = f_stack_pull_16(cpu) + cpu->X;
            break;
        }
        case ZERO_PAGE: {
            address = f_stack_pull(cpu);
            break;
        }
        case X_INDEXED_ZERO_PAGE: {
            address = f_stack_pull(cpu) + cpu->X;
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
    PUT_MEM_ADDR(cpu, address, GET_MEM_ADDR(cpu, address) - 1);
}

void DEX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMPLIED: {
            cpu->X--;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
        }
    }
}

void DEY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMPLIED: {
            cpu->Y--;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
        }
    }
}

void INC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    u_byte address;
    switch (addressingMode) {
        case ABSOLUTE: {
            address = f_stack_pull_16(cpu);
            break;
        }
        case X_INDEXED_ABSOLUTE: {
            address = f_stack_pull_16(cpu) + cpu->X;
            break;
        }
        case ZERO_PAGE: {
            address = f_stack_pull(cpu);
            break;
        }
        case X_INDEXED_ZERO_PAGE: {
            address = f_stack_pull(cpu) + cpu->X;
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
    PUT_MEM_ADDR(cpu, address, GET_MEM_ADDR(cpu, address) + 1);
}

void INX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMPLIED: {
            cpu->X++;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
        }
    }
}

void INY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMPLIED: {
            cpu->Y++;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
        }
    }
}