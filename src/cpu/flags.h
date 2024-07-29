#pragma once
#include "../include/6502.h"

#define CARRY                   (1 << 0)
#define ZERO                    (1 << 1)
#define INERRUPT_DISABLE        (1 << 2)
#define DECIMAL_MODE            (1 << 3)
#define BRK_COMMAND             (1 << 4)
/* Nothing                      (1 << 5) */
#define OVERFLOW                (1 << 6)
#define NEGATIVE                (1 << 7)

constexpr static inline void set_flag(cpu* cpu, const byte_raw flag) {
    cpu->SR |= flag;
}

constexpr static inline void reset_flag(cpu* cpu, const byte_raw flag) {
    cpu->SR &= ~flag;
}

constexpr static inline void toggle_flag(cpu* cpu, const byte_raw flag) {
    cpu->SR ^= (1 << flag);
}

constexpr static inline unsigned char set_bit(unsigned char d, char i) {
    return d | (1 << i);
}