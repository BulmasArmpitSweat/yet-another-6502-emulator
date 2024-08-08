#ifndef DJFK4JF_EIUVN_45N4JJ4J_4NJF8U
#define DJFK4JF_EIUVN_45N4JJ4J_4NJF8U

#include "6502-types.h"
#include "../config.h"

void ADC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);       // A + M + C -> A, C
void CMP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);       // A + M + C -> A, C
void CPX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);       // A + M + C -> A, C
void CPY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);       // A + M + C -> A, C
void SBC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);       // A + M + C -> A, C

#if defined(INCLUDE_ILLEGAL_OPCODES)

/* ###################################################### */
/* ################### Illegal opcodes ################## */
/* ###################################################### */

void ANC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void ARR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void ASR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void DCP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void ISC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void RLA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void RRA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void SBX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void SLO(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void SRE(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void XXA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);


#endif
#endif /* DJFK4JF_EIUVN_45N4JJ4J_4NJF8U */