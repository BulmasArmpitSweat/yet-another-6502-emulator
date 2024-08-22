#include "menu.h"
#include "../cpu/flags.h"
#include "../include/6502-types.h"
#include <stdint.h>
#include <stdio.h>

int resolve_cli_input(char *input, cpu *main) {
    uint8_t len = strlen(input);
    if (len == 0)
        goto command_parse_failed;
    switch (input[0]) {
        // case 's':
        // case 'S': {
        //     printf("Status: %s", (main->halt) ? "running" : "halted");
        // }
        case 'r':
        case 'R': {
            printf("Printing CPU register contents::\n");

            printf("General purpose registers::\n");
            printf("    A (Accumulator): 0x%2x\n\n", main->A);
            
            printf("Index registers::\n");
            printf("    X (Index X): 0x%2x\n", main->X);
            printf("    Y (Index Y): 0x%2x\n\n", main->Y);

            printf("Memory pointer registers::\n");
            printf("    PC (Program Counter): 0x%2x\n", main->PC);
            printf("    SP (Stack Pointer): 0x%2x\n", main->SP);
            printf("    SR (Status Register): 0x%2x\n\n", main->SR);
            
            printf("Status Register Breakdown::\n");
            printf("    Binary representation: 0b"BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(main->SR));
            printf("    Index 0: C (Carry)             : %d | %B", (test_flag(main, CARRY))             ? 1 : 0, (test_flag(main, CARRY))             ? true : false);
            printf("    Index 1: Z (Zero)              : %d | %B", (test_flag(main, ZERO))              ? 1 : 0, (test_flag(main, ZERO))              ? true : false);
            printf("    Index 2: I (Interrupt Disable) : %d | %B", (test_flag(main, INTERRUPT_DISABLE)) ? 1 : 0, (test_flag(main, INTERRUPT_DISABLE)) ? true : false);
            printf("    Index 3: D (Decimal Mode (BCD)): %d | %B", (test_flag(main, DECIMAL_MODE))      ? 1 : 0, (test_flag(main, DECIMAL_MODE))      ? true : false);
            printf("    Index 4: B (Break Command)     : %d | %B", (test_flag(main, BRK_COMMAND))       ? 1 : 0, (test_flag(main, BRK_COMMAND))       ? true : false);
            printf("    Index 5: _ (Not Used)          : 0  | 0");
            printf("    Index 6: V (Overflow)          : %d | %B", (test_flag(main, OVERFLOW))          ? 1 : 0, (test_flag(main, OVERFLOW))          ? true : false);
            printf("    Index 7: N (Negative)          : %d | %B", (test_flag(main, NEGATIVE))          ? 1 : 0, (test_flag(main, NEGATIVE))          ? true : false);
        }
        case 'm':
        case 'M': {
            if (len == 1) {
                ushort start = main->PC - (main->PC % 10);
                ushort end = start + 10;
                uint8_t pos = 0;
                printf("0x%4x:: ", start);
                
                for (ushort i = start; i <= end; i++) {
                    printf("%2x ", main->mem[i]);
                    if (i == main->PC)
                        pos = (i % 10) * 3;
                }
                for (int i = 0; i <= pos; i++)
                    printf(" ");
                printf("^PC");
            }
            if (input[1] != ' ')
                goto command_parse_failed;

            ushort start, end;
            
            if (sscanf(input, "m %hd-%hd", &start, &end) != 2)
                goto command_parse_failed;
            
            printf("Printing memory contents from 0x%2x to 0x%2x:\n   ", main->mem[start], main->mem[end]);
            
            for (ushort i = start, printed = 0; i <= end; i++, printed++) {
                if (printed == 0)
                    printf("0x%4x:: ", main->mem[i]);
            
                printf("%2x ", main->mem[i]);
            
                if (printed % 10 == 0 && printed != 0) {
                    printf("|\n");
                    printf("0x%4x:: ", main->mem[i]);
                }
            }
        }
    }
    return 0;
command_parse_failed:
    return COMMAND_PARSE_FAILED;
}