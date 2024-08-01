#ifndef A3D95EF7_0C06_479E_8957_7656BAB4A8D9
#define A3D95EF7_0C06_479E_8957_7656BAB4A8D9

#include "../../include/6502.h"
#include "../../config.h"

#if defined (INCLUDE_ILLEGAL_OPCODES)

void JAM(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);

#endif
#endif /* A3D95EF7_0C06_479E_8957_7656BAB4A8D9 */
