#include "../../include/6502-types.h"
#include "../../include/arith.h"
#include "../mem.h"
#include "../flags.h"
#include "../../include/stack-manip.h"
#include "../rotate.h"
#include "../split.h"
#include <stdio.h>

void ADC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    byte_raw operand;
    ushort result;
    byte_raw carry = test_flag(cpu, CARRY);

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
    byte_raw operand;
    ushort result;
    byte_raw carry = test_flag(cpu, CARRY);

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
        r_rotate(cpu, operand);
        break;
    default:
        FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
        return;
    }

    cpu->A = (cpu->A & operand) >> 1;
    resolve_flags_NZ(cpu, cpu->A);

    (test_flag(cpu, DECIMAL_MODE) && test_bit(cpu->A, 6) != test_bit(original, 6)) ? set_flag(cpu, OVERFLOW) : reset_flag(cpu, OVERFLOW);

    (test_flag(cpu, DECIMAL_MODE) && (original & 0xF0) + (original & 0x10) > 0x50) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);

    (!test_flag(cpu, DECIMAL_MODE) && test_bit(cpu->A, 6) != test_bit(cpu->A, 5)) ? set_flag(cpu, OVERFLOW) : reset_flag(cpu, OVERFLOW);
    (!test_flag(cpu, DECIMAL_MODE) && test_bit(cpu->A, 6)) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
}

void ASR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMMEDIATE: {
            cpu->A &= f_stack_pull(cpu);
            set_flag_on_bit(cpu, CARRY, cpu->A, 0);
            cpu->A >>= 1;
            (cpu->A == 0) ? set_flag(cpu, ZERO) : reset_flag(cpu, ZERO);
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }

}

void DCP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    byte_raw addr;
    switch (addressingMode) {
        case ABSOLUTE: {
            addr = f_stack_pull_16(cpu);
            break;
        }
        case X_INDEXED_ABSOLUTE: {
            addr = f_stack_pull_16(cpu) + cpu->X;
            break;
        }
        case Y_INDEXED_ABSOLUTE: {
            addr = f_stack_pull_16(cpu) + cpu->Y;
            break;
        }
        case ZERO_PAGE: {
            addr = f_stack_pull(cpu);
            break;
        }
        case X_INDEXED_ZERO_PAGE: {
            addr = f_stack_pull(cpu) + cpu->X;
            break;
        }
        case X_INDEXED_ZERO_PAGE_INDIRECT: {
            addr = GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu)) + cpu->X;
            break;
        }
        case ZERO_PAGE_INDIRECT_Y_INDEXED: {
            addr = GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu) + cpu->Y);
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
    PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) - 1);
    resolve_flags_NZ(cpu, cpu->A - GET_MEM_ADDR(cpu, addr));
    (GET_MEM_ADDR(cpu, addr) <= cpu->A) ? set_flag(cpu, CARRY) : set_flag(cpu, CARRY);
}

void ISC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    byte_raw addr;
    switch (addressingMode) {
        case ABSOLUTE: {
            addr = f_stack_pull_16(cpu);
            break;
        }
        case X_INDEXED_ABSOLUTE: {
            addr = f_stack_pull_16(cpu) + cpu->X;
            break;
        }
        case Y_INDEXED_ABSOLUTE: {
            addr = f_stack_pull_16(cpu) + cpu->Y;
            break;
        }
        case ZERO_PAGE: {
            addr = f_stack_pull(cpu);
            break;
        }
        case X_INDEXED_ZERO_PAGE: {
            addr = f_stack_pull(cpu) + cpu->X;
            break;
        }
        case X_INDEXED_ZERO_PAGE_INDIRECT: {
            addr = GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu)) + cpu->X;
            break;
        }
        case ZERO_PAGE_INDIRECT_Y_INDEXED: {
            addr = GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu) + cpu->Y);
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
    PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) + 1);
    cpu->A = cpu->A + (~GET_MEM_ADDR(cpu, addr) + 1);
    ((byte_raw)cpu->A < 0) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
    ((byte_raw)cpu->A > 127 || (byte_raw)cpu->A < -127) ? set_flag(cpu, OVERFLOW) : reset_flag(cpu, OVERFLOW);
    resolve_flags_NZ(cpu, cpu->A);
}

void RLA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    byte_raw addr;
    switch (addressingMode) {
        case ABSOLUTE: {
            addr = f_stack_pull_16(cpu);
            break;
        }
        case X_INDEXED_ABSOLUTE: {
            addr = f_stack_pull_16(cpu) + cpu->X;
            break;
        }
        case Y_INDEXED_ABSOLUTE: {
            addr = f_stack_pull_16(cpu) + cpu->Y;
            break;
        }
        case ZERO_PAGE: {
            addr = f_stack_pull(cpu);
            break;
        }
        case X_INDEXED_ZERO_PAGE: {
            addr = f_stack_pull(cpu) + cpu->X;
            break;
        }
        case X_INDEXED_ZERO_PAGE_INDIRECT: {
            addr = GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu)) + cpu->X;
            break;
        }
        case ZERO_PAGE_INDIRECT_Y_INDEXED: {
            addr = GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu) + cpu->Y);
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
    byte_raw dat = GET_MEM_ADDR(cpu, addr);
    l_rotate(cpu, dat);
    PUT_MEM_ADDR(cpu, addr, dat);
    cpu->A &= GET_MEM_ADDR(cpu, addr);
    resolve_flags_NZ(cpu, cpu->A);
}

void RRA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    byte_raw addr;
    switch (addressingMode) {
        case ABSOLUTE: {
            addr = f_stack_pull_16(cpu);
            break;
        }
        case X_INDEXED_ABSOLUTE: {
            addr = f_stack_pull_16(cpu) + cpu->X;
            break;
        }
        case Y_INDEXED_ABSOLUTE: {
            addr = f_stack_pull_16(cpu) + cpu->Y;
            break;
        }
        case ZERO_PAGE: {
            addr = f_stack_pull(cpu);
            break;
        }
        case X_INDEXED_ZERO_PAGE: {
            addr = f_stack_pull(cpu) + cpu->X;
            break;
        }
        case X_INDEXED_ZERO_PAGE_INDIRECT: {
            addr = GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu)) + cpu->X;
            break;
        }
        case ZERO_PAGE_INDIRECT_Y_INDEXED: {
            addr = GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu) + cpu->Y);
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
    byte_raw dat = GET_MEM_ADDR(cpu, addr);
    r_rotate(cpu, dat);
    PUT_MEM_ADDR(cpu, addr, dat);
    ushort result = cpu->A + GET_MEM_ADDR(cpu, addr) + test_flag(cpu, CARRY);
    (test_flag(cpu, DECIMAL_MODE) && result > 99) ? set_flag(cpu, CARRY) : (result > 255) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
    byte_raw hi, lo;
    split_ushort(result, hi, lo);
    cpu->A += (hi + lo);
}

void SBX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    byte_raw operand, result;
    switch (addressingMode) {
        case IMMEDIATE: {
            operand = f_stack_pull(cpu);
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
    result = (cpu->A & cpu->X) - operand;
    (result >= 0) ? set_flag(cpu, CARRY) : reset_flag(cpu, CARRY);
    resolve_flags_NZ(cpu, result);

    cpu->X = result;
}

void SLO(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    byte_raw addr;
    switch (addressingMode) {
        case ABSOLUTE: {
            addr = f_stack_pull_16(cpu);
            break;
        }
        case X_INDEXED_ABSOLUTE: {
            addr = f_stack_pull_16(cpu) + cpu->X;
            break;
        }
        case Y_INDEXED_ABSOLUTE: {
            addr = f_stack_pull_16(cpu) + cpu->Y;
            break;
        }
        case ZERO_PAGE: {
            addr = f_stack_pull(cpu);
            break;
        }
        case X_INDEXED_ZERO_PAGE: {
            addr = f_stack_pull(cpu) + cpu->X;
            break;
        }
        case X_INDEXED_ZERO_PAGE_INDIRECT: {
            addr = GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu)) + cpu->X;
            break;
        }
        case ZERO_PAGE_INDIRECT_Y_INDEXED: {
            addr = GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu) + cpu->Y);
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }
    PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) << 1);
    cpu->A &= GET_MEM_ADDR(cpu, addr);
    resolve_flags_NZ(cpu, cpu->A);
    (test_flag(cpu, NEGATIVE)) ? (set_flag_on_bit(cpu, CARRY, cpu->A, 7)) : ((void)0);
}

void SRE(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    byte_raw addr;
    switch (addressingMode) {
        case ABSOLUTE: {
            addr = f_stack_pull_16(cpu);
            break;
        }
        case X_INDEXED_ABSOLUTE: {
            addr = f_stack_pull_16(cpu) + cpu->X;
            break;
        }
        case Y_INDEXED_ABSOLUTE: {
            addr = f_stack_pull_16(cpu) + cpu->Y;
            break;
        }
        case ZERO_PAGE: {
            addr = f_stack_pull(cpu);
            break;
        }
        case X_INDEXED_ZERO_PAGE: {
            addr = f_stack_pull(cpu) + cpu->X;
            break;
        }
        case X_INDEXED_ZERO_PAGE_INDIRECT: {
            addr = GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu)) + cpu->X;
            break;
        }
        case ZERO_PAGE_INDIRECT_Y_INDEXED: {
            addr = GET_MEM_ADDR(cpu, 
                        f_stack_pull(cpu) + cpu->Y);
            break;
        }
        default: {
            FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
            return;
        }
    }

    PUT_MEM_ADDR(cpu, addr, GET_MEM_ADDR(cpu, addr) >> 1);
    cpu->A ^= GET_MEM_ADDR(cpu, addr);
}

static inline float Q_rsqrt(float number)
{
  long i;
  float x2, y;
  const float threehalfs = 1.5F;

  x2 = number * 0.5F;
  y  = number;
  i  = * ( long * ) &y;                       // evil floating point bit level hacking
  i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
  y  = * ( float * ) &i;
  y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
  // y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

  return y;
}

void XXA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    if (addressingMode != IMMEDIATE)
        FATAL_ERROR(ERR_UNSUPPORTED_ADDR_MODE);
    
    // Create potential options as base value, and bitmask
    const u_byte vectors[5] = { 0x00, 0xEE, 0XEF, 0XFE, 0XFF };
    const u_byte bitmask = 0b010111000;

    // Leaving random variables uninitialized to increase potential randomness
    // Keep things volatile to make GCC and other compilers happy
    volatile byte_raw operation_result;
    volatile u_byte vector_offset, vector_selection;
    
    // Generate shitty random number
    u_byte low_quality_rand = rand();

    // Extract value from UNIX standard entropy pool
    FILE* random = fopen("/dev/urandom", "r");
    if (random == NULL)
        FATAL_ERROR(ERR_FILE_READ_ERROR);
    fscanf(random, "%c", &operation_result);
    fclose(random);

    // XOR result because I'm ✨️ fancy ✨️
    operation_result ^= bitmask;

    // AND low quality random number with high-quality one.
    vector_offset = low_quality_rand & operation_result;
    
    // Decide chosen vector from array using high quality random
    vector_selection = vector_offset % 5;

    // Add offset to chosen vector to high quality vector
    operation_result += vectors[vector_selection] + vector_offset;
    
    // Finally, perform core operation
    cpu->A = (cpu->A & operation_result ^ low_quality_rand) & cpu->X & f_stack_pull(cpu);

    // Fuck you. (fast inverse square root's your ass)
    cpu->A = (u_byte)Q_rsqrt((float)cpu->A++);
}
#endif