#include "menu.h"
#include "../cpu/flags.h"
#include "../include/6502-types.h"
#include "../cpu/mem.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int resolve_cli_input(char *input, cpu *main) {
    uint8_t len = strlen(input);
    if (len == 0 || input[1] != ' ' )
        goto command_parse_failed;
    switch (input[0]) {
        case 'e':
        case 'E': {
            return 127;
        }
        case 'd':
        case 'D': {
            char* command = malloc(sizeof(char) * 8);
            printf("Dump memory contents");
            printf("External file or stdout? (default : stdout): ");
            scanf("%8s", command);
            for (int i = 0; i < strlen(command); i++)
                command[i] = tolower(command[i]);
            
            FILE* output;
            if (strcmp(command, "ext") == 0 || strcmp(command, "external") == 0 || strcmp(command, "e") == 0) {
                char* file_path = malloc(sizeof(char) * 100);
                printf("path to external file (will be created if it doesn't exist): ");
                scanf("%100s", command);
                output = fopen(file_path, "w");
                free(file_path);
                if (output == NULL) {
                    fprintf(stderr, "Failed to open external file. Are you sure you inputted the right path, and that you have write permissions to the folder?\n");
                    break;
                }
            } else if (strcmp(command, "stdout") == 0 || strcmp(command, "term") == 0 || strcmp(command, "std") == 0) {
                output = fopen("/dev/stdout", "w");
                if (output == NULL) {
                    fprintf(stderr, "Failed to open stdout for reading. I think you need a new computer.\n");
                    break;
                }
            }
            ushort num_zeros = 0;
            for (ushort pos = 0; pos <= MAX_MEM_LEN; pos++) {
                if (pos % 16 == 0) {
                    /* Round off the previous line */
                    fprintf(output, "|\n");
                    fprintf(output, "0x%004x -> 0x%004x:: ", pos, (pos + 15 < MAX_MEM_LEN) ? pos + 15 : MAX_MEM_LEN - 1);
                }
                fprintf(output, "%002x ", main->mem[pos]);
                if (main->mem[pos] == 0)
                    num_zeros++;
            }
            fprintf(output, "\n%d bytes (%Lf%%) of the internal memory is filled", MAX_MEM_LEN - num_zeros, ((MAX_MEM_LEN - num_zeros) / (long double)MAX_MEM_LEN) * 100.0);
            free(command);
            break;
        }
        // case 's':
        // case 'S': {
        //     printf("Status: %s", (main->halt) ? "running" : "halted");
        // }
        case 'r':
        case 'R': {
            printf("Printing CPU register contents::\n");

            printf("General purpose registers::\n");
            printf("    A (Accumulator): 0x%02x\n\n", main->A);
            
            printf("Index registers::\n");
            printf("    X (Index X): 0x%02x\n", main->X);
            printf("    Y (Index Y): 0x%02x\n\n", main->Y);

            printf("Memory pointer registers::\n");
            printf("    PC (Program Counter): 0x%02x\n", main->PC);
            printf("    SP (Stack Pointer): 0x%04x\n", main->SP);
            printf("Status Registers::\n");
            printf("    SR (Status Register): 0x%02x\n\n", main->SR);
            
            printf("Status Register Breakdown::\n");
            printf("    Binary representation: 0b"BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(main->SR));
            printf("    Index 0: C (Carry)             : %d | %B\n", (test_flag(main, CARRY))             ? 1 : 0, (test_flag(main, CARRY))             ? true : false);
            printf("    Index 1: Z (Zero)              : %d | %B\n", (test_flag(main, ZERO))              ? 1 : 0, (test_flag(main, ZERO))              ? true : false);
            printf("    Index 2: I (Interrupt Disable) : %d | %B\n", (test_flag(main, INTERRUPT_DISABLE)) ? 1 : 0, (test_flag(main, INTERRUPT_DISABLE)) ? true : false);
            printf("    Index 3: D (Decimal Mode (BCD)): %d | %B\n", (test_flag(main, DECIMAL_MODE))      ? 1 : 0, (test_flag(main, DECIMAL_MODE))      ? true : false);
            printf("    Index 4: B (Break Command)     : %d | %B\n", (test_flag(main, BRK_COMMAND))       ? 1 : 0, (test_flag(main, BRK_COMMAND))       ? true : false);
            printf("    Index 5: _ (Not Used)          : 0  | 0\n");
            printf("    Index 6: V (Overflow)          : %d | %B\n", (test_flag(main, OVERFLOW))          ? 1 : 0, (test_flag(main, OVERFLOW))          ? true : false);
            printf("    Index 7: N (Negative)          : %d | %B\n", (test_flag(main, NEGATIVE))          ? 1 : 0, (test_flag(main, NEGATIVE))          ? true : false);
        
        }
        case 'm':
        case 'M': {
            if (len == 1) {
                ushort start = main->PC - (main->PC % 10);
                ushort end = start + 10;
                uint8_t pos = 0;
                printf("0x%04x:: ", start);
                
                for (ushort i = start; i <= end; i++) {
                    printf("%02x ", main->mem[i]);
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
            
            char* command = malloc(1);
            if (sscanf(input, "%c %hd-%hd", command, &start, &end) != 3)
                goto command_parse_failed;
            /* Discard first char because I can */
            free(command);
            
            int num_digits = count_digits(end);
            printf("Printing memory contents from 0x%04x (decimal: %*d) to 0x%04x (decimal: %*d):\n", 
                   start, num_digits, start, end, num_digits, end);

            // Process the memory contents from start to end
            for (int i = start - (start % 16); i < end + (16 - (end % 16)) % 16; i++) {
                if (i % 16 == 0) {
                    if (i != start) {
                        printf("|\n");  // End the previous line
                    }
                    printf("0x%04x (decimal: %*d) -> 0x%04x (decimal: %*d):: ", 
                           i, num_digits, i, (i > end) ? end : i + 15, num_digits, (i > end) ? end : i + 15);
                }
                if (i < start)
                    printf("-- ");  // Print the memory contents
                else if (i > end)
                    printf("-- ");  // Print the memory contents
                else
                    printf("%02x ", (unsigned char)main->mem[i]);  // Print the memory contents
            }

    printf("|\n");  // Final newline after loop completes
        }
    }
    return 0;
command_parse_failed:
    return COMMAND_PARSE_FAILED;
}

bool get_yes_no_response(char *prompt, bool _default) {
    char input;
    while (true) {
        printf("%s : (default : %s): ", prompt, (_default == true) ? "yes" : "no");
        scanf("%c", &input);
        input = tolower(input);
        char tests[] = {'y', 'n', '\n'};
        if (test_char(input, tests) == false) {
            printf("That's not a valid input, kiddo\n");
            continue;
        }
        if (strcmp(&input, "\n") == 0)
            return _default;
        if (strcmp(&input, "n") == 0)
            return false;
        return true;            
    }
}