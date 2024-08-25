#ifndef F1A031F9_7D08_46E4_A5A2_F45030213D8F
#define F1A031F9_7D08_46E4_A5A2_F45030213D8F

#include "../include/6502-types.h"

#define CARRY                   (byte_raw)(1 << 0)
#define ZERO                    (byte_raw)(1 << 1)
#define INTERRUPT_DISABLE       (byte_raw)(1 << 2)
#define DECIMAL_MODE            (byte_raw)(1 << 3)
#define BRK_COMMAND             (byte_raw)(1 << 4)
/* Unused                       (byte_raw)(1 << 5) */
#define OVERFLOW                (byte_raw)(1 << 6)
#define NEGATIVE                (byte_raw)(1 << 7)

#define CARRY_I                   (byte_raw)0
#define ZERO_I                    (byte_raw)1
#define INTERRUPT_DISABLE_I       (byte_raw)2
#define DECIMAL_MODE_I            (byte_raw)3
#define BRK_COMMAND_I             (byte_raw)4
/* Unused                         (byte_raw)5 */
#define OVERFLOW_I                (byte_raw)6
#define NEGATIVE_I                (byte_raw)7

void set_flag(cpu* cpu, const byte_raw flag);
void reset_flag(cpu* cpu, const byte_raw flag);
void toggle_flag(cpu* cpu, const byte_raw flag);
bool test_flag(cpu* cpu, const byte_raw flag);

void set_bit(byte_raw data, char index);
void reset_bit(byte_raw data, char index);
void toggle_bit(byte_raw data, char index);
bool test_bit(byte_raw reference, char index);

#define set_flag_on_bit(cpu, flag, dat, index) (test_bit(dat, index)) ? set_flag(cpu, flag) : reset_flag(cpu, flag)
#define set_bit_on_flag(cpu, flag, dat, index) (test_flag(cpu, flag)) ? set_bit(dat, index) : reset_bit(dat, index)

void resolve_flags_NZ(cpu* cpu, byte_raw reference);
#endif /* F1A031F9_7D08_46E4_A5A2_F45030213D8F */
