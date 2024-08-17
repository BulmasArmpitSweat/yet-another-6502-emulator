#include "../../include/ctrl.h"
#include "../../include/6502-types.h"
#include "../../include/stack-manip.h"
#include "../mem.h"

void BRK(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMPLIED: {
            cpu->PC = BRK_VECTOR;
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
}

void JMP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    u_byte address;
    switch (addressingMode) {
        case ABSOLUTE: {
            address = f_stack_pull(cpu);
            break;
        }
        case ABSOLUTE_INDIRECT: {
            address = GET_MEM_ADDR(cpu, 
                        f_stack_pull_16(cpu));
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
    cpu->PC = GET_MEM_ADDR_16(cpu, address);
}

void JSR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    u_byte address;
    switch (addressingMode) {
        case ABSOLUTE: {
            address = f_stack_pull_16(cpu);
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
    stack_push_16(cpu, cpu->PC + 2);
    cpu->PC = GET_MEM_ADDR_16(cpu, address);
}

void RTI(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    if (addressingMode != IMPLIED) FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
    cpu->SR = stack_pull(cpu);
    cpu->PC = stack_pull_16(cpu);
}

void RTS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    if (addressingMode != IMPLIED) FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
    cpu->PC = stack_pull_16(cpu) + 1;
}