#include "../../include/6502.h"
#include "../flags.h"
#include "../../include/stack-manip.h"
#include "../mem.h"
#include "../../config.h"

void TAX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMPLIED: {
            cpu->X = cpu->A;
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
        }
    }
    resolve_flags_NZ(cpu, cpu->X);
}

void TAY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMPLIED: {
            cpu->Y = cpu->A;
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
        }
    }
    resolve_flags_NZ(cpu, cpu->Y);
}

void TSX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMPLIED: {
            cpu->X = cpu->SP;
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
        }
    }
    resolve_flags_NZ(cpu, cpu->X);
}

void TXA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMPLIED: {
            cpu->A = cpu->X;
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
        }
    }
    resolve_flags_NZ(cpu, cpu->A);
}

void TXS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMPLIED: {
            cpu->SP = cpu->X;
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
        }
    }
}

void TYA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMPLIED: {
            cpu->A = cpu->Y;
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
        }
    }
    resolve_flags_NZ(cpu, cpu->A);
}

#if defined (INCLUDE_ILLEGAL_OPCODES)

/* ###################################################### */
/* ################### Illegal opcodes ################## */
/* ###################################################### */

void SHS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case Y_INDEXED_ABSOLUTE: {
            ushort addr = f_stack_pull_16(cpu);
            cpu->SP = (cpu->A & cpu->X);
            PUT_MEM_ADDR(cpu, addr + cpu->Y, cpu->SP & (byte_raw)(addr >> 8) + 1);
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
        }
    }
}

#endif