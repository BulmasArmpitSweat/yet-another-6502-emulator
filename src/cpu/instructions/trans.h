#ifndef C705195C_34A5_4854_9099_6A3E93F99BAB
#define C705195C_34A5_4854_9099_6A3E93F99BAB

#include "../../include/6502.h"
#include "../../config.h"

void TAX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void TAY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void TSX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void TXA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void TXS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void TYA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);

#if defined (INCLUDE_ILLEGAL_OPCODES)

/* ###################################################### */
/* ################### Illegal opcodes ################## */
/* ###################################################### */

void SHS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);

#endif
#endif /* C705195C_34A5_4854_9099_6A3E93F99BAB */
