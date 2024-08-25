#include "../include/6502-types.h"
#include "rotate.h"
#include "flags.h"

void l_rotate(cpu* cpu, byte_raw data) {
    bit c;
    c.x = test_flag(cpu, CARRY);
    set_flag_on_bit(cpu, CARRY, data, 0);
    data >>= 1;
    set_bit_on_flag(cpu, CARRY, data, 7);
}

void r_rotate(cpu* cpu, byte_raw data) {
    bit c;
    c.x = test_flag(cpu, CARRY);
    set_flag_on_bit(cpu, CARRY, data, 7);
    data <<= 1;
    set_bit_on_flag(cpu, CARRY, data, 0);
}