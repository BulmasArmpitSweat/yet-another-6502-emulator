#include "../../include/stack.h"
#include "../../include/stack-manip.h"
#include "../../include/6502-types.h"
#include "../flags.h"

void PHA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMPLIED: {
            stack_push(cpu, cpu->A);
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
}


void PHP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMPLIED: {
            stack_push(cpu, cpu->SR);
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
}

void PLA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMPLIED: {
            cpu->A = stack_pull(cpu);
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
    resolve_flags_NZ(cpu, cpu->A);
}

void PLP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMPLIED: {
            cpu->SR = stack_pull(cpu);
            reset_flag(cpu, BRK_COMMAND);
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
}