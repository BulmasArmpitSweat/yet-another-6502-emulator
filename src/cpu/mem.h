#ifndef B7C8AE76_51F7_4235_9EF8_88084D24D1FC
#define B7C8AE76_51F7_4235_9EF8_88084D24D1FC
#include "../include/6502-types.h"
#include <ctype.h>
#include <string.h>

byte_raw GET_MEM_PC(cpu* cpu);
byte_raw GET_MEM_ADDR(cpu* cpu, ushort addr);
/* Get memory zero-page */
byte_raw GET_MEM_ADDR_ZP(cpu* cpu, byte_raw addr);
ushort GET_MEM_ADDR_16(cpu *cpu, byte_raw addr);
void PUT_MEM_PC(cpu* cpu, byte_raw data);
void PUT_MEM_ADDR(cpu* cpu, ushort addr, byte_raw data);
bool test_char(char input, char* reference);
int count_digits(int number);

#endif /* B7C8AE76_51F7_4235_9EF8_88084D24D1FC */
