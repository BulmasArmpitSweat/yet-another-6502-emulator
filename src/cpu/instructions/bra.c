#include "../../include/bra.h"
#include "../../include/6502-types.h"
#include "../../include/stack-manip.h"
#include "../flags.h"

void BCC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    if (addressingMode != RELATIVE) FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
    (test_flag(cpu, CARRY)) ? cpu->PC = (byte_raw)f_stack_pull_16(cpu) : ((void)0);
}

void BCS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    if (addressingMode != RELATIVE) FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
    (!test_flag(cpu, CARRY)) ? cpu->PC = (byte_raw)f_stack_pull_16(cpu) : ((void)0);
}

void BEQ(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    if (addressingMode != RELATIVE) FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
    (test_flag(cpu, ZERO)) ? cpu->PC = (byte_raw)f_stack_pull_16(cpu) : ((void)0);
}

void BNE(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    if (addressingMode != RELATIVE) FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
    (!test_flag(cpu, ZERO)) ? cpu->PC = (byte_raw)f_stack_pull_16(cpu) : ((void)0);
}

void BMI(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    if (addressingMode != RELATIVE) FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
    (test_flag(cpu, NEGATIVE)) ? cpu->PC = (byte_raw)f_stack_pull_16(cpu) : ((void)0);
}

void BPL(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    if (addressingMode != RELATIVE) FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
    (!test_flag(cpu, NEGATIVE)) ? cpu->PC = (byte_raw)f_stack_pull_16(cpu) : ((void)0);
}

void BVC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    if (addressingMode != RELATIVE) FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
    (test_flag(cpu, OVERFLOW)) ? cpu->PC = (byte_raw)f_stack_pull_16(cpu) : ((void)0);
}

void BVS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    if (addressingMode != RELATIVE) FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
    (!test_flag(cpu, OVERFLOW)) ? cpu->PC = (byte_raw)f_stack_pull_16(cpu) : ((void)0);
}