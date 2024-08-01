#ifndef F1A031F9_7D08_46E4_A5A2_F45030213D8F
#define F1A031F9_7D08_46E4_A5A2_F45030213D8F

#include "../include/6502.h"

#define CARRY                   (byte_raw)(1 << 0)
#define ZERO                    (byte_raw)(1 << 1)
#define INTERRUPT_DISABLE       (byte_raw)(1 << 2)
#define DECIMAL_MODE            (byte_raw)(1 << 3)
#define BRK_COMMAND             (byte_raw)(1 << 4)
/* Nothing                      (byte_raw)(1 << 5) */
#define OVERFLOW                (byte_raw)(1 << 6)
#define NEGATIVE                (byte_raw)(1 << 7)

static inline void set_flag(cpu* cpu, const byte_raw flag) {
    cpu->SR |= flag;
}

static inline void reset_flag(cpu* cpu, const byte_raw flag) {
    cpu->SR &= ~flag;
}

static inline void toggle_flag(cpu* cpu, const byte_raw flag) {
    cpu->SR ^= (1 << flag);
}

static inline byte_raw set_bit(byte_raw data, char index) {
    return data & (1 << index);
}

static inline bool test_bit(byte_raw reference, char index) {
    return (reference & (1 << index)) ? true : false;
}

static inline void resolve_flags_NV(cpu* cpu, byte_raw reference) {
    if (reference == 0) set_flag(cpu, ZERO);
    else                reset_flag(cpu, ZERO);

    if (test_bit(reference, 7) == true) set_flag(cpu, NEGATIVE);
    else                                reset_flag(cpu, NEGATIVE);
}
#endif /* F1A031F9_7D08_46E4_A5A2_F45030213D8F */
