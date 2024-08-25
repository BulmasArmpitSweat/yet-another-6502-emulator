#include "flags.h"
#include "../include/6502-types.h"

void set_flag(cpu* cpu, const byte_raw flag) {
    cpu->SR |= flag;
}

void reset_flag(cpu* cpu, const byte_raw flag) {
    cpu->SR &= ~flag;
}


void toggle_flag(cpu* cpu, const byte_raw flag) {
    cpu->SR ^= (1 << flag);
}

bool test_flag(cpu* cpu, const byte_raw flag) {
    return (cpu->SR & flag) != 0;
}

void set_bit(byte_raw data, char index) {
    data |= (1 << index);
}

void reset_bit(byte_raw data, char index) {
    data &= ~(1 << index);
}

void toggle_bit(byte_raw data, char index) {
    data ^= (1 << index);
}

bool test_bit(byte_raw reference, char index) {
    return (reference & (1 << index)) ? true : false;
}

void resolve_flags_NZ(cpu* cpu, byte_raw reference) {
    if (reference == 0) set_flag(cpu, ZERO);
    else reset_flag(cpu, ZERO);

    if (test_bit(reference, 7) == true) set_flag(cpu, NEGATIVE);
    else reset_flag(cpu, NEGATIVE);
}