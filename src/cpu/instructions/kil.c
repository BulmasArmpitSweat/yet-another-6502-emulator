#include "../../include/6502.h"
#include "../../config.h"

#if defined (INCLUDE_ILLEGAL_OPCODES)

void JAM(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    for (;;)
        ;
}

#endif