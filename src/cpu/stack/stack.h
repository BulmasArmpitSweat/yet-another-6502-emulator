#pragma once
#include "../../include/6502.h"

#define FSP_CLAMP(value) ((value) < 0 ? 0 : ((value) > 5 ? 5 : (value)))

void stack_push(cpu* cpu, byte_raw src);
byte_raw stack_pull(cpu* cpu);
void stack_push_16(cpu* cpu, unsigned short src);
unsigned short stack_pull_16(cpu* cpu);

void f_stack_push(cpu* cpu, byte_raw src);
byte_raw f_stack_pull(cpu* cpu);
void f_stack_push_16(cpu* cpu, unsigned short src);
unsigned short f_stack_pull_16(cpu* cpu);