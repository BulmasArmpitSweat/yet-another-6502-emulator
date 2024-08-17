#ifndef JFIDJIJ_344BFEJD_DDJJE3J
#define JFIDJIJ_344BFEJD_DDJJE3J

#include "6502-types.h"

void BRK(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void JMP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void JSR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void RTI(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void RTS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);

#endif /* JFIDJIJ_344BFEJD_DDJJE3J */