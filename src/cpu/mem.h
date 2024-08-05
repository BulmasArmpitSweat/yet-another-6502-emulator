#ifndef B7C8AE76_51F7_4235_9EF8_88084D24D1FC
#define B7C8AE76_51F7_4235_9EF8_88084D24D1FC
#include "../include/6502-types.h"

static inline byte_raw GET_MEM_PC(cpu* cpu) {
    return cpu->mem[cpu->PC];
}

static inline byte_raw GET_MEM_ADDR(cpu* cpu, ushort addr) {
    if (addr <= sizeof(cpu->mem)) FATAL_ERROR(ERR_BUFFER_OVERFLOW);
    return cpu->mem[addr];
}

static inline ushort GET_MEM_ADDR_ZP(cpu *cpu, byte_raw addr) {
    byte_raw low_byte = cpu->mem[addr];
    byte_raw high_byte = cpu->mem[(addr + 1) % 256];
    return (ushort)(high_byte << 8) | high_byte;
}

static inline void PUT_MEM_PC(cpu* cpu, byte_raw data) {
    cpu->mem[cpu->PC] = data;
}

static inline void PUT_MEM_ADDR(cpu* cpu, ushort addr, byte_raw data) {
    cpu->mem[addr] = data;
}
#endif /* B7C8AE76_51F7_4235_9EF8_88084D24D1FC */
