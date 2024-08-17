#ifndef XJFKDJF_DNVNDFJ_J3NNFJD_DMEJ
#define XJFKDJF_DNVNDFJ_J3NNFJD_DMEJ

#include "6502-types.h"

void DEC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void DEX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void DEY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void INC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void INX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void INY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);

#endif /* XJFKDJF_DNVNDFJ_J3NNFJD_DMEJ */