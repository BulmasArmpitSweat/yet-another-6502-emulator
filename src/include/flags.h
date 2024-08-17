#ifndef XXMDKFJK_939UFIDF_39DJK
#define XXMDKFJK_939UFIDF_39DJK

#include "6502-types.h"

void CLC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void CLD(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void CLI(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void CLV(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void SEC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void SED(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void SEI(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);

#endif /* XXMDKFJK_939UFIDF_39DJK */