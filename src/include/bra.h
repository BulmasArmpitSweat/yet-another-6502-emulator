#ifndef JDKJF_88XXKDFK_XX3JI3R
#define JDKJF_88XXKDFK_XX3JI3R

#include "6502-types.h"

void BCC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void BCS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void BEQ(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void BMI(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void BNE(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void BPL(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void BVC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void BVS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);

#endif /* JDKJF_88XXKDFK_XX3JI3R */