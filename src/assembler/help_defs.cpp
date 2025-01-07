#pragma once

#include <clocale>
#include <cwchar>
#include <cstdio>
#include "include.cpp"

typedef enum {
    IMPLIED,                           // N/A
    ACCUMULATOR,                       // A
    IMMEDIATE,                         // #$nn
    ABSOLUTE,                          // $nnnn
    X_INDEXED_ABSOLUTE,                // $nnnn,X
    Y_INDEXED_ABSOLUTE,                // $nnnn,Y
    ABSOLUTE_INDIRECT,                 // ($nnnn)
    ZERO_PAGE,                         // $nn
    X_INDEXED_ZERO_PAGE,               // $nn,X
    Y_INDEXED_ZERO_PAGE,               // $nn,Y
    X_INDEXED_ZERO_PAGE_INDIRECT,      // ($nn,X)
    ZERO_PAGE_INDIRECT_Y_INDEXED,      // (&nn),Y
    RELATIVE,                          // $nnnn
    NOTHING,                           // N/A
} AddressingModes;

typedef struct {
    AddressingModes addr_mode;
    byte_raw bytecode;
    byte_raw num_bytes;
    byte_raw num_cycles[6];
    bool undocumented_instruction;
} addr_mode_info;

typedef enum {
    AFFECTED,
    NOT_AFFECTED,
    RESET,
    INITIALIZED
} flag;

typedef struct {
    char mnemonic[4];
    char operation_summary[256];
    /* i.e. operation_string = "LDA [$HHLL]" = mem[HHLL] -> A */
    char operation_string[100];
    char category[7];
    flag affected_flags[8];
    char help_message[2048];
    byte_raw addr_mode_info_len;
    addr_mode_info AddressingModes[30];
} help_message;

// Copied and pasted from a similar C file
static inline void print_instruction_help(help_message instruction) {
    for (int i = 0; i < 3; i++)
        instruction.mnemonic[i] = toupper(instruction.mnemonic[i]);
    printf("Mnemonic: %s\n", instruction.mnemonic);
    printf("Summary: %s\n", instruction.operation_summary);
    printf("Category: %s\n", instruction.category);
    printf("Operation: %s\n", instruction.operation_string);
    printf("Affected flags:\n");
    printf("| C | Z | I | D | B | _ | V | N |\n");
    printf("|");
    setlocale(LC_ALL, "");
    for (int i = 0; i < 8; i++)
        wprintf(L" %lc |", 
            (instruction.affected_flags[i] == AFFECTED) ? "✓" :
            (instruction.affected_flags[i] == NOT_AFFECTED) ? "-" :
            (instruction.affected_flags[i] == RESET) ? "0" : "1");
    printf("\n\n");
    printf("______________________________________________________________________\n\n");
    printf("%s\n\n", instruction.help_message);
    printf("Expected addressing modes: (%hhd modes)", instruction.addr_mode_info_len);
    for (int i = 0; i < instruction.addr_mode_info_len; i++) {
        switch (instruction.AddressingModes[i].addr_mode) {
            case IMPLIED:
                printf("IMPLIED (0x%02x)%c                      | %s\n", instruction.AddressingModes[i].bytecode, (instruction.AddressingModes[i].undocumented_instruction == true) ? '*' : ' ', instruction.mnemonic);
                break;
            case ACCUMULATOR:
                printf("ACCUMULATOR (0x%02x)%c                  | %s A\n", instruction.AddressingModes[i].bytecode, (instruction.AddressingModes[i].undocumented_instruction == true) ? '*' : ' ', instruction.mnemonic);
                break;
            case IMMEDIATE:
                printf("IMMEDIATE (0x%02x)%c                    | %s #$HHLL\n", instruction.AddressingModes[i].bytecode, (instruction.AddressingModes[i].undocumented_instruction == true) ? '*' : ' ', instruction.mnemonic);
                break;
            case ABSOLUTE:
                printf("ABSOLUTE (0x%02x)%c                     | %s $HHLL\n", instruction.AddressingModes[i].bytecode, (instruction.AddressingModes[i].undocumented_instruction == true) ? '*' : ' ', instruction.mnemonic);
                break;
            case X_INDEXED_ABSOLUTE:
                printf("X INDEXED ABSOLUTE (0x%02x)%c           | %s $HHLL,X\n", instruction.AddressingModes[i].bytecode, (instruction.AddressingModes[i].undocumented_instruction == true) ? '*' : ' ', instruction.mnemonic);
                break;
            case Y_INDEXED_ABSOLUTE:
                printf("Y INDEXED ABSOLUTE (0x%02x)%c           | %s $HHLL,Y\n", instruction.AddressingModes[i].bytecode, (instruction.AddressingModes[i].undocumented_instruction == true) ? '*' : ' ', instruction.mnemonic);
                break;
            case ABSOLUTE_INDIRECT:
                printf("ABSOLUTE INDIRECT (0x%02x)%c            | %s ($HHLL)\n", instruction.AddressingModes[i].bytecode, (instruction.AddressingModes[i].undocumented_instruction == true) ? '*' : ' ', instruction.mnemonic);
                break;
            case ZERO_PAGE:
                printf("ZERO PAGE (0x%02x)%c                    | %s $HH\n", instruction.AddressingModes[i].bytecode, (instruction.AddressingModes[i].undocumented_instruction == true) ? '*' : ' ', instruction.mnemonic);
                break;
            case X_INDEXED_ZERO_PAGE:
                printf("X INDEXED ZERO PAGE (0x%02x)%c          | %s $HH,X\n", instruction.AddressingModes[i].bytecode, (instruction.AddressingModes[i].undocumented_instruction == true) ? '*' : ' ', instruction.mnemonic);
                break;
            case Y_INDEXED_ZERO_PAGE:
                printf("Y INDEXED ZERO PAGE (0x%02x)%c          | %s $HH,X\n", instruction.AddressingModes[i].bytecode, (instruction.AddressingModes[i].undocumented_instruction == true) ? '*' : ' ', instruction.mnemonic);
                break;
            case X_INDEXED_ZERO_PAGE_INDIRECT:
                printf("X INDEXED ZERO PAGE INDIRECT (0x%02x)%c | %s ($HH,X)\n", instruction.AddressingModes[i].bytecode, (instruction.AddressingModes[i].undocumented_instruction == true) ? '*' : ' ', instruction.mnemonic);
                break;
            case ZERO_PAGE_INDIRECT_Y_INDEXED:
                printf("ZERO PAGE INDIRECT Y INDEXED (0x%02x)%c | %s ($HH),Y\n", instruction.AddressingModes[i].bytecode, (instruction.AddressingModes[i].undocumented_instruction == true) ? '*' : ' ', instruction.mnemonic);
                break;
            case RELATIVE:
                printf("RELATIVE (0x%02x)%c                     | %s $HHLL\n", instruction.AddressingModes[i].bytecode, (instruction.AddressingModes[i].undocumented_instruction == true) ? '*' : ' ', instruction.mnemonic);
                break;
            case NOTHING:
                printf("NOTHING (0x%02x)%c                      | %s N/A\n", instruction.AddressingModes[i].bytecode, (instruction.AddressingModes[i].undocumented_instruction == true) ? '*' : ' ', instruction.mnemonic);
                break;
        }
        printf("\n");
    }
}