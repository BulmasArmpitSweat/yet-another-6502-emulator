#include "../../include/6502-types.h"
#include "../../include/arith.h"
#include "../mem.h"
#include "../flags.h"
#include "../../include/stack-manip.h"

void ADC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    u_byte operand;
    ushort result;
    u_byte carry = get_flag(cpu, CARRY);

    switch (addressingMode) {
        case IMMEDIATE: {
            operand = f_stack_pull(cpu);
            break;
        }
        case ABSOLUTE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull_16(cpu));
            break;
        }
        case X_INDEXED_ABSOLUTE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull_16(cpu) + cpu->X);
            break;
        }
        case Y_INDEXED_ABSOLUTE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull_16(cpu) + cpu->Y);
            break;
        }
        case ZERO_PAGE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull(cpu));
            break;
        }
        case X_INDEXED_ZERO_PAGE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull(cpu) + cpu->X);
            break;
        }
        case X_INDEXED_ZERO_PAGE_INDIRECT: {
            operand = GET_MEM_ADDR(cpu, GET_MEM_ADDR(cpu, f_stack_pull(cpu) + cpu->X));
            break;
        }
        case ZERO_PAGE_INDIRECT_Y_INDEXED: {
            operand = GET_MEM_ADDR(cpu, GET_MEM_ADDR(cpu, f_stack_pull(cpu)) + cpu->Y);
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return; // Ensure we don't fall through
        }
    }

    /* Add the components */
    result = cpu->A + operand + carry;

    /* Set the Carry flag based on the result */
    (result > 0xFF) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);

    /* Set the Zero flag based on the result */
    (result & 0xFF) ? reset_flag(cpu, ZERO) : set_flag(cpu, ZERO);

    /* Set the Overflow flag based on the result */
    (((cpu->A ^ result) & 0x80) && !((cpu->A ^ operand) & 0x80)) ? set_flag(cpu, OVERFLOW) : reset_flag(cpu, OVERFLOW);

    /* Set the Negative flag based on the result */
    (result & 0x80) ? set_flag(cpu, NEGATIVE) : reset_flag(cpu, NEGATIVE);

    /* Store the result in the accumulator */
    cpu->A = result & 0xFF; /* Keep only the lower 8 bits */
}

void CMP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    byte_raw operand;
    switch (addressingMode) {
        case IMMEDIATE: {
            operand = f_stack_pull(cpu);
            break;
        }
        case ABSOLUTE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull_16(cpu));
            break;
        }
        case X_INDEXED_ABSOLUTE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull_16(cpu) + cpu->X);
            break;
        }
        case Y_INDEXED_ABSOLUTE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull_16(cpu) + cpu->Y);
            break;
        }
        case ZERO_PAGE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull(cpu));
            break;
        }
        case X_INDEXED_ZERO_PAGE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull(cpu) + cpu->X);
            break;
        }
        case X_INDEXED_ZERO_PAGE_INDIRECT: {
            operand = GET_MEM_ADDR(cpu, GET_MEM_ADDR(cpu, f_stack_pull(cpu) + cpu->X));
            break;
        }
        case ZERO_PAGE_INDIRECT_Y_INDEXED: {
            operand = GET_MEM_ADDR(cpu, GET_MEM_ADDR(cpu, f_stack_pull(cpu)) + cpu->Y);
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return; // Ensure we don't fall through
        }
    }
    resolve_flags_NZ(cpu, cpu->A - operand);
    (operand <= cpu->A) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
}

void CPX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    byte_raw operand;
    switch (addressingMode) {
        case IMMEDIATE: {
            operand = f_stack_pull(cpu);
            break;
        }
        case ABSOLUTE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull_16(cpu));
            break;
        }
        case X_INDEXED_ABSOLUTE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull_16(cpu) + cpu->X);
            break;
        }
        case Y_INDEXED_ABSOLUTE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull_16(cpu) + cpu->Y);
            break;
        }
        case ZERO_PAGE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull(cpu));
            break;
        }
        case X_INDEXED_ZERO_PAGE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull(cpu) + cpu->X);
            break;
        }
        case X_INDEXED_ZERO_PAGE_INDIRECT: {
            operand = GET_MEM_ADDR(cpu, GET_MEM_ADDR(cpu, f_stack_pull(cpu) + cpu->X));
            break;
        }
        case ZERO_PAGE_INDIRECT_Y_INDEXED: {
            operand = GET_MEM_ADDR(cpu, GET_MEM_ADDR(cpu, f_stack_pull(cpu)) + cpu->Y);
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return; // Ensure we don't fall through
        }
    }
    resolve_flags_NZ(cpu, cpu->X - operand);
    (operand <= cpu->X) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
}

void CPY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    byte_raw operand;
    switch (addressingMode) {
        case IMMEDIATE: {
            operand = f_stack_pull(cpu);
            break;
        }
        case ABSOLUTE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull_16(cpu));
            break;
        }
        case X_INDEXED_ABSOLUTE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull_16(cpu) + cpu->X);
            break;
        }
        case Y_INDEXED_ABSOLUTE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull_16(cpu) + cpu->Y);
            break;
        }
        case ZERO_PAGE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull(cpu));
            break;
        }
        case X_INDEXED_ZERO_PAGE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull(cpu) + cpu->X);
            break;
        }
        case X_INDEXED_ZERO_PAGE_INDIRECT: {
            operand = GET_MEM_ADDR(cpu, GET_MEM_ADDR(cpu, f_stack_pull(cpu) + cpu->X));
            break;
        }
        case ZERO_PAGE_INDIRECT_Y_INDEXED: {
            operand = GET_MEM_ADDR(cpu, GET_MEM_ADDR(cpu, f_stack_pull(cpu)) + cpu->Y);
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return; // Ensure we don't fall through
        }
    }
    resolve_flags_NZ(cpu, cpu->Y - operand);
    (operand <= cpu->Y) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
}

void SBC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    u_byte operand;
    ushort result;
    u_byte carry = get_flag(cpu, CARRY);

    switch (addressingMode) {
        case IMMEDIATE: {
            operand = f_stack_pull(cpu);
            break;
        }
        case ABSOLUTE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull_16(cpu));
            break;
        }
        case X_INDEXED_ABSOLUTE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull_16(cpu) + cpu->X);
            break;
        }
        case Y_INDEXED_ABSOLUTE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull_16(cpu) + cpu->Y);
            break;
        }
        case ZERO_PAGE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull(cpu));
            break;
        }
        case X_INDEXED_ZERO_PAGE: {
            operand = GET_MEM_ADDR(cpu, f_stack_pull(cpu) + cpu->X);
            break;
        }
        case X_INDEXED_ZERO_PAGE_INDIRECT: {
            operand = GET_MEM_ADDR(cpu, GET_MEM_ADDR(cpu, f_stack_pull(cpu) + cpu->X));
            break;
        }
        case ZERO_PAGE_INDIRECT_Y_INDEXED: {
            operand = GET_MEM_ADDR(cpu, GET_MEM_ADDR(cpu, f_stack_pull(cpu)) + cpu->Y);
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return; // Ensure we don't fall through
        }
    }

    /* Calculate the result */
    result = cpu->A - operand - (1 - carry);

    /* Set the Carry flag if there was no borrow */
    (cpu->A >= operand + (1 - carry)) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);

    /* Set the Zero flag if the result is zero */
    (result & 0xFF) ? reset_flag(cpu, ZERO) : set_flag(cpu, ZERO);

    /* Set the Overflow flag if there's a signed overflow */
    (((cpu->A ^ result) & 0x80) && ((cpu->A ^ operand) & 0x80)) ? set_flag(cpu, OVERFLOW) : reset_flag(cpu, OVERFLOW);

    /* Set the Negative flag based on the result */
    (result & 0x80) ? set_flag(cpu, NEGATIVE) : reset_flag(cpu, NEGATIVE);

    /* Store the result in the accumulator */
    cpu->A = result & 0xFF; /* Keep only the lower 8 bits */
}

#if defined(INCLUDE_ILLEGAL_OPCODES)

void ANC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    byte_raw operand;
    switch (addressingMode)
    {
    case IMMEDIATE:
        operand = f_stack_pull(cpu);
        break;
    
    default:
        FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
        return;
    }

    cpu->A &= operand;
    resolve_flags_NZ(cpu, cpu->A);
    (cpu->A & 0x80) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
}

void ARR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    byte_raw operand, original;
    switch (addressingMode)
    {
    case IMMEDIATE:
        original = operand = f_stack_pull(cpu);
        break;
    default:
        FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
        return;
    }

    cpu->A = (cpu->A & operand) >> 1;
    resolve_flags_NZ(cpu, cpu->A);

    (test_bit(cpu->SR, DECIMAL_MODE) && test_bit(cpu, cpu->A) != test_bit(cpu, original)) ? set_flag(cpu, OVERFLOW) : reset_flag(cpu, OVERFLOW);
    (test_bit(cpu->SR, DECIMAL_MODE) && (original & 0xF0) + (original & 0x10) > 0x50) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);

    (!test_bit(cpu->SR, DECIMAL_MODE) && test_bit(cpu->A, 6) != test_bit(cpu->A, 5)) ? set_flag(cpu, OVERFLOW) : reset_flag(cpu, OVERFLOW);
    (!test_bit(cpu->SR, DECIMAL_MODE) && test_bit(cpu->A, 6)) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
}
#endif