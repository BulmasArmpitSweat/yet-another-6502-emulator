#include "stack.h"

void stack_push(cpu *cpu, byte_raw src) {
    cpu->mem[cpu->SP++] = src;
}

byte_raw stack_pull(cpu *cpu) {
    return cpu->mem[cpu->SP--];
}

unsigned short stack_pull_16(cpu* cpu) {
    return (stack_pull(cpu) << 8) | stack_pull(cpu);
}

void stack_push_16(cpu* cpu, unsigned short src) {
    stack_push(cpu, (src >> 8));
    stack_push(cpu, (byte_raw)src);
}

void f_stack_push(cpu *cpu, byte_raw src) {
    cpu->f_stack[FSP_CLAMP(cpu->FSP++)] = src;
}

byte_raw f_stack_pull(cpu *cpu) {
    return cpu->f_stack[FSP_CLAMP(cpu->FSP--)];
}

void f_stack_push_16(cpu* cpu, unsigned short src) {
    f_stack_push(cpu, (src >> 8));
    f_stack_push(cpu, (byte_raw)src);
}

unsigned short f_stack_pull_16(cpu *cpu) {
    return (f_stack_pull(cpu) << 8) | f_stack_pull(cpu);
}

// int *(*(*(**x[])(char*, int*(*)(char*)))[])(char**, char*(*)());