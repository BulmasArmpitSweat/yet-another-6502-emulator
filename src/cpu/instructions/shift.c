#include "../../include/6502.h"
#include "../flags.h"
#include "../mem.h"

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
        }
    }
}

void ROL(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case ACCUMULATOR: {
            bit C;
            C.x = (test_bit(cpu->SR, CARRY)) ? 0 : 1;
            (cpu->A & 0x80) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
            cpu->A <<= 1;
            (C.x == 1) ? set_bit(cpu->A, 0) : reset_bit(cpu->A, 0);
            resolve_flags_NZ(cpu, cpu->A);
        }
        case ABSOLUTE: {
            ushort addr = f_stack_pull_16(cpu);
            bit C;
            C.x = (test_bit(cpu->SR, CARRY)) ? 0 : 1;
            (GET_MEM_ADDR(cpu, addr) & 0x80) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
            PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) << 1);
            (C.x == 1) ? set_bit(cpu->mem[addr], 0) : reset_flag(cpu, 0);
            resolve_flags_NZ(cpu, cpu->mem[addr]);
        }
        case X_INDEXED_ABSOLUTE: {
            ushort addr = f_stack_pull_16(cpu) + cpu->X;
            bit C;
            C.x = (test_bit(cpu->SR, CARRY)) ? 0 : 1;
            (GET_MEM_ADDR(cpu, addr) & 0x80) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
            PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) << 1);
            (C.x == 1) ? set_bit(cpu->mem[addr], 0) : reset_flag(cpu, 0);
            resolve_flags_NZ(cpu, cpu->mem[addr]);
        }
        case ZERO_PAGE: {
            ushort addr = f_stack_pull(cpu);
            bit C;
            C.x = (test_bit(cpu->SR, CARRY)) ? 0 : 1;
            (GET_MEM_ADDR(cpu, addr) & 0x80) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
            PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) << 1);
            (C.x == 1) ? set_bit(cpu->mem[addr], 0) : reset_flag(cpu, 0);
            resolve_flags_NZ(cpu, cpu->mem[addr]);
        }
        case X_INDEXED_ZERO_PAGE: {
            ushort addr = f_stack_pull(cpu) + cpu->X;
            bit C;
            C.x = (test_bit(cpu->SR, CARRY)) ? 0 : 1;
            (GET_MEM_ADDR(cpu, addr) & 0x80) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
            PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) << 1);
            (C.x == 1) ? set_bit(cpu->mem[addr], 0) : reset_flag(cpu, 0);
            resolve_flags_NZ(cpu, cpu->mem[addr]);
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
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
    }
}

#else

void ROR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case ACCUMULATOR: {
            bit C;
            C.x = (test_bit(cpu->SR, CARRY)) ? 0 : 1;
            (cpu->A & 0x01) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
            cpu->A >>= 1;
            (C.x == 1) ? set_bit(cpu->A, 7) : reset_bit(cpu->A, 7);
            resolve_flags_NZ(cpu, cpu->A);
        }
        case ABSOLUTE: {
            ushort addr = f_stack_pull_16(cpu);
            bit C;
            C.x = (test_bit(cpu->SR, CARRY)) ? 0 : 1;
            (GET_MEM_ADDR(cpu, addr) & 0x01) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
            PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) >> 1);
            (C.x == 1) ? set_bit(cpu->mem[addr], 7) : reset_flag(cpu, 7);
            resolve_flags_NZ(cpu, cpu->mem[addr]);
        }
        case X_INDEXED_ABSOLUTE: {
            ushort addr = f_stack_pull_16(cpu) + cpu->X;
            bit C;
            C.x = (test_bit(cpu->SR, CARRY)) ? 0 : 1;
            (GET_MEM_ADDR(cpu, addr) & 0x01) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
            PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) >> 1);
            (C.x == 1) ? set_bit(cpu->mem[addr], 7) : reset_flag(cpu, 7);
            resolve_flags_NZ(cpu, cpu->mem[addr]);
        }
        case ZERO_PAGE: {
            ushort addr = f_stack_pull(cpu);
            bit C;
            C.x = (test_bit(cpu->SR, CARRY)) ? 0 : 1;
            (GET_MEM_ADDR(cpu, addr) & 0x01) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
            PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) >> 1);
            (C.x == 1) ? set_bit(cpu->mem[addr], 7) : reset_flag(cpu, 7);
            resolve_flags_NZ(cpu, cpu->mem[addr]);
        }
        case X_INDEXED_ZERO_PAGE: {
            ushort addr = f_stack_pull(cpu) + cpu->X;
            bit C;
            C.x = (test_bit(cpu->SR, CARRY)) ? 0 : 1;
            (GET_MEM_ADDR(cpu, addr) & 0x01) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
            PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) >> 1);
            (C.x == 1) ? set_bit(cpu->mem[addr], 7) : reset_flag(cpu, 7);
            resolve_flags_NZ(cpu, cpu->mem[addr]);
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
        }
    }
}

#endif