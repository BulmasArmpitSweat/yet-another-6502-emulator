#include "../../include/6502.h"
#include "../flags.h"
#include "../mem.h"
#include "../rotate.h"

void ASL(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case ACCUMULATOR: {
            (cpu->A & 0x80) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
            cpu->A <<= 1;
            resolve_flags_NZ(cpu, cpu->A);
        }
        case ABSOLUTE: {
            ushort addr = f_stack_pull_16(cpu);
            (GET_MEM_ADDR(cpu, addr) & 0x80) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
            PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) << 1);
            resolve_flags_NZ(cpu, GET_MEM_ADDR(cpu, addr));
        }
        case X_INDEXED_ABSOLUTE: {
            ushort addr = f_stack_pull_16(cpu) + cpu->X;
            (GET_MEM_ADDR(cpu, addr) & 0x80) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
            PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) << 1);
            resolve_flags_NZ(cpu, GET_MEM_ADDR(cpu, addr));
        }
        case ZERO_PAGE: {
            ushort addr = f_stack_pull(cpu);
            (GET_MEM_ADDR(cpu, addr) & 0x80) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
            PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) << 1);
            resolve_flags_NZ(cpu, GET_MEM_ADDR(cpu, addr));
        }
        case X_INDEXED_ZERO_PAGE: {
            ushort addr = f_stack_pull(cpu) + cpu->X;
            (GET_MEM_ADDR(cpu, addr) & 0x80) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
            PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) << 1);
            resolve_flags_NZ(cpu, GET_MEM_ADDR(cpu, addr));
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
}

void LSR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case ACCUMULATOR: {
            (cpu->A & 0x01) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
            cpu->A >>= 1;
            resolve_flags_NZ(cpu, cpu->A);
        }
        case ABSOLUTE: {
            ushort addr = f_stack_pull_16(cpu);
            (GET_MEM_ADDR(cpu, addr) & 0x01) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
            PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) >> 1);
            resolve_flags_NZ(cpu, GET_MEM_ADDR(cpu, addr));
        }
        case X_INDEXED_ABSOLUTE: {
            ushort addr = f_stack_pull_16(cpu) + cpu->X;
            (GET_MEM_ADDR(cpu, addr) & 0x01) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
            PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) >> 1);
            resolve_flags_NZ(cpu, GET_MEM_ADDR(cpu, addr));
        }
        case ZERO_PAGE: {
            ushort addr = f_stack_pull(cpu);
            (GET_MEM_ADDR(cpu, addr) & 0x01) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
            PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) >> 1);
            resolve_flags_NZ(cpu, GET_MEM_ADDR(cpu, addr));
        }
        case X_INDEXED_ZERO_PAGE: {
            ushort addr = f_stack_pull(cpu) + cpu->X;
            (GET_MEM_ADDR(cpu, addr) & 0x01) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
            PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) >> 1);
            resolve_flags_NZ(cpu, GET_MEM_ADDR(cpu, addr));
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
}

void ROL(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case ACCUMULATOR: {
            l_rotate(cpu, cpu->A);
            resolve_flags_NZ(cpu, cpu->A);
        }
        case ABSOLUTE: {
            ushort addr = f_stack_pull_16(cpu);
            l_rotate(cpu, GET_MEM_ADDR(cpu, addr));
            resolve_flags_NZ(cpu, cpu->mem[addr]);
        }
        case X_INDEXED_ABSOLUTE: {
            ushort addr = f_stack_pull_16(cpu) + cpu->X;
            l_rotate(cpu, GET_MEM_ADDR(cpu, addr));
            resolve_flags_NZ(cpu, cpu->mem[addr]);
        }
        case ZERO_PAGE: {
            ushort addr = f_stack_pull(cpu);
            l_rotate(cpu, GET_MEM_ADDR(cpu, addr));
            resolve_flags_NZ(cpu, cpu->mem[addr]);
        }
        case X_INDEXED_ZERO_PAGE: {
            ushort addr = f_stack_pull(cpu) + cpu->X;
            l_rotate(cpu, GET_MEM_ADDR(cpu, addr));
            resolve_flags_NZ(cpu, cpu->mem[addr]);
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
}

#if defined (INCLUDE_ROR_BUG)

void ROR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case ACCUMULATOR: {
            cpu->A <<= 1;
        }
        case ABSOLUTE: {
            ushort addr = f_stack_pull_16(cpu);
            PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) << 1);
        }
        case X_INDEXED_ABSOLUTE: {
            ushort addr = f_stack_pull_16(cpu) + cpu->X;
            PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) << 1);
        }
        case ZERO_PAGE: {
            ushort addr = f_stack_pull(cpu);
            PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) << 1);
        }
        case X_INDEXED_ZERO_PAGE: {
            ushort addr = f_stack_pull(cpu) + cpu->X;
            PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) << 1);
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
}

#else

void ROR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case ACCUMULATOR: {
            r_rotate(cpu, cpu->A);
            resolve_flags_NZ(cpu, cpu->A);
        }
        case ABSOLUTE: {
            ushort addr = f_stack_pull_16(cpu);
            r_rotate(cpu, GET_MEM_ADDR(cpu, addr));
            resolve_flags_NZ(cpu, cpu->mem[addr]);
        }
        case X_INDEXED_ABSOLUTE: {
            ushort addr = f_stack_pull_16(cpu) + cpu->X;
            r_rotate(cpu, GET_MEM_ADDR(cpu, addr));
            resolve_flags_NZ(cpu, cpu->mem[addr]);
        }
        case ZERO_PAGE: {
            ushort addr = f_stack_pull(cpu);
            r_rotate(cpu, GET_MEM_ADDR(cpu, addr));
            resolve_flags_NZ(cpu, cpu->mem[addr]);
        }
        case X_INDEXED_ZERO_PAGE: {
            ushort addr = f_stack_pull(cpu) + cpu->X;
            r_rotate(cpu, GET_MEM_ADDR(cpu, addr));
            resolve_flags_NZ(cpu, cpu->mem[addr]);
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
}

#endif