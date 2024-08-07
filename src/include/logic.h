#ifndef JDKJFK_89DDA_DJ3DKJ_3JKDF
#define JDKJFK_89DDA_DJ3DKJ_3JKDF

#include "6502-types.h"

void AND(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void BIT(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void EOR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void ORA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);

#endif /* JDKJFK_89DDA_DJ3DKJ_3JKDF */