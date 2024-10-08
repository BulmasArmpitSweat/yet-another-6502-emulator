#include "load.h"
#include "trans.h"
#include "stack.h"
#include "shift.h"
#include "logic.h"
#include "arith.h"
#include "inc.h"
#include "ctrl.h"
#include "bra.h"
#include "flags.h"
#include "kil.h"
#include "nop.h"

#ifndef EB4D9005_CF3A_40AB_8349_0F425DA6B566
#define EB4D9005_CF3A_40AB_8349_0F425DA6B566
#include "6502-types.h"
#include "../config.h"

static cpu* main_cpu;

/*
    ------------------------------------------------------------------------------------------------------
    Websites used:
        https://www.princeton.edu/~mae412/HANDOUTS/Datasheets/6502.pdf
        https://www.masswerk.at/6502/6502_instruction_set.html#SLO
        https://www.pagetable.com/c64ref/6502/?tab=3#

        And a little help from...

        https://chat.openai.com
        https://en.wikipedia.org/wiki/MOS_Technology_6502
        https://github.com/jdah/doomenstein-3d/blob/main/src/main_wolf.c

        Yes, I have been using ChatGPT, but only when i'm too lazy to figure out some more complex things,
        namely how the instruction function pointers work, however the details of how the CPU itself works
        has been pretty much exclusively collected from the three topmost websites.
    ------------------------------------------------------------------------------------------------------
*/

/*
Instructions:

void ADC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);       // A + M + C -> A, C
void AND(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);       // A & M → A
void ASL(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);       // C <- /M7...M0/ <- 0

void BCC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);       // Branch on C == 0
void BCS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);       // Branch on C == 1
void BEQ(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);       // Branch on Z == 1
void BIT(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);       // A & M, M7 -> N, M6 -> V
void BMI(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);       // Branch on N == 1
void BNE(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);       // Branch on Z == 0
void BPL(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);       // Branch on N == 0
void BRK(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Force break
void BVC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Branch on overflow clear
void BVS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Branch on overflow set

void CLC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Clear carry flag
void CLD(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Clear decimal mode flag
void CLI(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Clear Interrupt disable flag
void CLV(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Clear overflow flag
void CMP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Compare memory and accumulator
void CPX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Compare memory and index X
void CPY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Compare memory and index Y

void DEC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Decrement memory by one
void DEX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Decrement X index by one
void DEY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Decrement Y index by one

void EOR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // "Exclusive Or" memory with accumulator

void INC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Increment memory by one
void INX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Increment X index by one
void INY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Increment Y index by one

void JMP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Jump to new location
void JSR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Jump to subroutine

void LDA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Load accumulator with memory
void LDX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Load Index X with memory
void LDY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Load Index Y with memory
void LSR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Shift one bit right (Memory or accumulator)

void NOP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // No operation

void ORA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // "Or" memory with accumulator

void PHA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Push accumulator on stack
void PHP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Push processor status on stack
void PLA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Pull accumulator from stack
void PLP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Pull processor status from stack

void ROL(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Rotate one bit left (Memory or accumulator)
void ROR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Rotate one bit right (Memory or accumulator)
void RTI(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Return from interrupt
void RTS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Return from subroutine

void SBC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Subtract memory from accumulator with borrow
void SEC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Set carry flag

void SED(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Set decimal mode flag
void SEI(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Set interrupt disable mode flag
void STA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Store accumulator in memory
void STX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Store index X in memory
void STY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Store index Y in memory

void TAX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Transfer accumulator to index X
void TAY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Transfer accumulator to index Y
void TSX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Transfer stack pointer to index X
void TXA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Transfer index X to accumulator
void TXS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Transfer index X to stack pointer
void TYA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Transfer index Y to accumulator


#if defined (INCLUDE_ILLEGAL_OPCODES)

// --------------------------------------------------- //
// ----------------- Illegal Opcodes ----------------- //
// --------------------------------------------------- //

void JAM(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // For realism; Freezes the CPU

void SLO(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // M >> 1 -> M, A || M -> M
void ANC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void RLA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void SRE(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);

void ASR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void RRA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void ARR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void SAX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // A & X -> M

void XXA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void SHA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void TAS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void SHY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);

void SHX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void LAX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // M -> A, X
void LXA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void LAS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // M & SP -> A, X, SP

void DCP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void SBX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void ISC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);

void SHS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Transfer Accumulator "AND" Index Register X to Stack Pointer then Store Stack Pointer "AND" Hi-Byte In Memory
#endif
*/
#endif /* EB4D9005_CF3A_40AB_8349_0F425DA6B566 */
