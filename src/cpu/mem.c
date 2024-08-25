#include "../include/6502-types.h"
#include "mem.h"
#include <ctype.h>
#include <string.h>

byte_raw GET_MEM_PC(cpu* cpu) {
    return cpu->mem[cpu->PC];
}

byte_raw GET_MEM_ADDR(cpu* cpu, ushort addr) {
    if (addr > sizeof(cpu->mem)) FATAL_ERROR(ERR_BUFFER_OVERFLOW);
    return cpu->mem[addr];
}

/* Get memory zero-page */
byte_raw GET_MEM_ADDR_ZP(cpu* cpu, byte_raw addr) {
    return cpu->mem[addr];
}

ushort GET_MEM_ADDR_16(cpu *cpu, byte_raw addr) {
    if (addr <= sizeof(cpu->mem)) FATAL_ERROR(ERR_BUFFER_OVERFLOW);
    byte_raw low_byte = cpu->mem[addr];
    byte_raw high_byte = cpu->mem[(addr + 1) % 256];
    return (ushort)(high_byte << 8) | high_byte;
}

void PUT_MEM_PC(cpu* cpu, byte_raw data) {
    cpu->mem[cpu->PC] = data;
}

void PUT_MEM_ADDR(cpu* cpu, ushort addr, byte_raw data) {
    cpu->mem[addr] = data;
}

bool test_char(char input, char* reference) {
    input = tolower(input);
    int referenceLen = strlen(reference);
    for (int i = 0; i < referenceLen; i++)
        if (input == reference[i])
            return true;
    return false;
}

int count_digits(int number) {
    int count = 0;

    // Handle the case where the number is 0
    if (number == 0) {
        return 1;
    }

    // Handle negative numbers by taking their absolute value
    if (number < 0) {
        number = -number;
    }

    while (number != 0) {
        number /= 10;
        count++;
    }

    return count;
}