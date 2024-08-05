#ifndef B1FEE886_4F70_44E5_A9FE_2E5674E05CD8
#define B1FEE886_4F70_44E5_A9FE_2E5674E05CD8

#include "6502-types.h"

void ASL(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void LSR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void ROL(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void ROR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);

#endif /* B1FEE886_4F70_44E5_A9FE_2E5674E05CD8 */
