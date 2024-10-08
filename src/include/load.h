#ifndef EA110649_F6F0_4D81_B2E3_117E21965C3A
#define EA110649_F6F0_4D81_B2E3_117E21965C3A

#include "stack-manip.h"
#include "6502-types.h"
#include "../config.h"

void LDA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // M → A
void LDX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // M → X
void LDY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // M → Y
void STA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // A → M
void STX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // X → M
void STY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Y → M

#if defined (INCLUDE_ILLEGAL_OPCODES)

/* ###################################################### */
/* ################### Illegal opcodes ################## */
/* ###################################################### */

void LAS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // M & S → A, X, S
void LAX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // M → A, X
void SAX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // A & X → M
void SHA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // A & X & V → M
void SHX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // X & (HI + 1) → M
void SHY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Y & (H + 1) → M

#endif
#endif /* EA110649_F6F0_4D81_B2E3_117E21965C3A */