#ifndef JDKFJ4KJ_HEJFD83_E8FDU_H8E9JH
#define JDKFJ4KJ_HEJFD83_E8FDU_H8E9JH

#include "../include/6502-types.h"
#include "flags.h"

static inline void l_rotate(cpu* cpu, byte_raw data) {
    bit c;
    c.x = test_flag(cpu, CARRY);
    set_flag_on_bit(cpu, CARRY, data, 0);
    data >>= 1;
    set_bit_on_flag(cpu, CARRY, data, 7);
}

static inline void r_rotate(cpu* cpu, byte_raw data) {
    bit c;
    c.x = test_flag(cpu, CARRY);
    set_flag_on_bit(cpu, CARRY, data, 7);
    data <<= 1;
    set_bit_on_flag(cpu, CARRY, data, 0);
}

#endif /* JDKFJ4KJ_HEJFD83_E8FDU_H8E9JH */