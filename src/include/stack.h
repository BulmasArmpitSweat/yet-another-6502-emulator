#ifndef CE18B1ED_43A8_45F7_948B_60766991B325
#define CE18B1ED_43A8_45F7_948B_60766991B325
#include "6502-types.h"

void PHA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void PHP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void PLA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void PLP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
#endif /* CE18B1ED_43A8_45F7_948B_60766991B325 */
