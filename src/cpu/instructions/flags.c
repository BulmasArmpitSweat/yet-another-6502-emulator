#include "../../include/flags.h"
#include "../flags.h"

void CLC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    if (addressingMode != IMPLIED) FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
    reset_flag(cpu, CARRY);
}

void CLD(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    if (addressingMode != IMPLIED) FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
    reset_flag(cpu, DECIMAL_MODE);
}

void CLI(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    if (addressingMode != IMPLIED) FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
    reset_flag(cpu, INTERRUPT_DISABLE);
}

void CLV(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    if (addressingMode != IMPLIED) FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
    reset_flag(cpu, OVERFLOW);
}

void SEC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    if (addressingMode != IMPLIED) FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
    set_flag(cpu, CARRY);
}

void SED(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    if (addressingMode != IMPLIED) FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
    set_flag(cpu, DECIMAL_MODE);
}

void SEI(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    if (addressingMode != IMPLIED) FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
    set_flag(cpu, INTERRUPT_DISABLE);
}