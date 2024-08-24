#ifndef MFDKDK_KF4MFKKD_FJ4JFJ_DK
#define MFDKDK_KF4MFKKD_FJ4JFJ_DK

#include "include/6502.h"
#include "config.h"

#if defined (INCLUDE_ILLEGAL_OPCODES)
InstructionInfo instructionTable[256] = {
    // Instructions marked with a '*' are illegal opcodes

    // 0x00 to 0x0F
    { *BRK, 7,    IMPLIED, true },
    { *ORA, 6,    X_INDEXED_ZERO_PAGE_INDIRECT, true },
    { *JAM, -1,   NOTHING, true },                                          // *
    { *SLO, 8,    X_INDEXED_ZERO_PAGE_INDIRECT, true },                     // *
    { *NOP, 3,    ZERO_PAGE, true },                                        // *
    { *ORA, 3,    ZERO_PAGE, true },
    { *ASL, 5,    ZERO_PAGE, true },
    { *SLO, 5,    ZERO_PAGE, true },                                        // *
    { *PHP, 3,    IMPLIED, true },
    { *ORA, 2,    IMMEDIATE, true },
    { *ASL, 2,    ACCUMULATOR, true },
    { *ANC, 2,    IMMEDIATE, true },                                        // *
    { *NOP, 4,    ABSOLUTE, true },                                         // *
    { *ORA, 4,    ABSOLUTE, true },
    { *ASL, 6,    ABSOLUTE, true },
    { *SLO, 6,    ABSOLUTE, true },                                         // *
    // 0x10 to 0x1F
    { *BPL, 2,    RELATIVE, false },
    { *ORA, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED, false },
    { *JAM, -1,   NOTHING, true },                                          // *
    { *SLO, 8,    ZERO_PAGE_INDIRECT_Y_INDEXED, true },                     // *
    { *NOP, 4,    X_INDEXED_ZERO_PAGE, true },                              // *
    { *ORA, 4,    X_INDEXED_ZERO_PAGE, true },
    { *ASL, 6,    X_INDEXED_ZERO_PAGE, true },
    { *SLO, 6,    X_INDEXED_ZERO_PAGE, true },                              // *
    { *CLC, 2,    IMPLIED, true },
    { *ORA, 4,    Y_INDEXED_ABSOLUTE, false },
    { *NOP, 4,    X_INDEXED_ABSOLUTE, false },                              // *
    { *SLO, 7,    Y_INDEXED_ABSOLUTE, false },                              // *
    { *NOP, 4,    X_INDEXED_ABSOLUTE, false },                              // *
    { *ORA, 4,    X_INDEXED_ABSOLUTE, false },
    { *ASL, 7,    X_INDEXED_ABSOLUTE, true },
    { *SLO, 7,    X_INDEXED_ABSOLUTE, true },                               // *
    // 0x20 to 0x2F
    { *JSR, 6,    ABSOLUTE, true },
    { *AND, 6,    X_INDEXED_ZERO_PAGE_INDIRECT, true },
    { *JAM, -1,   NOTHING, true },                                          // *
    { *RLA, 8,    X_INDEXED_ZERO_PAGE_INDIRECT, true },                     // *
    { *BIT, 3,    ZERO_PAGE, true },
    { *AND, 3,    ZERO_PAGE, true },
    { *ROL, 5,    ZERO_PAGE, true },
    { *RLA, 5,    ZERO_PAGE, true },                                        // *
    { *PLP, 4,    IMPLIED, true },
    { *AND, 2,    IMMEDIATE, true },
    { *ROL, 2,    ACCUMULATOR, true },
    { *ANC, 2,    IMMEDIATE, true },                                        // *
    { *BIT, 4,    ABSOLUTE, true },
    { *AND, 4,    ABSOLUTE, true },
    { *ROL, 6,    ABSOLUTE, true },
    { *RLA, 6,    ABSOLUTE, true },                                         // *
    // 0x30 to 0x3F
    { *BMI, 2,    RELATIVE, false },
    { *AND, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED, false },
    { *JAM, -1,   NOTHING, true },                                          // *
    { *RLA, 8,    ZERO_PAGE_INDIRECT_Y_INDEXED, true },                     // *
    { *NOP, 4,    X_INDEXED_ZERO_PAGE, true },                              // *
    { *AND, 4,    X_INDEXED_ZERO_PAGE, true },
    { *ROL, 6,    X_INDEXED_ZERO_PAGE, true },
    { *RLA, 6,    X_INDEXED_ZERO_PAGE, true },                              // *
    { *SEC, 2,    IMPLIED, true },
    { *AND, 4,    Y_INDEXED_ABSOLUTE, false },
    { *NOP, 4,    X_INDEXED_ABSOLUTE, false },                              // *
    { *RLA, 7,    Y_INDEXED_ABSOLUTE, true },                               // *
    { *NOP, 4,    X_INDEXED_ABSOLUTE, false },                              // *
    { *AND, 4,    X_INDEXED_ABSOLUTE, false },
    { *ROL, 7,    X_INDEXED_ABSOLUTE, true },
    { *RLA, 7,    X_INDEXED_ABSOLUTE, true },                               // *
    // 0x40 to 0x4F
    { *RTI, 6,    IMPLIED, true },
    { *EOR, 6,    X_INDEXED_ZERO_PAGE_INDIRECT, true },
    { *JAM, -1,   NOTHING, true },                                          // *
    { *SRE, 8,    X_INDEXED_ZERO_PAGE_INDIRECT, true },                     // *
    { *NOP, 3,    ZERO_PAGE, true },                                        // *
    { *EOR, 3,    ZERO_PAGE, true },
    { *LSR, 5,    ZERO_PAGE, true },
    { *SRE, 5,    ZERO_PAGE, true },                                        // *
    { *PHA, 3,    IMPLIED, true },
    { *EOR, 2,    IMMEDIATE, true },
    { *LSR, 2,    ACCUMULATOR, true },
    { *ASR, 2,    IMMEDIATE, true },                                        // *
    { *JMP, 2,    ABSOLUTE, true },
    { *EOR, 4,    ABSOLUTE, true },
    { *LSR, 6,    ABSOLUTE, true },
    { *SRE, 6,    ABSOLUTE, true },                                         // *
    // 0x50 to 0x5F
    { *BVC, 2,    RELATIVE, false },
    { *EOR, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED, false },
    { *JAM, -1,   NOTHING, true },                                          // *
    { *SRE, 8,    ZERO_PAGE_INDIRECT_Y_INDEXED, true },                     // *
    { *NOP, 4,    X_INDEXED_ZERO_PAGE, true },                              // *
    { *EOR, 4,    X_INDEXED_ZERO_PAGE, true },
    { *LSR, 6,    X_INDEXED_ZERO_PAGE, true },
    { *SRE, 6,    X_INDEXED_ZERO_PAGE, true },                              // *
    { *CLI, 2,    IMPLIED, true },
    { *EOR, 4,    Y_INDEXED_ABSOLUTE, false },
    { *NOP, 4,    X_INDEXED_ABSOLUTE, false },                              // *
    { *SRE, 7,    Y_INDEXED_ABSOLUTE, true },                               // *
    { *NOP, 4,    X_INDEXED_ABSOLUTE, false },                              // *
    { *EOR, 4,    X_INDEXED_ABSOLUTE, false },
    { *LSR, 7,    X_INDEXED_ABSOLUTE, true },
    { *SRE, 7,    X_INDEXED_ABSOLUTE, true },                               // *
    // 0x60 to 0x6F
    { *RTS, 6,    IMPLIED, true },
    { *ADC, 6,    X_INDEXED_ZERO_PAGE_INDIRECT, true },
    { *JAM, -1,   NOTHING, true },                                          // *
    { *RRA, 8,    X_INDEXED_ZERO_PAGE_INDIRECT, true },                     // *
    { *NOP, 3,    ZERO_PAGE, true },                                        // *
    { *ADC, 3,    ZERO_PAGE, true },
    { *ROR, 5,    ZERO_PAGE, true },
    { *RRA, 5,    ZERO_PAGE, true },                                        // *
    { *PLA, 4,    IMPLIED, true },
    { *ADC, 2,    IMMEDIATE, true },
    { *ROR, 2,    ACCUMULATOR, true },
    { *ARR, 2,    IMMEDIATE, true },                                        // *
    { *JMP, 5,    ABSOLUTE_INDIRECT, true },
    { *ADC, 4,    ABSOLUTE, true },
    { *ROR, 6,    ABSOLUTE, true },
    { *RRA, 6,    ABSOLUTE, true },                                         // *
    // 0x70 to 0x7F
    { *BVS, 2,    RELATIVE, false },
    { *ADC, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED, false },
    { *JAM, -1,   NOTHING, true },                                          // *
    { *RRA, 8,    ZERO_PAGE_INDIRECT_Y_INDEXED, true },                     // *
    { *NOP, 4,    X_INDEXED_ZERO_PAGE, true },                              // *
    { *ADC, 4,    X_INDEXED_ZERO_PAGE, true },
    { *ROR, 6,    X_INDEXED_ZERO_PAGE, true },
    { *RRA, 6,    X_INDEXED_ZERO_PAGE, true },                              // *
    { *SEI, 2,    IMPLIED, true },
    { *ADC, 4,    Y_INDEXED_ABSOLUTE, false },
    { *NOP, 4,    X_INDEXED_ABSOLUTE, false },                              // *
    { *RRA, 7,    Y_INDEXED_ABSOLUTE, true },                               // *
    { *NOP, 4,    X_INDEXED_ABSOLUTE, false },                              // *
    { *ADC, 4,    X_INDEXED_ABSOLUTE, false },
    { *ROR, 7,    X_INDEXED_ABSOLUTE, true },
    { *RRA, 7,    X_INDEXED_ABSOLUTE, true },                               // *
    // 0x80 to 0x8F
    { *NOP, 2,    IMMEDIATE, true },                                        // *
    { *STA, 6,    X_INDEXED_ZERO_PAGE_INDIRECT, true },
    { *NOP, 2,    IMMEDIATE, true },                                        // *
    { *SAX, 6,    X_INDEXED_ZERO_PAGE_INDIRECT, true },                     // *
    { *STY, 3,    ZERO_PAGE, true },
    { *STA, 3,    ZERO_PAGE, true },
    { *STX, 3,    ZERO_PAGE, true },
    { *SAX, 3,    ZERO_PAGE, true },                                        // *
    { *DEY, 2,    IMPLIED, true },
    { *NOP, 2,    IMMEDIATE, true },                                        // *
    { *TXA, 2,    IMPLIED, true },
    { *XXA, 2,    IMMEDIATE, true },                                        // *
    { *STY, 4,    ABSOLUTE, true },
    { *STA, 4,    ABSOLUTE, true },
    { *STX, 4,    ABSOLUTE, true },
    { *SAX, 4,    ABSOLUTE, true },                                         // *
    // 0x90 to 0x9F
    { *BCC, 2,    RELATIVE, false },
    { *STA, 6,    ZERO_PAGE_INDIRECT_Y_INDEXED, true },
    { *JAM, -1,   NOTHING, true },                                          // *
    { *SHA, 6,    ZERO_PAGE_INDIRECT_Y_INDEXED, true },                     // *
    { *STY, 4,    X_INDEXED_ZERO_PAGE, true },
    { *STA, 4,    X_INDEXED_ZERO_PAGE, true },
    { *STX, 4,    Y_INDEXED_ZERO_PAGE, true },
    { *SAX, 4,    Y_INDEXED_ZERO_PAGE, true },                              // *
    { *TYA, 2,    IMPLIED, true },
    { *STA, 5,    Y_INDEXED_ABSOLUTE, true },
    { *TXS, 2,    IMPLIED, true },
    { *SHS, 5,    Y_INDEXED_ABSOLUTE, true },                               // *
    { *SHY, 5,    X_INDEXED_ABSOLUTE, true },                               // *
    { *STA, 5,    X_INDEXED_ABSOLUTE, true },
    { *SHX, 5,    Y_INDEXED_ABSOLUTE, true },                               // *
    { *SHA, 5,    Y_INDEXED_ABSOLUTE, true },                               // *
    // 0xA0 to 0xAF
    { *LDY, 2,    IMMEDIATE, true },
    { *LDA, 6,    X_INDEXED_ZERO_PAGE_INDIRECT, true },
    { *LDX, 2,    IMMEDIATE, true },
    { *LAX, 6,    X_INDEXED_ZERO_PAGE_INDIRECT, true },                     // *
    { *LDY, 3,    ZERO_PAGE, true },
    { *LDA, 3,    ZERO_PAGE, true },
    { *LDX, 3,    ZERO_PAGE, true },
    { *LAX, 3,    ZERO_PAGE, true },                                        // *
    { *TAY, 2,    IMPLIED, true },
    { *LDA, 2,    IMMEDIATE, true },
    { *TAX, 2,    IMPLIED, true },
    { *LDY, 2,    IMMEDIATE, true },                                        // *
    { *LDY, 4,    ABSOLUTE, true },
    { *LDA, 4,    ABSOLUTE, true },
    { *LDX, 4,    ABSOLUTE, true },
    { *LAX, 4,    ABSOLUTE, true },                                         // *
    // 0xB0 to 0xBF
    { *BCS, 2,    RELATIVE, false },
    { *LDA, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED, false },
    { *JAM, -1,   NOTHING, true },                                          // *
    { *LAX, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED, false },                    // *
    { *LDY, 4,    X_INDEXED_ZERO_PAGE, true },
    { *LDA, 4,    X_INDEXED_ZERO_PAGE, true },
    { *LDX, 4,    Y_INDEXED_ZERO_PAGE, true },
    { *LAX, 4,    Y_INDEXED_ZERO_PAGE, true },                              // *
    { *CLV, 2,    IMPLIED, true },
    { *LDA, 4,    Y_INDEXED_ABSOLUTE, false },
    { *TSX, 2,    IMPLIED, true },
    { *LAS, 4,    Y_INDEXED_ABSOLUTE, false },                              // *
    { *LDY, 4,    X_INDEXED_ABSOLUTE, false },
    { *LDA, 4,    X_INDEXED_ABSOLUTE, false },
    { *LDX, 4,    Y_INDEXED_ABSOLUTE, false },
    { *LAX, 4,    Y_INDEXED_ABSOLUTE, false },                              // *
    // 0xC0 to 0xCF
    { *CPY, 2,    IMMEDIATE, true },
    { *CMP, 6,    X_INDEXED_ZERO_PAGE_INDIRECT, true },
    { *NOP, 2,    IMMEDIATE, true },                                        // *
    { *DCP, 8,    X_INDEXED_ZERO_PAGE_INDIRECT, true },                     // *
    { *CPY, 3,    ZERO_PAGE, true },
    { *CMP, 3,    ZERO_PAGE, true },
    { *DEC, 5,    ZERO_PAGE, true },
    { *DCP, 5,    ZERO_PAGE, true },                                        // *
    { *INY, 2,    IMPLIED, true },
    { *CMP, 2,    IMMEDIATE, true },
    { *DEX, 2,    IMPLIED, true },
    { *SBX, 2,    IMMEDIATE, true },                                        // *
    { *CPY, 4,    ABSOLUTE, true },
    { *CMP, 4,    ABSOLUTE, true },
    { *DEC, 6,    ABSOLUTE, true },
    { *DCP, 6,    ABSOLUTE, true },                                         // *
    // 0xD0 to 0xDF
    { *BNE, 2,    RELATIVE, false },
    { *CMP, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED, false },
    { *JAM, -1,   NOTHING, true },                                          // *
    { *DCP, 8,    ZERO_PAGE_INDIRECT_Y_INDEXED, true },                     // *
    { *NOP, 4,    X_INDEXED_ZERO_PAGE, true },                              // *
    { *CMP, 3,    ZERO_PAGE, true },
    { *DEC, 6,    X_INDEXED_ZERO_PAGE, true },
    { *DCP, 6,    X_INDEXED_ZERO_PAGE, true },                              // *
    { *CLD, 2,    IMPLIED, true },
    { *CMP, 4,    Y_INDEXED_ABSOLUTE, false },
    { *NOP, 4,    X_INDEXED_ABSOLUTE, false },                              // *
    { *DCP, 7,    Y_INDEXED_ABSOLUTE, true },                               // *
    { *NOP, 4,    X_INDEXED_ABSOLUTE, false },                              // *
    { *CMP, 4,    X_INDEXED_ABSOLUTE, false },
    { *DEC, 7,    X_INDEXED_ABSOLUTE, true },
    { *DCP, 7,    X_INDEXED_ABSOLUTE, true },                               // *
    // 0xE0 to 0xEF
    { *CPX, 2,    IMMEDIATE, true },
    { *SBC, 6,    X_INDEXED_ZERO_PAGE_INDIRECT, true },
    { *NOP, 2,    IMMEDIATE, true },                                        // *
    { *ISC, 8,    X_INDEXED_ZERO_PAGE_INDIRECT, true },                     // *
    { *CPX, 3,    ZERO_PAGE, true },
    { *SBC, 3,    ZERO_PAGE, true },
    { *INC, 5,    ZERO_PAGE, true },
    { *ISC, 5,    ZERO_PAGE, true },                                        // *
    { *INX, 2,    IMPLIED, true },
    { *SBC, 2,    IMMEDIATE, true },
    { *NOP, 2,    IMPLIED, true },
    { *SBC,2,    IMMEDIATE, true },                                         // *
    { *CPX, 4,    ABSOLUTE, true },
    { *SBC, 4,    ABSOLUTE, true },
    { *INC, 6,    ABSOLUTE, true },
    { *ISC, 6,    ABSOLUTE, true },                                         // *
    // 0xF0 to 0xFF
    { *BEQ, 2,    RELATIVE, false },
    { *SBC, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED, false },
    { *JAM, -1,   NOTHING, true },                                          // *
    { *ISC, 8,    ZERO_PAGE_INDIRECT_Y_INDEXED, true },                     // *
    { *NOP, 4,    X_INDEXED_ZERO_PAGE, true },                              // *
    { *SBC, 4,    X_INDEXED_ZERO_PAGE, true },
    { *INC, 6,    X_INDEXED_ZERO_PAGE, true },
    { *ISC, 6,    X_INDEXED_ZERO_PAGE, true },                              // *
    { *SED, 2,    IMPLIED, true },
    { *SBC, 4,    Y_INDEXED_ABSOLUTE, false },
    { *NOP, 4,    X_INDEXED_ABSOLUTE, false },                              // *
    { *ISC, 7,    Y_INDEXED_ABSOLUTE, true },                               // *
    { *NOP, 4,    X_INDEXED_ABSOLUTE, false },                              // *
    { *SBC, 4,    X_INDEXED_ABSOLUTE, false },
    { *INC, 7,    X_INDEXED_ABSOLUTE, true },
    { *ISC, 7,    X_INDEXED_ABSOLUTE, true },                               // *
};
#else
InstructionInfo instructionTable[256] = {
    // 0x00 to 0x0F
    { *BRK, 7,    IMPLIED, true },
    { *ORA, 6,    X_INDEXED_ZERO_PAGE_INDIRECT, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *ORA, 3,    ZERO_PAGE, true },
    { *ASL, 5,    ZERO_PAGE, true },
    { *NOP, 2,    IMPLIED, true },
    { *PHP, 3,    IMPLIED, true },
    { *ORA, 2,    IMMEDIATE, true },
    { *ASL, 2,    ACCUMULATOR, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *ORA, 4,    ABSOLUTE, true },
    { *ASL, 6,    ABSOLUTE, true },
    { *NOP, 2,    IMPLIED, true },
    // 0x10 to 0x1F
    { *BPL, 2,    RELATIVE, false },
    { *ORA, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED, false },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *ORA, 4,    X_INDEXED_ZERO_PAGE, true },
    { *ASL, 6,    X_INDEXED_ZERO_PAGE, true },
    { *NOP, 2,    IMPLIED, true },
    { *CLC, 2,    IMPLIED, true },
    { *ORA, 4,    Y_INDEXED_ABSOLUTE, false },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *ORA, 4,    X_INDEXED_ABSOLUTE, false },
    { *ASL, 7,    X_INDEXED_ABSOLUTE, true },
    { *NOP, 2,    IMPLIED, true },
    // 0x20 to 0x2F
    { *JSR, 6,    ABSOLUTE, true },
    { *AND, 6,    X_INDEXED_ZERO_PAGE_INDIRECT, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *BIT, 3,    ZERO_PAGE, true },
    { *AND, 3,    ZERO_PAGE, true },
    { *ROL, 5,    ZERO_PAGE, true },
    { *NOP, 2,    IMPLIED, true },
    { *PLP, 4,    IMPLIED, true },
    { *AND, 2,    IMMEDIATE, true },
    { *ROL, 2,    ACCUMULATOR, true },
    { *NOP, 2,    IMPLIED, true },
    { *BIT, 4,    ABSOLUTE, true },
    { *AND, 4,    ABSOLUTE, true },
    { *ROL, 6,    ABSOLUTE, true },
    { *NOP, 2,    IMPLIED, true },
    // 0x30 to 0x3F
    { *BMI, 2,    RELATIVE, false },
    { *AND, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED, false },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *AND, 4,    X_INDEXED_ZERO_PAGE, true },
    { *ROL, 6,    X_INDEXED_ZERO_PAGE, true },
    { *NOP, 2,    IMPLIED, true },
    { *SEC, 2,    IMPLIED, true },
    { *AND, 4,    Y_INDEXED_ABSOLUTE, false },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *AND, 4,    X_INDEXED_ABSOLUTE, false },
    { *ROL, 7,    X_INDEXED_ABSOLUTE, true },
    { *NOP, 2,    IMPLIED, true },
    // 0x40 to 0x4F
    { *RTI, 6,    IMPLIED, true },
    { *EOR, 6,    X_INDEXED_ZERO_PAGE_INDIRECT, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *EOR, 3,    ZERO_PAGE, true },
    { *LSR, 5,    ZERO_PAGE, true },
    { *NOP, 2,    IMPLIED, true },
    { *PHA, 3,    IMPLIED, true },
    { *EOR, 2,    IMMEDIATE, true },
    { *LSR, 2,    ACCUMULATOR, true },
    { *NOP, 2,    IMPLIED, true },
    { *JMP, 2,    ABSOLUTE, true },
    { *EOR, 4,    ABSOLUTE, true },
    { *LSR, 6,    ABSOLUTE, true },
    { *NOP, 2,    IMPLIED, true },
    // 0x50 to 0x5F
    { *BVC, 2,    RELATIVE, false },
    { *EOR, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED, false },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *EOR, 4,    X_INDEXED_ZERO_PAGE, true },
    { *LSR, 6,    X_INDEXED_ZERO_PAGE, true },
    { *NOP, 2,    IMPLIED, true },
    { *CLI, 2,    IMPLIED, true },
    { *EOR, 4,    Y_INDEXED_ABSOLUTE, false },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *EOR, 4,    X_INDEXED_ABSOLUTE, false },
    { *LSR, 7,    X_INDEXED_ABSOLUTE, true },
    { *NOP, 2,    IMPLIED, true },
    // 0x60 to 0x6F
    { *RTS, 6,    IMPLIED, true },
    { *ADC, 6,    X_INDEXED_ZERO_PAGE_INDIRECT, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *ADC, 3,    ZERO_PAGE, true },
    { *ROR, 5,    ZERO_PAGE, true },
    { *NOP, 2,    IMPLIED, true },
    { *PLA, 4,    IMPLIED, true },
    { *ADC, 2,    IMMEDIATE, true },
    { *ROR, 2,    ACCUMULATOR, true },
    { *NOP, 2,    IMPLIED, true },
    { *JMP, 5,    ABSOLUTE_INDIRECT, true },
    { *ADC, 4,    ABSOLUTE, true },
    { *ROR, 6,    ABSOLUTE, true },
    { *NOP, 2,    IMPLIED, true },
    // 0x70 to 0x7F
    { *BVS, 2,    RELATIVE, false },
    { *ADC, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED, false },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *ADC, 4,    X_INDEXED_ZERO_PAGE, true },
    { *ROR, 6,    X_INDEXED_ZERO_PAGE, true },
    { *NOP, 2,    IMPLIED, true },
    { *SEI, 2,    IMPLIED, true },
    { *ADC, 4,    Y_INDEXED_ABSOLUTE, false },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *ADC, 4,    X_INDEXED_ABSOLUTE, false },
    { *ROR, 7,    X_INDEXED_ABSOLUTE, true },
    { *NOP, 2,    IMPLIED, true },
    // 0x80 to 0x8F
    { *NOP, 2,    IMPLIED, true },
    { *STA, 6,    X_INDEXED_ZERO_PAGE_INDIRECT, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *STY, 3,    ZERO_PAGE, true },
    { *STA, 3,    ZERO_PAGE, true },
    { *STX, 3,    ZERO_PAGE, true },
    { *NOP, 2,    IMPLIED, true },
    { *DEY, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *TXA, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *STY, 4,    ABSOLUTE, true },
    { *STA, 4,    ABSOLUTE, true },
    { *STX, 4,    ABSOLUTE, true },
    { *NOP, 2,    IMPLIED, true },
    // 0x90 to 0x9F
    { *BCC, 2,    RELATIVE, false },
    { *STA, 6,    ZERO_PAGE_INDIRECT_Y_INDEXED, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *STY, 4,    X_INDEXED_ZERO_PAGE, true },
    { *STA, 4,    X_INDEXED_ZERO_PAGE, true },
    { *STX, 4,    Y_INDEXED_ZERO_PAGE, true },
    { *NOP, 2,    IMPLIED, true },
    { *TYA, 2,    IMPLIED, true },
    { *STA, 5,    Y_INDEXED_ABSOLUTE, true },
    { *TXS, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *STA, 5,    X_INDEXED_ABSOLUTE, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    // 0xA0 to 0xAF
    { *LDY, 2,    IMMEDIATE, true },
    { *LDA, 6,    X_INDEXED_ZERO_PAGE_INDIRECT, true },
    { *LDX, 2,    IMMEDIATE, true },
    { *NOP, 2,    IMPLIED, true },
    { *LDY, 3,    ZERO_PAGE, true },
    { *LDA, 3,    ZERO_PAGE, true },
    { *LDX, 3,    ZERO_PAGE, true },
    { *NOP, 2,    IMPLIED, true },
    { *TAY, 2,    IMPLIED, true },
    { *LDA, 2,    IMMEDIATE, true },
    { *TAX, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *LDY, 4,    ABSOLUTE, true },
    { *LDA, 4,    ABSOLUTE, true },
    { *LDX, 4,    ABSOLUTE, true },
    { *NOP, 2,    IMPLIED, true },
    // 0xB0 to 0xBF
    { *BCS, 2,    RELATIVE, false },
    { *LDA, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED, false },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *LDY, 4,    X_INDEXED_ZERO_PAGE, true },
    { *LDA, 4,    X_INDEXED_ZERO_PAGE, true },
    { *LDX, 4,    Y_INDEXED_ZERO_PAGE, true },
    { *NOP, 2,    IMPLIED, true },
    { *CLV, 2,    IMPLIED, true },
    { *LDA, 4,    Y_INDEXED_ABSOLUTE, false },
    { *TSX, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *LDY, 4,    X_INDEXED_ABSOLUTE, false },
    { *LDA, 4,    X_INDEXED_ABSOLUTE, false },
    { *LDX, 4,    Y_INDEXED_ABSOLUTE, false },
    { *NOP, 2,    IMPLIED, true },
    // 0xC0 to 0xCF
    { *CPY, 2,    IMMEDIATE, true },
    { *CMP, 6,    X_INDEXED_ZERO_PAGE_INDIRECT, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *CPY, 3,    ZERO_PAGE, true },
    { *CMP, 3,    ZERO_PAGE, true },
    { *DEC, 5,    ZERO_PAGE, true },
    { *NOP, 2,    IMPLIED, true },
    { *INY, 2,    IMPLIED, true },
    { *CMP, 2,    IMMEDIATE, true },
    { *DEX, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *CPY, 4,    ABSOLUTE, true },
    { *CMP, 4,    ABSOLUTE, true },
    { *DEC, 6,    ABSOLUTE, true },
    { *NOP, 2,    IMPLIED, true },
    // 0xD0 to 0xDF
    { *BNE, 2,    RELATIVE, false },
    { *CMP, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED, false },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *CMP, 3,    ZERO_PAGE, true },
    { *DEC, 6,    X_INDEXED_ZERO_PAGE, true },
    { *NOP, 2,    IMPLIED, true },
    { *CLD, 2,    IMPLIED, true },
    { *CMP, 4,    Y_INDEXED_ABSOLUTE, false },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *CMP, 4,    X_INDEXED_ABSOLUTE, false },
    { *DEC, 7,    X_INDEXED_ABSOLUTE, true },
    { *NOP, 2,    IMPLIED, true },
    // 0xE0 to 0xEF
    { *CPX, 2,    IMMEDIATE, true },
    { *SBC, 6,    X_INDEXED_ZERO_PAGE_INDIRECT, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *CPX, 3,    ZERO_PAGE, true },
    { *SBC, 3,    ZERO_PAGE, true },
    { *INC, 5,    ZERO_PAGE, true },
    { *NOP, 2,    IMPLIED, true },
    { *INX, 2,    IMPLIED, true },
    { *SBC, 2,    IMMEDIATE, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *CPX, 4,    ABSOLUTE, true },
    { *SBC, 4,    ABSOLUTE, true },
    { *INC, 6,    ABSOLUTE, true },
    { *NOP, 2,    IMPLIED, true },
    // 0xF0 to 0xFF
    { *BEQ, 2,    RELATIVE, false },
    { *SBC, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED, false },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *SBC, 4,    X_INDEXED_ZERO_PAGE, true },
    { *INC, 6,    X_INDEXED_ZERO_PAGE, true },
    { *NOP, 2,    IMPLIED, true },
    { *SED, 2,    IMPLIED, true },
    { *SBC, 4,    Y_INDEXED_ABSOLUTE, false },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *NOP, 2,    IMPLIED, true },
    { *SBC, 4,    X_INDEXED_ABSOLUTE, false },
    { *INC, 7,    X_INDEXED_ABSOLUTE, true },
    { *NOP, 2,    IMPLIED, true },
};
#endif

#endif /* MFDKDK_KF4MFKKD_FJ4JFJ_DK */