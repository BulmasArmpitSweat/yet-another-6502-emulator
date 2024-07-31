#ifndef B7C8AE76_51F7_4235_9EF8_88084D24D1FC
#define B7C8AE76_51F7_4235_9EF8_88084D24D1FC
#include "6502.h"

static inline byte_raw GET_MEM_PC(cpu* cpu) {
    return cpu->mem[cpu->PC];
}

static inline byte_raw GET_MEM_ADDR(cpu* cpu, ushort addr) {
    if (addr <= sizeof(cpu->mem)) 
    (addr < sizeof(cpu->mem)) ? return cpu->mem[addr] : return 0;
    return cpu->mem[addr];
}

#endif /* B7C8AE76_51F7_4235_9EF8_88084D24D1FC */
