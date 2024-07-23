#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "6502.h"

InstructionInfo instructionTable[256] = {
    // Instructions marked with a '*' are illegal opcodes

    // 0x00 to 0x0F
    { *BRK, 7,    IMPLIED },
    { *ORA, 6,    X_INDEXED_ZERO_PAGE_INDIRECT },
    { *JAM, -1,   NOTHING },                            // *
    { *SLO, 8,    X_INDEXED_ZERO_PAGE_INDIRECT },       // *
    { *NOP, 3,    ZERO_PAGE },                          // *
    { *ORA, 3,    ZERO_PAGE },
    { *ASL, 5,    ZERO_PAGE },
    { *SLO, 5,    ZERO_PAGE },                          // *
    { *PHP, 3,    IMPLIED },
    { *ORA, 2,    IMMEDIATE },
    { *ASL, 2,    ACCUMULATOR },
    { *ANC, 2,    IMMEDIATE },                          // *
    { *NOP, 4,    ABSOLUTE },                           // *
    { *ORA, 4,    ABSOLUTE },
    { *ASL, 6,    ABSOLUTE },
    { *SLO, 6,    ABSOLUTE },                           // *
    // 0x10 to 0x1F
    { *BPL, 2,    RELATIVE },
    { *ORA, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED },
    { *JAM, -1,   NOTHING },                            // *
    { *SLO, 8,    ZERO_PAGE_INDIRECT_Y_INDEXED },       // *
    { *NOP, 4,    X_INDEXED_ZERO_PAGE },                // *
    { *ORA, 4,    X_INDEXED_ZERO_PAGE },
    { *ASL, 6,    X_INDEXED_ZERO_PAGE },
    { *SLO, 6,    X_INDEXED_ZERO_PAGE },                // *
    { *CLC, 2,    IMPLIED },
    { *ORA, 4,    Y_INDEXED_ABSOLUTE },
    { *NOP, 2,    IMPLIED },                            // *
    { *SLO, 7,    Y_INDEXED_ABSOLUTE },                 // *
    { *NOP, 4,    X_INDEXED_ABSOLUTE },                 // *
    { *ORA, 4,    X_INDEXED_ABSOLUTE },
    { *ASL, 7,    X_INDEXED_ABSOLUTE },
    { *SLO, 7,    X_INDEXED_ABSOLUTE },                 // *
    // 0x20 to 0x2F
    { *JSR, 6,    ABSOLUTE },
    { *AND, 6,    X_INDEXED_ZERO_PAGE_INDIRECT },
    { *JAM, -1,   NOTHING },                            // *
    { *RLA, 8,    X_INDEXED_ZERO_PAGE_INDIRECT },       // *
    { *BIT, 3,    ZERO_PAGE },
    { *AND, 3,    ZERO_PAGE },
    { *ROL, 5,    ZERO_PAGE },
    { *RLA, 5,    ZERO_PAGE },                          // *
    { *PLP, 4,    IMPLIED },
    { *AND, 2,    IMMEDIATE },
    { *ROL, 2,    ACCUMULATOR },
    { *ANC, 2,    IMMEDIATE },                          // *
    { *BIT, 4,    ABSOLUTE },
    { *AND, 4,    ABSOLUTE },
    { *ROL, 6,    ABSOLUTE },
    { *RLA, 6,    ABSOLUTE },                           // *
    // 0x30 to 0x3F
    { *BMI, 2,    RELATIVE },
    { *AND, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED },
    { *JAM, -1,   NOTHING },                            // *
    { *RLA, 8,    ZERO_PAGE_INDIRECT_Y_INDEXED },       // *
    { *NOP, 4,    X_INDEXED_ZERO_PAGE },                // *
    { *AND, 4,    X_INDEXED_ZERO_PAGE },
    { *ROL, 6,    X_INDEXED_ZERO_PAGE },
    { *RLA, 6,    X_INDEXED_ZERO_PAGE },                // *
    { *SEC, 2,    IMPLIED },
    { *AND, 4,    Y_INDEXED_ABSOLUTE },
    { *NOP, 2,    IMPLIED },                            // *
    { *RLA, 7,    Y_INDEXED_ABSOLUTE },                 // *
    { *NOP, 4,    X_INDEXED_ABSOLUTE },                 // *
    { *AND, 4,    X_INDEXED_ABSOLUTE },
    { *ROL, 7,    X_INDEXED_ABSOLUTE },
    { *RLA, 7,    X_INDEXED_ABSOLUTE },                 // *
    // 0x40 to 0x4F
    { *RTI, 6,    IMPLIED },
    { *EOR, 6,    X_INDEXED_ZERO_PAGE_INDIRECT },
    { *JAM, -1,   NOTHING },                            // *
    { *SRE, 8,    X_INDEXED_ZERO_PAGE_INDIRECT },       // *
    { *NOP, 3,    ZERO_PAGE },                          // *
    { *EOR, 3,    ZERO_PAGE },
    { *LSR, 5,    ZERO_PAGE },
    { *SRE, 5,    ZERO_PAGE },                          // *
    { *PHA, 3,    IMPLIED },
    { *EOR, 2,    IMMEDIATE },
    { *LSR, 2,    ACCUMULATOR },
    { *ALR, 2,    IMMEDIATE },                          // *
    { *JMP, 2,    ABSOLUTE },
    { *EOR, 4,    ABSOLUTE },
    { *LSR, 6,    ABSOLUTE },
    { *SRE, 6,    ABSOLUTE },                        // *
    // 0x50 to 0x5F
    { *BVC, 2,    RELATIVE },
    { *EOR, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED },
    { *JAM, -1,   NOTHING },                            // *
    { *SRE, 8,    ZERO_PAGE_INDIRECT_Y_INDEXED },       // *
    { *NOP, 4,    X_INDEXED_ZERO_PAGE },                // *
    { *EOR, 4,    X_INDEXED_ZERO_PAGE },
    { *LSR, 6,    X_INDEXED_ZERO_PAGE },
    { *SRE, 6,    X_INDEXED_ZERO_PAGE },                // *
    { *CLI, 2,    IMPLIED },
    { *EOR, 4,    Y_INDEXED_ABSOLUTE },
    { *NOP, 2,    IMPLIED },                            // *
    { *SRE, 7,    Y_INDEXED_ABSOLUTE },                 // *
    { *NOP, 4,    X_INDEXED_ABSOLUTE },                 // *
    { *EOR, 4,    X_INDEXED_ABSOLUTE },
    { *LSR, 7,    X_INDEXED_ABSOLUTE },
    { *SRE, 7,    X_INDEXED_ABSOLUTE },                 // *
    // 0x60 to 0x6F
    { *RTS, 6,    IMPLIED },
    { *ADC, 6,    X_INDEXED_ZERO_PAGE_INDIRECT },
    { *JAM, -1,   NOTHING },                            // *
    { *RRA, 8,    X_INDEXED_ZERO_PAGE_INDIRECT },       // *
    { *NOP, 3,    ZERO_PAGE },                          // *
    { *ADC, 3,    ZERO_PAGE },
    { *ROR, 5,    ZERO_PAGE },
    { *RRA, 5,    ZERO_PAGE },                          // *
    { *PLA, 4,    IMPLIED },
    { *ADC, 2,    IMMEDIATE },
    { *ROR, 2,    ACCUMULATOR },
    { *ARR, 2,    IMMEDIATE },                          // *
    { *JMP, 5,    ABSOLUTE_INDIRECT },
    { *ADC, 4,    ABSOLUTE },
    { *ROR, 6,    ABSOLUTE },
    { *RRA, 6,    ABSOLUTE },                           // *
    // 0x70 to 0x7F
    { *BVS, 2,    RELATIVE },
    { *ADC, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED },
    { *JAM, -1,   NOTHING },                            // *
    { *RRA, 8,    ZERO_PAGE_INDIRECT_Y_INDEXED },       // *
    { *NOP, 4,    X_INDEXED_ZERO_PAGE },                // *
    { *ADC, 4,    X_INDEXED_ZERO_PAGE },
    { *ROR, 6,    X_INDEXED_ZERO_PAGE },
    { *RRA, 6,    X_INDEXED_ZERO_PAGE },                // *
    { *SEI, 2,    IMPLIED },
    { *ADC, 4,    Y_INDEXED_ABSOLUTE },
    { *NOP, 2,    IMPLIED },                            // *
    { *RRA, 7,    Y_INDEXED_ABSOLUTE },                 // *
    { *NOP, 4,    X_INDEXED_ABSOLUTE },                 // *
    { *ADC, 4,    X_INDEXED_ABSOLUTE },
    { *ROR, 7,    X_INDEXED_ABSOLUTE },
    { *RRA, 7,    X_INDEXED_ABSOLUTE },                 // *
    // 0x80 to 0x8F
    { *NOP, 2,    IMMEDIATE },                          // *
    { *STA, 6,    X_INDEXED_ZERO_PAGE_INDIRECT },
    { *NOP, 2,    IMMEDIATE },                          // *
    { *SAX, 6,    X_INDEXED_ZERO_PAGE_INDIRECT },       // *
    { *STY, 3,    ZERO_PAGE },
    { *STA, 3,    ZERO_PAGE },
    { *STX, 3,    ZERO_PAGE },
    { *SAX, 3,    ZERO_PAGE },                          // *
    { *DEY, 2,    IMPLIED },
    { *NOP, 2,    IMMEDIATE },                          // *
    { *TXA, 2,    IMPLIED },
    { *XXA, 2,    IMMEDIATE },                          // *
    { *STY, 4,    ABSOLUTE },
    { *STA, 4,    ABSOLUTE },
    { *STX, 4,    ABSOLUTE },
    { *SAX, 4,    ABSOLUTE },                           // *
    // 0x90 to 0x9F
    { *BCC, 2,    RELATIVE },
    { *STA, 6,    ZERO_PAGE_INDIRECT_Y_INDEXED },
    { *JAM, -1,   NOTHING },                            // *
    { *SHA, 6,    ZERO_PAGE_INDIRECT_Y_INDEXED },       // *
    { *STY, 4,    X_INDEXED_ZERO_PAGE },
    { *STA, 4,    X_INDEXED_ZERO_PAGE },
    { *STX, 4,    Y_INDEXED_ZERO_PAGE },
    { *SAX, 4,    Y_INDEXED_ZERO_PAGE },                // *
    { *TYA, 2,    IMPLIED },
    { *STA, 5,    Y_INDEXED_ABSOLUTE },
    { *TXS, 2,    IMPLIED },
    { *TAS, 5,    Y_INDEXED_ABSOLUTE },                 // *
    { *SHY, 5,    X_INDEXED_ABSOLUTE },                 // *
    { *STA, 5,    X_INDEXED_ABSOLUTE },
    { *SHX, 5,    Y_INDEXED_ABSOLUTE },                 // *
    { *SHA, 5,    Y_INDEXED_ABSOLUTE },                 // *
    // 0xA0 to 0xAF
    { *LDY, 2,    IMMEDIATE },
    { *LDA, 6,    X_INDEXED_ZERO_PAGE_INDIRECT },
    { *LDX, 2,    IMMEDIATE },
    { *LAX, 6,    X_INDEXED_ZERO_PAGE_INDIRECT },       // *
    { *LDY, 3,    ZERO_PAGE },
    { *LDA, 3,    ZERO_PAGE },
    { *LDX, 3,    ZERO_PAGE },
    { *LAX, 3,    ZERO_PAGE },                          // *
    { *TAY, 2,    IMPLIED },
    { *LDA, 2,    IMMEDIATE },
    { *TAX, 2,    IMPLIED },
    { *LXA, 2,    IMMEDIATE },                          // *
    { *LDY, 4,    ABSOLUTE },
    { *LDA, 4,    ABSOLUTE },
    { *LDX, 4,    ABSOLUTE },
    { *LAX, 4,    ABSOLUTE },                           // *
    // 0xB0 to 0xBF
    { *BCS, 2,    RELATIVE },
    { *LDA, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED },
    { *JAM, -1,   NOTHING },                            // *
    { *LAX, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED },       // *
    { *LDY, 4,    X_INDEXED_ZERO_PAGE },
    { *LDA, 4,    X_INDEXED_ZERO_PAGE },
    { *LDX, 4,    Y_INDEXED_ZERO_PAGE },
    { *LAX, 4,    Y_INDEXED_ZERO_PAGE },                // *
    { *CLV, 2,    IMPLIED },
    { *LDA, 4,    Y_INDEXED_ABSOLUTE },
    { *TSX, 2,    IMPLIED },
    { *LAS, 4,    Y_INDEXED_ABSOLUTE },                 // *
    { *LDY, 4,    X_INDEXED_ABSOLUTE },
    { *LDA, 4,    X_INDEXED_ABSOLUTE },
    { *LDX, 4,    Y_INDEXED_ABSOLUTE },
    { *LAX, 4,    Y_INDEXED_ABSOLUTE },                 // *
    // 0xC0 to 0xCF
    { *CPY, 2,    IMMEDIATE },
    { *CMP, 6,    X_INDEXED_ZERO_PAGE_INDIRECT },
    { *NOP, 2,    IMMEDIATE },                          // *
    { *DCP, 8,    X_INDEXED_ZERO_PAGE_INDIRECT },       // *
    { *CPY, 3,    ZERO_PAGE },
    { *CMP, 3,    ZERO_PAGE },
    { *DEC, 5,    ZERO_PAGE },
    { *DCP, 5,    ZERO_PAGE },                          // *
    { *INY, 2,    IMPLIED },
    { *CMP, 2,    IMMEDIATE },
    { *DEX, 2,    IMPLIED },
    { *SBX, 2,    IMMEDIATE },                          // *
    { *CPY, 4,    ABSOLUTE },
    { *CMP, 4,    ABSOLUTE },
    { *DEC, 6,    ABSOLUTE },
    { *DCP, 6,    ABSOLUTE },                           // *
    // 0xD0 to 0xDF
    { *BNE, 2,    RELATIVE },
    { *CMP, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED },
    { *JAM, -1,   NOTHING },                            // *
    { *DCP, 8,    ZERO_PAGE_INDIRECT_Y_INDEXED },       // *
    { *NOP, 4,    X_INDEXED_ZERO_PAGE },                // *
    { *CMP, 3,    ZERO_PAGE },
    { *DEC, 6,    X_INDEXED_ZERO_PAGE },
    { *DCP, 6,    X_INDEXED_ZERO_PAGE },                // *
    { *CLD, 2,    IMPLIED },
    { *CMP, 4,    Y_INDEXED_ABSOLUTE },
    { *NOP, 2,    IMPLIED },                            // *
    { *DCP, 7,    Y_INDEXED_ABSOLUTE },                 // *
    { *NOP, 4,    X_INDEXED_ABSOLUTE },                 // *
    { *CMP, 4,    X_INDEXED_ABSOLUTE },
    { *DEC, 7,    X_INDEXED_ABSOLUTE },
    { *DCP, 7,    X_INDEXED_ABSOLUTE },                 // *
    // 0xE0 to 0xEF
    { *CPX, 2,    IMMEDIATE },
    { *SBC, 6,    X_INDEXED_ZERO_PAGE_INDIRECT },
    { *NOP, 2,    IMMEDIATE },                          // *
    { *ISC, 8,    X_INDEXED_ZERO_PAGE_INDIRECT },       // *
    { *CPX, 3,    ZERO_PAGE },
    { *SBC, 3,    ZERO_PAGE },
    { *INC, 5,    ZERO_PAGE },
    { *ISC, 5,    ZERO_PAGE },                          // *
    { *INX, 2,    IMPLIED },
    { *SBC, 2,    IMMEDIATE },
    { *NOP, 2,    IMPLIED },
    { *USBC,2,    IMMEDIATE },                          // *
    { *CPX, 4,    ABSOLUTE },
    { *SBC, 4,    ABSOLUTE },
    { *INC, 6,    ABSOLUTE },
    { *ISC, 6,    ABSOLUTE },                           // *
    // 0xF0 to 0xFF
    { *BEQ, 2,    RELATIVE },
    { *SBC, 5,    ZERO_PAGE_INDIRECT_Y_INDEXED },
    { *JAM, -1,   NOTHING },                            // *
    { *ISC, 8,    ZERO_PAGE_INDIRECT_Y_INDEXED },       // *
    { *NOP, 4,    X_INDEXED_ZERO_PAGE },                // *
    { *SBC, 4,    X_INDEXED_ZERO_PAGE },
    { *INC, 6,    X_INDEXED_ZERO_PAGE },
    { *ISC, 6,    X_INDEXED_ZERO_PAGE },                // *
    { *SED, 2,    IMPLIED },
    { *SBC, 4,    Y_INDEXED_ABSOLUTE },
    { *NOP, 2,    IMPLIED },                            // *
    { *ISC, 7,    Y_INDEXED_ABSOLUTE },                 // *
    { *NOP, 4,    X_INDEXED_ABSOLUTE },                 // *
    { *SBC, 4,    X_INDEXED_ABSOLUTE },
    { *INC, 7,    X_INDEXED_ABSOLUTE },
    { *ISC, 7,    X_INDEXED_ABSOLUTE },                 // *
};

unsigned char set_bit(unsigned char d, char i) {
    return d | (1 << i);
}

int main() {
    cpu main;

    main.A  =
    main.X  =
    main.Y  =
    main.SR =
    main.PC = 0;
    
    main.quit = false;

    /* Allocate memory block and registers */
    /* 6502 could access up to 65535 bytes (64K) of memory */
    /* This is not the 6510, shut the fuck up */
    main.mem = calloc(MAX_MEM_SIZE, 1);

    if (main.mem == NULL) return -1;
    main.PC = main.mem[RESET_VEC];
    
    instructionTable[main.mem[main.PC]].InstructionPointer
    // TODO: Do this
    (instructionTable[main.mem[main.PC]].mode, instructionTable[main.mem[main.PC]].cycles, NULL, NULL);
    // TODO: Do shit

    free(main.mem);
    return 0;
}