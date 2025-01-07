#pragma once

#include "include.cpp"
#include <set>
#include <map>
#include <string>

std::map<Addr_Modes, int> literal_position = {
    {Addr_Modes::IMPLIED, 0},
    {Addr_Modes::ACCUMULATOR, 0},
    {Addr_Modes::IMMEDIATE, 2},
    {Addr_Modes::BASIC_ARGUMENT, 1},
  //{Addr_Modes::ABSOLUTE, 1},
    {Addr_Modes::X_INDEXED_ABSOLUTE, 1},
    {Addr_Modes::Y_INDEXED_ABSOLUTE, 1},
    {Addr_Modes::ABSOLUTE_INDIRECT, 2},
  //{Addr_Modes::ZERO_PAGE, 1},
    {Addr_Modes::X_INDEXED_ZERO_PAGE, 1},
    {Addr_Modes::Y_INDEXED_ZERO_PAGE, 1},
    {Addr_Modes::X_INDEXED_ZERO_PAGE_INDIRECT, 2},
    {Addr_Modes::ZERO_PAGE_INDIRECT_Y_INDEXED, 2},
}; // Position of argument in token vector

std::map<Addr_Modes, int> arg_types = {
    {Addr_Modes::IMPLIED, 0},
    {Addr_Modes::ACCUMULATOR, 0},
    {Addr_Modes::IMMEDIATE, 8},
    {Addr_Modes::ABSOLUTE, 16},
    {Addr_Modes::X_INDEXED_ABSOLUTE, 16},
    {Addr_Modes::Y_INDEXED_ABSOLUTE, 16},
    {Addr_Modes::ABSOLUTE_INDIRECT, 16},
    {Addr_Modes::ZERO_PAGE, 8},
    {Addr_Modes::X_INDEXED_ZERO_PAGE, 8},
    {Addr_Modes::Y_INDEXED_ZERO_PAGE, 8},
    {Addr_Modes::X_INDEXED_ZERO_PAGE_INDIRECT, 8},
    {Addr_Modes::ZERO_PAGE_INDIRECT_Y_INDEXED, 8},
};

std::map<Addr_Modes, int> size_in_bytes = {
    {Addr_Modes::IMPLIED, 1},
    {Addr_Modes::ACCUMULATOR, 1},
    {Addr_Modes::IMMEDIATE, 2},
    {Addr_Modes::ABSOLUTE, 3},
    {Addr_Modes::X_INDEXED_ABSOLUTE, 3},
    {Addr_Modes::Y_INDEXED_ABSOLUTE, 3},
    {Addr_Modes::ABSOLUTE_INDIRECT, 3},
    {Addr_Modes::ZERO_PAGE, 2},
    {Addr_Modes::X_INDEXED_ZERO_PAGE, 2},
    {Addr_Modes::Y_INDEXED_ZERO_PAGE, 2},
    {Addr_Modes::X_INDEXED_ZERO_PAGE_INDIRECT, 2},
    {Addr_Modes::ZERO_PAGE_INDIRECT_Y_INDEXED, 2},
};

std::map<std::string, struct instruction> instruction_info = {
    {
        "LAS",
        {
            .modes = {Addr_Modes::Y_INDEXED_ABSOLUTE,},
            .bytecodes = {0xBB,},
            .is_undocumented = {true,},
        }
    },
    {
        "LAX",
        {
            .modes = {Addr_Modes::IMMEDIATE, Addr_Modes::ABSOLUTE, Addr_Modes::Y_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::Y_INDEXED_ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE_INDIRECT, Addr_Modes::ZERO_PAGE_INDIRECT_Y_INDEXED,},
            .bytecodes = {0xAB, 0xAF, 0xBF, 0xA7, 0xB7, 0xA3, 0xB3,},
            .is_undocumented = {true, true, true, true, true, true, true,},
        }
    },
    {
        "LDA",
        {
            .modes = {Addr_Modes::IMMEDIATE, Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::Y_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE_INDIRECT, Addr_Modes::ZERO_PAGE_INDIRECT_Y_INDEXED,},
            .bytecodes = {0xA9, 0xAD, 0xBD, 0xB9, 0xA5, 0xB5, 0xA1, 0xB1,},
            .is_undocumented = {false, false, false, false, false, false, false, false},
        }
    },
    {
        "LDX",
        {
            .modes = {Addr_Modes::IMMEDIATE, Addr_Modes::ABSOLUTE, Addr_Modes::Y_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::Y_INDEXED_ZERO_PAGE,},
            .bytecodes = {0xA2, 0xAE, 0xBE, 0xA6, 0xB6,},
            .is_undocumented = {false, false, false, false, false,},
        }
    },
    {
        "LDY",
        {
            .modes = {Addr_Modes::IMMEDIATE, Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE,},
            .bytecodes = {0xA0, 0xAC, 0xBC, 0xA4, 0xB4,},
            .is_undocumented = {false, false, false, false, false,},
        }
    },
    {
        "SAX",
        {
            .modes = {Addr_Modes::ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::Y_INDEXED_ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE_INDIRECT,},
            .bytecodes = {0x8F, 0x87, 0x97, 0x83,},
            .is_undocumented = {true, true, true, true,},
        }
    },
    {
        "SHA",
        {
            .modes = {Addr_Modes::Y_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE_INDIRECT_Y_INDEXED,},
            .bytecodes = {0x9F, 0x93,},
            .is_undocumented = {true, true,},
        }
    },
    {
        "SHX",
        {
            .modes = {Addr_Modes::Y_INDEXED_ABSOLUTE,},
            .bytecodes = {0x9E,},
            .is_undocumented = {true,},
        }
    },
    {
        "SHY",
        {
            .modes = {Addr_Modes::X_INDEXED_ABSOLUTE,},
            .bytecodes = {0x9C,},
            .is_undocumented = {true,},
        }
    },
    {
        "STA",
        {
            .modes = {Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::Y_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE_INDIRECT, Addr_Modes::ZERO_PAGE_INDIRECT_Y_INDEXED,},
            .bytecodes = {0x8D, 0x9D, 0x99, 0x85, 0x95, 0x81, 0x91,},
            .is_undocumented = {false, false, false, false, false, false, false,},
        }
    },
    {
        "STX",
        {
            .modes = {Addr_Modes::ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE,},
            .bytecodes = {0x8E, 0x86, 0x96,},
            .is_undocumented = {false, false, false,},
        }
    },
    {
        "STY",
        {
            .modes = {Addr_Modes::ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE,},
            .bytecodes = {0x8C, 0x84, 0x94,},
            .is_undocumented = {false, false, false,},
        }
    },
    {
        "SHS",
        {
            .modes = {Addr_Modes::Y_INDEXED_ABSOLUTE,},
            .bytecodes = {0x9B,},
            .is_undocumented = {true,},
        }
    },
    {
        "TAX",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes = {0xAA,},
            .is_undocumented = {false,},
        }
    },
    {
        "TAY",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes = {0xA8,},
            .is_undocumented = {false,},
        }
    },
    {
        "TSX",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes = {0x9A,},
            .is_undocumented = {false,},
        }
    },
    {
        "TYA",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes = {0x98,},
            .is_undocumented = {false,},
        }
    },
    {
        "PHA",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes = {0x48,},
            .is_undocumented = {false,},
        }
    },
    {
        "PHP",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes = {0xAA,},
            .is_undocumented = {false,},
        }
    },
    {
        "PLA",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes = {0x68,},
            .is_undocumented = {false,},
        }
    },
    {
        "PLP",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes = {0x28,},
            .is_undocumented = {false,},
        }
    },
    {
        "ASL",
        {
            .modes = {Addr_Modes::ACCUMULATOR, Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE,},
            .bytecodes = {0x0A, 0x0E, 0x1E, 0x06, 0x16,},
            .is_undocumented = {false, false, false, false, false,},
        }
    },
    {
        "LSR",
        {
            .modes = {Addr_Modes::ACCUMULATOR, Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE,},
            .bytecodes = {0x4A, 0x4E, 0x5E, 0x46, 0x56,},
            .is_undocumented = {false, false, false, false, false,},
        }
    },
    {
        "ROL",
        {
           .modes = {Addr_Modes::ACCUMULATOR, Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE,},
           .bytecodes = {0x2A, 0x2E, 0x3E, 0x26, 0x36,},
           .is_undocumented = {false, false, false, false, false,},
        }
    },
    {
        "ROR",
        {
            .modes = {Addr_Modes::ACCUMULATOR, Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE,},
            .bytecodes = {0x6A, 0x6E, 0x7E, 0x66, 0x76,},
            .is_undocumented = {false, false, false, false, false,},
        }
    },
    {
        "AND",
        {
            .modes = {Addr_Modes::IMMEDIATE, Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::Y_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE_INDIRECT, Addr_Modes::ZERO_PAGE_INDIRECT_Y_INDEXED,},
            .bytecodes = {0x29, 0x2D, 0x3D, 0x39, 0x25, 0x35, 0x21, 0x31,},
            .is_undocumented = {false, false, false, false, false, false, false, false,},
        }
    },
    {
        "BIT",
        {
            .modes = {Addr_Modes::ABSOLUTE, Addr_Modes::ZERO_PAGE,},
            .bytecodes = {0x2C, 0x24,},
            .is_undocumented = {false, false,},
        }
    },
    {
        "EOR",
        {
            .modes = {Addr_Modes::IMMEDIATE, Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::Y_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE_INDIRECT, Addr_Modes::ZERO_PAGE_INDIRECT_Y_INDEXED,},
            .bytecodes = {0x49, 0x4D, 0x5D, 0x59, 0x45, 0x55, 0x41, 0x51,},
            .is_undocumented = {false, false, false, false, false, false, false, false,},
        }
    },
    {
        "ORA",
        {
            .modes = {Addr_Modes::IMMEDIATE, Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::Y_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE_INDIRECT, Addr_Modes::ZERO_PAGE_INDIRECT_Y_INDEXED,},
            .bytecodes = {0x09, 0x0D, 0x1D, 0x19, 0x05, 0x15, 0x01, 0x11,},
            .is_undocumented = {false, false, false, false, false, false, false, false,},
        }
    },
    {
        "ADC",
        {
            .modes = {Addr_Modes::IMMEDIATE, Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::Y_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE_INDIRECT, Addr_Modes::ZERO_PAGE_INDIRECT_Y_INDEXED,},
            .bytecodes = {0x69, 0x6D, 0x7D, 0x79, 0x65, 0x75, 0x61, 0x71,},
            .is_undocumented = {false, false, false, false, false, false, false, false,},
        }
    },
    {
        "ANC",
        {
            .modes = {Addr_Modes::IMMEDIATE,},
            .bytecodes = {0x0B,},
            .is_undocumented = {true,},
        }
    },
    {
        "ARR",
        {
            .modes = {Addr_Modes::IMMEDIATE,},
            .bytecodes = {0x6B,},
            .is_undocumented = {true,},
        }
    },
    {
        "ASR",
        {
            .modes = {Addr_Modes::IMMEDIATE,},
            .bytecodes = {0x4B,},
            .is_undocumented = {true,},
        }
    },
    {
        "ASR",
        {
            .modes = {Addr_Modes::IMMEDIATE,},
            .bytecodes = {0x4B,},
            .is_undocumented = {true,},
        }
    },
    {
        "CMP",
        {
            .modes = {Addr_Modes::IMMEDIATE, Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::Y_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE_INDIRECT, Addr_Modes::ZERO_PAGE_INDIRECT_Y_INDEXED,},
            .bytecodes = {0xC9, 0xCD, 0xDD, 0xD9, 0xC5, 0xD5, 0xC1, 0xD1,},
            .is_undocumented = {false, false, false, false, false, false, false, false,},
        }
    },
    {
        "CPX",
        {
            .modes = {Addr_Modes::IMMEDIATE, Addr_Modes::ABSOLUTE, Addr_Modes::ZERO_PAGE,},
            .bytecodes = {0xE0, 0xEC, 0xE4,},
            .is_undocumented = {false, false, false,},
        }
    },
    {
        "CPY",
        {
            .modes = {Addr_Modes::IMMEDIATE, Addr_Modes::ABSOLUTE, Addr_Modes::ZERO_PAGE,},
            .bytecodes = {0xC0, 0xCC, 0xC4,},
            .is_undocumented = {false, false, false,},
        }
    },
    {
        "DCP",
        {
            .modes = {Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::Y_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE_INDIRECT, Addr_Modes::ZERO_PAGE_INDIRECT_Y_INDEXED,},
            .bytecodes = {0xCF, 0xDF, 0xDB, 0xC7, 0xD7, 0xC3, 0xD3,},
            .is_undocumented = {true, true, true, true, true, true, true,},
        }
    },
    {
        "ISC",
        {
            .modes = {Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::Y_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE_INDIRECT, Addr_Modes::ZERO_PAGE_INDIRECT_Y_INDEXED,},
            .bytecodes = {0xEF, 0xFF, 0xFB, 0xE7, 0xF7, 0xE3, 0xF3,},
            .is_undocumented = {true, true, true, true, true, true,},
        }
    },
    {
        "RLA",
        {
            .modes = {Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::Y_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE_INDIRECT, Addr_Modes::ZERO_PAGE_INDIRECT_Y_INDEXED,},
            .bytecodes = {0x2F, 0x3F, 0x3B, 0x27, 0x37, 0x23, 0x33,},
            .is_undocumented = {true, true, true, true, true, true,},
        }
    },
    {
        "RRA",
        {
            .modes = {Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::Y_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE_INDIRECT, Addr_Modes::ZERO_PAGE_INDIRECT_Y_INDEXED,},
            .bytecodes = {0x6F, 0x7F, 0x7B, 0x67, 0x77, 0x63, 0x73,},
            .is_undocumented = {true, true, true, true, true, true,},
        }
    },
    {
        "SBC",
        {
            .modes = {Addr_Modes::IMMEDIATE, Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::Y_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE ,Addr_Modes::X_INDEXED_ZERO_PAGE_INDIRECT, Addr_Modes::ZERO_PAGE_INDIRECT_Y_INDEXED,},
            .bytecodes = {0xE9, 0xED, 0xFD, 0xF9, 0xE5, 0xF5, 0xE1, 0xF1,},
            .is_undocumented = {false, false, false, false, false, false, false, false,},
        }
    },
    {
        "SBX",
        {
            .modes = {Addr_Modes::IMMEDIATE,},
            .bytecodes = {0xCB,},
            .is_undocumented = {true,},
        }
    },
    {
        "SLO",
        {
            .modes = {Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::Y_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE_INDIRECT, Addr_Modes::ZERO_PAGE_INDIRECT_Y_INDEXED,},
            .bytecodes = {0x0F, 0x1F, 0x1B, 0x07, 0x17, 0x03, 0x13,},
            .is_undocumented = {true, true, true, true, true, true, true,},
        }
    },
    {
        "SRE",
        {
            .modes = {Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::Y_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE_INDIRECT, Addr_Modes::ZERO_PAGE_INDIRECT_Y_INDEXED,},
            .bytecodes = {0x4F, 0x5F, 0x5B, 0x47, 0x57, 0x43, 0x53,},
            .is_undocumented = {true, true, true, true, true, true, true,},
        }
    },
    {
        "XXA",
        {
            .modes = {Addr_Modes::IMMEDIATE,},
            .bytecodes = {0x8B,},
            .is_undocumented = {true,},
        }
    },
    {
        "DEC",
        {
            .modes = {Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE,},
            .bytecodes = {0xCE, 0xDE, 0xC6, 0xD6,},
            .is_undocumented = {false, false, false, false,},
        }
    },
    {
        "DEX",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes {0xCA,},
            .is_undocumented = {false,},
        }
    },
    {
        "DEY",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes {0x88,},
            .is_undocumented = {false,},
        }
    },
    {
        "INC",
        {
            .modes = {Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE,},
            .bytecodes = {0xEE, 0xFE, 0xE6, 0xF6,},
            .is_undocumented = {false, false, false, false,},
        }
    },
    {
        "INX",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes {0xE8,},
            .is_undocumented = {false,},
        }
    },
    {
        "INY",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes {0xC8,},
            .is_undocumented = {false,},
        }
    },
    {
        "BRK",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes = {0x00,},
            .is_undocumented = {false,},
        }
    },
    {
        "JMP",
        {
            .modes = {Addr_Modes::ABSOLUTE, Addr_Modes::ABSOLUTE_INDIRECT,},
            .bytecodes = {0x4C, 0x6C,},
            .is_undocumented = {false, false,},
        }
    },
    {
        "JSR",
        {
            .modes = {Addr_Modes::ABSOLUTE,},
            .bytecodes = {0x20,},
            .is_undocumented = {false,},
        }
    },
    {
        "RTI",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes = {0x40,},
            .is_undocumented = {false,},
        }
    },
    {
        "RTS",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes = {0x60,},
            .is_undocumented = {false,},
        }
    },
    {
        "BCC",
        {
            .modes = {Addr_Modes::ABSOLUTE,},
            .bytecodes = {0x90,},
            .is_undocumented = {false,},
        }
    },
    {
        "BCS",
        {
            .modes = {Addr_Modes::ABSOLUTE,},
            .bytecodes = {0xB0,},
            .is_undocumented = {false,},
        }
    },
    {
        "BEQ",
        {
            .modes = {Addr_Modes::ABSOLUTE,},
            .bytecodes = {0xF0,},
            .is_undocumented = {false,},
        }
    },
    {
        "BMI",
        {
            .modes = {Addr_Modes::ABSOLUTE,},
            .bytecodes = {0x30,},
            .is_undocumented = {false,},
        }
    },
    {
        "BNE",
        {
            .modes = {Addr_Modes::ABSOLUTE,},
            .bytecodes = {0xD0,},
            .is_undocumented = {false,},
        }
    },
    {
        "BPL",
        {
            .modes = {Addr_Modes::ABSOLUTE,},
            .bytecodes = {0x10,},
            .is_undocumented = {false,},
        }
    },
    {
        "BVC",
        {
            .modes = {Addr_Modes::ABSOLUTE,},
            .bytecodes = {0x50,},
            .is_undocumented = {false,},
        }
    },
    {
        "BVS",
        {
            .modes = {Addr_Modes::ABSOLUTE,},
            .bytecodes = {0x70,},
            .is_undocumented = {false,},
        }
    },
    {
        "CLC",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes = {0x18,},
            .is_undocumented = {false,},
        }
    },
    {
        "CLD",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes = {0xD8,},
            .is_undocumented = {false,},
        }
    },
    {
        "CLI",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes = {0x58,},
            .is_undocumented = {false,},
        }
    },
    {
        "CLV",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes = {0xB8,},
            .is_undocumented = {false,},
        }
    },
    {
        "SEC",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes = {0x38,},
            .is_undocumented = {false,},
        }
    },
    {
        "SED",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes = {0xF8,},
            .is_undocumented = {false,},
        }
    },
    {
        "SEI",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes = {0x78,},
            .is_undocumented = {false,},
        }
    },
    {
        "JAM",
        {
            .modes = {Addr_Modes::IMPLIED,},
            .bytecodes = {0x02,},
            .is_undocumented = {true,},
        }
    },
    {
        "NOP",
        {
            .modes = {Addr_Modes::IMPLIED, Addr_Modes::IMMEDIATE, Addr_Modes::ABSOLUTE, Addr_Modes::X_INDEXED_ABSOLUTE, Addr_Modes::ZERO_PAGE, Addr_Modes::X_INDEXED_ZERO_PAGE,},
            .bytecodes = {0xEA, 0x80, 0x0C, 0x1C, 0x04, 0x14,},
            .is_undocumented = {true, true, true, true, true, true,},
        }
    }
};

std::set<std::string> mnemonics = {
    "LAS",  // *
    "LAX",  // *
    "LDA",
    "LDX",
    "LDY",
    "SAX",  // *
    "SHA",  // *
    "SHX",  // *
    "SHY",  // *
    "STA",
    "STX",
    "STY",
    "SHS",  // *
    "TAX",
    "TAY",
    "TSX",
    "TXA",
    "TXS",
    "TYA",
    "PHA",
    "PHP",
    "PLA",
    "PLP",
    "ASL",
    "LSR",
    "ROL",
    "ROR",
    "AND",
    "BIT",
    "EOR",
    "ORA",
    "ADC",
    "ANC",  // *
    "ARR",  // *
    "ASR",  // *
    "CMP",
    "CPX",
    "CPY",
    "DCP",  // *
    "ISC",  // *
    "RLA",  // *
    "RRA",  // *
    "SBC",
    "SBX",  // *
    "SLO",  // *
    "SRE",  // *
    "XXA",  // *
    "DEC",
    "DEX",
    "DEY",
    "INC",
    "INX",
    "INY",
    "BRK",
    "JMP",
    "JSR",
    "RTI",
    "RTS",
    "BCC",
    "BCS",
    "BEQ",
    "BMI",
    "BNE",
    "BPL",
    "BVC",
    "BVS",
    "CLC",
    "CLD",
    "CLI",
    "CLV",
    "SEC",
    "SED",
    "SEI",
    "JAM",  // *
    "NOP"
};