#ifndef DJKFDJ_DNN3JFND_dNEJNDSMN
#define DJKFDJ_DNN3JFND_dNEJNDSMN
#include "../config.h"
#include "hash.h"

#if defined (INCLUDE_ILLEGAL_OPCODES)

static MnemonicInfo mnemonics[75] = {
    { "LAS", {
        {Y_INDEXED_ABSOLUTE, 0xBB}
    }, 1 },
    { "LAX", {
        {IMMEDIATE                   , 0xAB},
        {ABSOLUTE                    , 0xAF},
        {Y_INDEXED_ABSOLUTE          , 0xBF},
        {ZERO_PAGE                   , 0xA7},
        {Y_INDEXED_ZERO_PAGE         , 0xB7},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0xA3},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0xB3} },
        7 },
    { "LDA", /* Mnemonic */ {
        {IMMEDIATE                   , 0xA9}, /* Addressing modes */
        {ABSOLUTE                    , 0xAD}, /* And their opcodes */
        {X_INDEXED_ABSOLUTE          , 0xBD},
        {Y_INDEXED_ABSOLUTE          , 0xB9},
        {ZERO_PAGE                   , 0xA5},
        {X_INDEXED_ZERO_PAGE         , 0xB5},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0xA1},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0xB1} },
        8 /* Number of entries in addr mode/opcode array above */ },
    { "LDX", {
        {IMMEDIATE          , 0xA2},
        {ABSOLUTE           , 0xAE},
        {Y_INDEXED_ABSOLUTE , 0xBE},
        {ZERO_PAGE          , 0xA6},
        {Y_INDEXED_ZERO_PAGE, 0xB6} },
        5 },
    { "LDY", {
        {IMMEDIATE          , 0xA0},
        {ABSOLUTE           , 0xAC},
        {X_INDEXED_ABSOLUTE , 0xBC},
        {ZERO_PAGE          , 0xA4},
        {X_INDEXED_ZERO_PAGE, 0xB4} },
        5 },
    { "SAX", {
        {ABSOLUTE                    , 0x8F},
        {ZERO_PAGE                   , 0x87},
        {Y_INDEXED_ZERO_PAGE         , 0x97},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0x83} },
        4 },
    { "SHA", {
        {Y_INDEXED_ABSOLUTE          , 0x9F},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0x93} },
        2 },
    { "SHX", {
        {Y_INDEXED_ABSOLUTE, 0x9E} },
        1 },
    { "SHY", {
        {Y_INDEXED_ABSOLUTE, 0x9C} },
        1 },
    { "STA", {
        {ABSOLUTE                    , 0x8D},
        {X_INDEXED_ABSOLUTE          , 0x9D},
        {Y_INDEXED_ABSOLUTE          , 0x99},
        {ZERO_PAGE                   , 0x85},
        {X_INDEXED_ZERO_PAGE         , 0x95},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0x91},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0x91} },
        7 },
    { "STX", {
        {ABSOLUTE           , 0x8E},
        {ZERO_PAGE          , 0x86},
        {Y_INDEXED_ZERO_PAGE, 0x96} },
        3 },
    { "STY", {
        {ABSOLUTE           , 0x8C},
        {ZERO_PAGE          , 0x84},
        {X_INDEXED_ZERO_PAGE, 0x94} },
        3 },
    { "SHS", {
        {Y_INDEXED_ABSOLUTE, 0x9B} },
        1 },
    { "TAX", {
        {IMPLIED, 0xAA} },
        1 },
    { "TAY", { 
        {IMPLIED, 0xA8} }, 
        1 },
    { "TSX", {
        {IMPLIED, 0xBA} },
        1 },
    { "TXA", {
        {IMPLIED, 0x8A} },
        1 },
    { "TXS", {
        {IMPLIED, 0x9A} },
        1 },
    { "TYA", {
        {IMPLIED, 0x98} },
        1 },
    { "PHA", {
        {IMPLIED, 0x48} },
        1 },
    { "PHP", {
        {IMPLIED, 0x08} },
        1 },
    { "PLA", {
        {IMPLIED, 0x68} },
        1 },
    { "PLP", {
        {IMPLIED, 0x28} },
        1 },
    { "ASL", {
        {ACCUMULATOR        , 0x0A},
        {ABSOLUTE           , 0x0E},
        {X_INDEXED_ABSOLUTE , 0x1E},
        {ZERO_PAGE          , 0x06},
        {X_INDEXED_ZERO_PAGE, 0x16} },
        5 },
    { "LSR", {
        {ACCUMULATOR        , 0x4A},
        {ABSOLUTE           , 0x4E},
        {X_INDEXED_ABSOLUTE , 0x5E},
        {ZERO_PAGE          , 0x46},
        {X_INDEXED_ZERO_PAGE, 0x56} },
        5 },
    { "ROL", {
        {ACCUMULATOR        , 0x2A},
        {ABSOLUTE           , 0x2E},
        {X_INDEXED_ABSOLUTE , 0x3E},
        {ZERO_PAGE          , 0x26},
        {X_INDEXED_ZERO_PAGE, 0x36} },
        5 },
    { "ROR", {
        {ACCUMULATOR        , 0x6A},
        {ABSOLUTE           , 0x6E},
        {X_INDEXED_ABSOLUTE , 0x7E},
        {ZERO_PAGE          , 0x66},
        {X_INDEXED_ZERO_PAGE, 0x76} },
        5 },
    { "AND", {
        {IMMEDIATE                   , 0x29},
        {ABSOLUTE                    , 0x2D},
        {X_INDEXED_ABSOLUTE          , 0x3D},
        {Y_INDEXED_ABSOLUTE          , 0x39},
        {ZERO_PAGE                   , 0x25},
        {X_INDEXED_ZERO_PAGE         , 0x35},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0x21},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0x31} },
        8 },
    { "BIT", {
        {ABSOLUTE , 0x2C},
        {ZERO_PAGE, 0X24} },
        2 },
    { "EOR", {
        {IMMEDIATE                   , 0x49},
        {ABSOLUTE                    , 0x4D},
        {X_INDEXED_ABSOLUTE          , 0x5D},
        {Y_INDEXED_ABSOLUTE          , 0x59},
        {ZERO_PAGE                   , 0x45},
        {X_INDEXED_ZERO_PAGE         , 0x55},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0x41},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0x51} },
        8 },
    { "ORA", {
        {IMMEDIATE                   , 0x09},
        {ABSOLUTE                    , 0x0D},
        {X_INDEXED_ABSOLUTE          , 0x1D},
        {Y_INDEXED_ABSOLUTE          , 0x19},
        {ZERO_PAGE                   , 0x05},
        {X_INDEXED_ZERO_PAGE         , 0x15},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0x01},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0x11} },
        8 },
    { "ADC", {
        {IMMEDIATE                   , 0x69},
        {ABSOLUTE                    , 0x6D},
        {X_INDEXED_ABSOLUTE          , 0x7D},
        {Y_INDEXED_ABSOLUTE          , 0x79},
        {ZERO_PAGE                   , 0x65},
        {X_INDEXED_ZERO_PAGE         , 0x75},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0x61},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0x71} },
        8 },
    { "ANC", {
        {IMMEDIATE, 0x0B},
        {IMMEDIATE, 0x2B} },
        2 },
    { "ARR", {
        {IMMEDIATE, 0x6B} },
        1 },
    { "ASR", {
        {IMMEDIATE, 0x4B} },
        1 },
    { "CMP", {
        {IMMEDIATE                   , 0xC9},
        {ABSOLUTE                    , 0xCD},
        {X_INDEXED_ABSOLUTE          , 0xDD},
        {Y_INDEXED_ABSOLUTE          , 0xD9},
        {ZERO_PAGE                   , 0xC5},
        {X_INDEXED_ZERO_PAGE         , 0xD5},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0xC1},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0xD1} },
        8 },
    { "CPX", {
        {IMMEDIATE, 0xE0},
        {ABSOLUTE , 0xEC},
        {ZERO_PAGE, 0xE4} },
        3 },
    { "CPY", {
        {IMMEDIATE, 0xC0},
        {ABSOLUTE , 0xCC},
        {ZERO_PAGE, 0xC4} },
        3 },
    { "DCP", {
        {ABSOLUTE                    , 0xCF},
        {X_INDEXED_ABSOLUTE          , 0xDF},
        {Y_INDEXED_ABSOLUTE          , 0xDB},
        {ZERO_PAGE                   , 0xC7},
        {X_INDEXED_ZERO_PAGE         , 0xD7},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0xC3},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0xD3} },
        7 },
    { "ISC", {
        {ABSOLUTE                    , 0xEF},
        {X_INDEXED_ABSOLUTE          , 0xFF},
        {Y_INDEXED_ABSOLUTE          , 0xFB},
        {ZERO_PAGE                   , 0xE7},
        {X_INDEXED_ZERO_PAGE         , 0xF7},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0xE3},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0xF3} },
        7 },
    { "RLA", {
        {ABSOLUTE                    , 0x2F},
        {X_INDEXED_ABSOLUTE          , 0x3F},
        {Y_INDEXED_ABSOLUTE          , 0x3B},
        {ZERO_PAGE                   , 0x27},
        {X_INDEXED_ZERO_PAGE         , 0x37},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0x23},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0x33} },
        7 },
    { "RRA", {
        {ABSOLUTE                    , 0x6F},
        {X_INDEXED_ABSOLUTE          , 0x7F},
        {Y_INDEXED_ABSOLUTE          , 0x7B},
        {ZERO_PAGE                   , 0x67},
        {X_INDEXED_ZERO_PAGE         , 0x77},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0x63},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0x73} },
        7 },
    { "SBC", {
        {IMMEDIATE                   , 0xE9},
        {IMMEDIATE                   , 0xEB},
        {ABSOLUTE                    , 0xED},
        {X_INDEXED_ABSOLUTE          , 0xFD},
        {Y_INDEXED_ABSOLUTE          , 0xF9},
        {ZERO_PAGE                   , 0xE5},
        {X_INDEXED_ZERO_PAGE         , 0xF5},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0xE1},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0xF1} },
        9 },
    { "SBX", {
        {IMMEDIATE, 0xCB} },
        1 },
    { "SLO", {
        {ABSOLUTE                    , 0x0F},
        {X_INDEXED_ABSOLUTE          , 0x1F},
        {Y_INDEXED_ABSOLUTE          , 0x1B},
        {ZERO_PAGE                   , 0x07},
        {X_INDEXED_ZERO_PAGE         , 0x17},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0x03},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0x13} },
        7 },
    { "SRE", {
        {ABSOLUTE                    , 0x4F},
        {X_INDEXED_ABSOLUTE          , 0x5F},
        {Y_INDEXED_ABSOLUTE          , 0x5B},
        {ZERO_PAGE                   , 0x47},
        {X_INDEXED_ZERO_PAGE         , 0x57},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0x43},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0x53} },
        7 },
    { "XXA", {
        {IMMEDIATE, 0x8B} },
        1 },
    { "DEC", {
        {ABSOLUTE           , 0xCE},
        {X_INDEXED_ABSOLUTE , 0xDE},
        {ZERO_PAGE          , 0xC6},
        {X_INDEXED_ZERO_PAGE, 0xD6} },
        4 },
    { "DEX", {
        {IMPLIED, 0xCA} },
        1 },
    { "DEY", {
        {IMPLIED, 0x88} },
        1 },
    { "INC", {
        {ABSOLUTE           , 0xEE},
        {X_INDEXED_ABSOLUTE , 0xFE},
        {ZERO_PAGE          , 0xE6},
        {X_INDEXED_ZERO_PAGE, 0xF6} },
        4 },
    { "INX", {
        {IMPLIED, 0xE8} },
        1 },
    { "INY", {
        {IMPLIED, 0xC8} },
        1 },
    { "BRK", {
        {IMPLIED, 0x00} },
        1 },
    { "JMP", {
        {ABSOLUTE         , 0x4C},
        {ABSOLUTE_INDIRECT, 0x6C} },
        2 },
    { "JSR", {
        {ABSOLUTE, 0x20} },
        1 },
    { "RTI", {
        {IMPLIED, 0x40} },
        1 },
    { "RTS", {
        {IMPLIED, 0x60} },
        1 },
    { "BCC", {
        {RELATIVE, 0x90} },
        1 },
    { "BCS", {
        {RELATIVE, 0xB0} },
        1 },
    { "BEQ", {
        {RELATIVE, 0xF0} },
        1 },
    { "BMI", {
        {RELATIVE, 0x30} },
        1 },
    { "BNE", {
        {RELATIVE, 0xD0} },
        1 },
    { "BPL", {
        {RELATIVE, 0x10} },
        1 },
    { "BVC", {
        {RELATIVE, 0x50} },
        1 },
    { "BVS", {
        {RELATIVE, 0x70} },
        1 },
    { "CLC", {
        {RELATIVE, 0x18} },
        1 },
    { "CLD", {
        {RELATIVE, 0xD8} },
        1 },
    { "CLI", {
        {RELATIVE, 0x58} },
        1 },
    { "CLV", {
        {RELATIVE, 0xB8} },
        1 },
    { "SEC", {
        {RELATIVE, 0x38} },
        1 },
    { "SED", {
        {RELATIVE, 0xF8} },
        1 },
    { "SEI", {
        {RELATIVE, 0x78} },
        1 },
    { "JAM", {
        {IMPLIED, 0x02},
        {IMPLIED, 0x12},
        {IMPLIED, 0x22},
        {IMPLIED, 0x32},
        {IMPLIED, 0x42},
        {IMPLIED, 0x52},
        {IMPLIED, 0x62},
        {IMPLIED, 0x72},
        {IMPLIED, 0x92},
        {IMPLIED, 0xB2},
        {IMPLIED, 0xD2},
        {IMPLIED, 0xF2} },
        12 },
    { "NOP", {
        {IMPLIED            , 0x1A},
        {IMPLIED            , 0x3A},
        {IMPLIED            , 0x5A},
        {IMPLIED            , 0x7A},
        {IMPLIED            , 0xDA},
        {IMPLIED            , 0xEA},
        {IMPLIED            , 0xFA},
        {IMMEDIATE          , 0x80},
        {IMMEDIATE          , 0x82},
        {IMMEDIATE          , 0x89},
        {IMMEDIATE          , 0xC2},
        {IMMEDIATE          , 0xE2},
        {ABSOLUTE           , 0x0C},
        {X_INDEXED_ABSOLUTE , 0x1C},
        {X_INDEXED_ABSOLUTE , 0x3C},
        {X_INDEXED_ABSOLUTE , 0x5C},
        {X_INDEXED_ABSOLUTE , 0x7C},
        {X_INDEXED_ABSOLUTE , 0xDC},
        {X_INDEXED_ABSOLUTE , 0xFC},
        {ZERO_PAGE          , 0x04},
        {ZERO_PAGE          , 0x44},
        {ZERO_PAGE          , 0x64},
        {X_INDEXED_ZERO_PAGE, 0x14},
        {X_INDEXED_ZERO_PAGE, 0x34},
        {X_INDEXED_ZERO_PAGE, 0x54},
        {X_INDEXED_ZERO_PAGE, 0x74},
        {X_INDEXED_ZERO_PAGE, 0xD4},
        {X_INDEXED_ZERO_PAGE, 0xF4}, },
        28 }
};

#else

static MnemonicInfo mnemonics[57] = {
    { "LDA", /* Mnemonic */ {
        {IMMEDIATE                   , 0xA9}, /* Addressing modes */
        {ABSOLUTE                    , 0xAD}, /* And their opcodes */
        {X_INDEXED_ABSOLUTE          , 0xBD},
        {Y_INDEXED_ABSOLUTE          , 0xB9},
        {ZERO_PAGE                   , 0xA5},
        {X_INDEXED_ZERO_PAGE         , 0xB5},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0xA1},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0xB1} },
        8 /* Number of entries in addr mode/opcode array above */ },
    { "LDX", {
        {IMMEDIATE          , 0xA2},
        {ABSOLUTE           , 0xAE},
        {Y_INDEXED_ABSOLUTE , 0xBE},
        {ZERO_PAGE          , 0xA6},
        {Y_INDEXED_ZERO_PAGE, 0xB6} },
        5 },
    { "LDY", {
        {IMMEDIATE          , 0xA0},
        {ABSOLUTE           , 0xAC},
        {X_INDEXED_ABSOLUTE , 0xBC},
        {ZERO_PAGE          , 0xA4},
        {X_INDEXED_ZERO_PAGE, 0xB4} },
        5 },
    { "STA", {
        {ABSOLUTE                    , 0x8D},
        {X_INDEXED_ABSOLUTE          , 0x9D},
        {Y_INDEXED_ABSOLUTE          , 0x99},
        {ZERO_PAGE                   , 0x85},
        {X_INDEXED_ZERO_PAGE         , 0x95},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0x91},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0x91} },
        7 },
    { "STX", {
        {ABSOLUTE           , 0x8E},
        {ZERO_PAGE          , 0x86},
        {Y_INDEXED_ZERO_PAGE, 0x96} },
        3 },
    { "STY", {
        {ABSOLUTE           , 0x8C},
        {ZERO_PAGE          , 0x84},
        {X_INDEXED_ZERO_PAGE, 0x94} },
        3 },
    { "TAX", {
        {IMPLIED, 0xAA} },
        1 },
    { "TAY", { 
        {IMPLIED, 0xA8} }, 
        1 },
    { "TSX", {
        {IMPLIED, 0xBA} },
        1 },
    { "TXA", {
        {IMPLIED, 0x8A} },
        1 },
    { "TXS", {
        {IMPLIED, 0x9A} },
        1 },
    { "TYA", {
        {IMPLIED, 0x98} },
        1 },
    { "PHA", {
        {IMPLIED, 0x48} },
        1 },
    { "PHP", {
        {IMPLIED, 0x08} },
        1 },
    { "PLA", {
        {IMPLIED, 0x68} },
        1 },
    { "PLP", {
        {IMPLIED, 0x28} },
        1 },
    { "ASL", {
        {ACCUMULATOR        , 0x0A},
        {ABSOLUTE           , 0x0E},
        {X_INDEXED_ABSOLUTE , 0x1E},
        {ZERO_PAGE          , 0x06},
        {X_INDEXED_ZERO_PAGE, 0x16} },
        5 },
    { "LSR", {
        {ACCUMULATOR        , 0x4A},
        {ABSOLUTE           , 0x4E},
        {X_INDEXED_ABSOLUTE , 0x5E},
        {ZERO_PAGE          , 0x46},
        {X_INDEXED_ZERO_PAGE, 0x56} },
        5 },
    { "ROL", {
        {ACCUMULATOR        , 0x2A},
        {ABSOLUTE           , 0x2E},
        {X_INDEXED_ABSOLUTE , 0x3E},
        {ZERO_PAGE          , 0x26},
        {X_INDEXED_ZERO_PAGE, 0x36} },
        5 },
    { "ROR", {
        {ACCUMULATOR        , 0x6A},
        {ABSOLUTE           , 0x6E},
        {X_INDEXED_ABSOLUTE , 0x7E},
        {ZERO_PAGE          , 0x66},
        {X_INDEXED_ZERO_PAGE, 0x76} },
        5 },
    { "AND", {
        {IMMEDIATE                   , 0x29},
        {ABSOLUTE                    , 0x2D},
        {X_INDEXED_ABSOLUTE          , 0x3D},
        {Y_INDEXED_ABSOLUTE          , 0x39},
        {ZERO_PAGE                   , 0x25},
        {X_INDEXED_ZERO_PAGE         , 0x35},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0x21},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0x31} },
        8 },
    { "BIT", {
        {ABSOLUTE , 0x2C},
        {ZERO_PAGE, 0X24} },
        2 },
    { "EOR", {
        {IMMEDIATE                   , 0x49},
        {ABSOLUTE                    , 0x4D},
        {X_INDEXED_ABSOLUTE          , 0x5D},
        {Y_INDEXED_ABSOLUTE          , 0x59},
        {ZERO_PAGE                   , 0x45},
        {X_INDEXED_ZERO_PAGE         , 0x55},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0x41},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0x51} },
        8 },
    { "ORA", {
        {IMMEDIATE                   , 0x09},
        {ABSOLUTE                    , 0x0D},
        {X_INDEXED_ABSOLUTE          , 0x1D},
        {Y_INDEXED_ABSOLUTE          , 0x19},
        {ZERO_PAGE                   , 0x05},
        {X_INDEXED_ZERO_PAGE         , 0x15},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0x01},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0x11} },
        8 },
    { "ADC", {
        {IMMEDIATE                   , 0x69},
        {ABSOLUTE                    , 0x6D},
        {X_INDEXED_ABSOLUTE          , 0x7D},
        {Y_INDEXED_ABSOLUTE          , 0x79},
        {ZERO_PAGE                   , 0x65},
        {X_INDEXED_ZERO_PAGE         , 0x75},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0x61},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0x71} },
        8 },
    { "CMP", {
        {IMMEDIATE                   , 0xC9},
        {ABSOLUTE                    , 0xCD},
        {X_INDEXED_ABSOLUTE          , 0xDD},
        {Y_INDEXED_ABSOLUTE          , 0xD9},
        {ZERO_PAGE                   , 0xC5},
        {X_INDEXED_ZERO_PAGE         , 0xD5},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0xC1},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0xD1} },
        8 },
    { "CPX", {
        {IMMEDIATE, 0xE0},
        {ABSOLUTE , 0xEC},
        {ZERO_PAGE, 0xE4} },
        3 },
    { "CPY", {
        {IMMEDIATE, 0xC0},
        {ABSOLUTE , 0xCC},
        {ZERO_PAGE, 0xC4} },
        3 },
    { "SBC", {
        {IMMEDIATE                   , 0xE9},
        {ABSOLUTE                    , 0xED},
        {X_INDEXED_ABSOLUTE          , 0xFD},
        {Y_INDEXED_ABSOLUTE          , 0xF9},
        {ZERO_PAGE                   , 0xE5},
        {X_INDEXED_ZERO_PAGE         , 0xF5},
        {X_INDEXED_ZERO_PAGE_INDIRECT, 0xE1},
        {ZERO_PAGE_INDIRECT_Y_INDEXED, 0xF1} },
        8 },
    { "DEC", {
        {ABSOLUTE           , 0xCE},
        {X_INDEXED_ABSOLUTE , 0xDE},
        {ZERO_PAGE          , 0xC6},
        {X_INDEXED_ZERO_PAGE, 0xD6} },
        4 },
    { "DEX", {
        {IMPLIED, 0xCA} },
        1 },
    { "DEY", {
        {IMPLIED, 0x88} },
        1 },
    { "INC", {
        {ABSOLUTE           , 0xEE},
        {X_INDEXED_ABSOLUTE , 0xFE},
        {ZERO_PAGE          , 0xE6},
        {X_INDEXED_ZERO_PAGE, 0xF6} },
        4 },
    { "INX", {
        {IMPLIED, 0xE8} },
        1 },
    { "INY", {
        {IMPLIED, 0xC8} },
        1 },
    { "BRK", {
        {IMPLIED, 0x00} },
        1 },
    { "JMP", {
        {ABSOLUTE         , 0x4C},
        {ABSOLUTE_INDIRECT, 0x6C} },
        2 },
    { "JSR", {
        {ABSOLUTE, 0x20} },
        1 },
    { "RTI", {
        {IMPLIED, 0x40} },
        1 },
    { "RTS", {
        {IMPLIED, 0x60} },
        1 },
    { "BCC", {
        {RELATIVE, 0x90} },
        1 },
    { "BCS", {
        {RELATIVE, 0xB0} },
        1 },
    { "BEQ", {
        {RELATIVE, 0xF0} },
        1 },
    { "BMI", {
        {RELATIVE, 0x30} },
        1 },
    { "BNE", {
        {RELATIVE, 0xD0} },
        1 },
    { "BPL", {
        {RELATIVE, 0x10} },
        1 },
    { "BVC", {
        {RELATIVE, 0x50} },
        1 },
    { "BVS", {
        {RELATIVE, 0x70} },
        1 },
    { "CLC", {
        {RELATIVE, 0x18} },
        1 },
    { "CLD", {
        {RELATIVE, 0xD8} },
        1 },
    { "CLI", {
        {RELATIVE, 0x58} },
        1 },
    { "CLV", {
        {RELATIVE, 0xB8} },
        1 },
    { "SEC", {
        {RELATIVE, 0x38} },
        1 },
    { "SED", {
        {RELATIVE, 0xF8} },
        1 },
    { "SEI", {
        {RELATIVE, 0x78} },
        1 },
    { "NOP", {
        {IMPLIED, 0xEA}, },
        1 }
};

#endif
#endif /* DJKFDJ_DNN3JFND_dNEJNDSMN */