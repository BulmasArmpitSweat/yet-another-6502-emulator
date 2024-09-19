#include "menu.h"
#include "../cpu/flags.h"
#include "../include/6502-types.h"
#include "../cpu/mem.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

#include "instruction_help.h"
#include "instruction_info.c"

// TODO: Implement regex checking with this pattern: ^(0x[0-9A-Fa-f]+|[0-9]+)$

__attribute__((optimize("Ofast")))
char* replace_string(const char* str, const char* replace_this, const char* with_that) {
    // Find the number of occurrences of 'replace_this' in 'str'
    int count = 0;
    const char* tmp = str;
    size_t replace_len = strlen(replace_this);
    size_t with_len = strlen(with_that);

    while ((tmp = strstr(tmp, replace_this))) {
        count++;
        tmp += replace_len;
    }

    // Allocate memory for the new result
    size_t result_len = strlen(str) + count * (with_len - replace_len);
    char* result = (char*)malloc(result_len + 1);  // +1 for null terminator
    if (!result) return NULL; // Handle memory allocation failure

    char* current = result;
    while (*str) {
        const char* p = strstr(str, replace_this);
        if (p) {
            // Copy part of the string before 'replace_this'
            size_t len = p - str;
            memcpy(current, str, len);
            current += len;

            // Copy 'with_that' in place of 'replace_this'
            memcpy(current, with_that, with_len);
            current += with_len;

            // Skip the 'replace_this' part in the original string
            str = p + replace_len;
        } else {
            // No more occurrences, copy the rest of the string
            strcpy(current, str);
            break;
        }
    }

    return result;
}

__attribute__((optimize("Ofast")))
char** split_string(char* str, char delimiter, int* num_tokens) {
    char* token;
    char* str_cpy = strdup(str);
    int size = 10;
    char** result = malloc(size* sizeof(char*));
    int index = 0;

    if (result == NULL || str_cpy == NULL)
	return NULL;

    token = strtok(str_cpy, &delimiter); // Get initial token

    while (token != NULL) {
	if (index >= size) {
	    size *= 2;
	    char** temp = realloc(result, size * sizeof(char*));
	    if (temp == NULL) {
		free(str_cpy);
		for (int i = 0; i < index; i++)
		    free(result[i]);
		free(result);
		return NULL;
	    }
	    result = temp;
	}

	result[index] = strdup(token);
	if (result[index] == NULL) {
	    free(str_cpy);
	    for (int i = 0; i < index; i++)
		free(result[i]);
	    free(result);
	    return NULL;
	}
	index++;
	token = strtok(NULL, &delimiter);
    }

    char** temp = realloc(result, index * sizeof(char*));
    if (temp == NULL) {
	free(str_cpy);
	for (int i = 0; i < index; i++)
	    free(result[i]);
	free(result);
	return NULL;
    }
    result = temp;

    *num_tokens = index;
    free(str_cpy);
    return result;
}
    
__attribute__((optimize("Ofast")))
int resolve_cli_input(char *input, cpu *main) {
    /* Calculate length of input string */
    uint8_t len = strlen(input);
    
    /* Set all characters of the input to lowercase for easier handling */
    tolower_char(input);

    /* Split string into 'tokens', using space character as a delimiter */
    int num_tokens = 0;
    char** input_tokens = split_string(input, ' ', &num_tokens);

    /* Exit with fail state if input is empty */
    if (len == 0)
        goto command_parse_failed;

    if (
        /* Exit program CLI */
        strcmp(input_tokens[0], "exit") == 0 ||
        strcmp(input_tokens[0], "exi")  == 0 ||
        strcmp(input_tokens[0], "ex")   == 0 ||
        strcmp(input_tokens[0], "e")    == 0
    ) {
        return EXIT_SUCCESS;
    } else if (
        /* Dump memory binary contents to either external file */
        strcmp(input_tokens[0], "dump") == 0 ||
        strcmp(input_tokens[0], "dum")  == 0 ||
        strcmp(input_tokens[0], "du")   == 0 ||
        strcmp(input_tokens[0], "d")    == 0
    ) {
        FILE* output;

        /* Get output destination selection from user */
        char* file_path = malloc(sizeof(char) * 100);
        printf("path to external file (will be created if it doesn't exist): ");
        scanf("%100s", file_path);
        output = fopen(file_path, "w");
        free(file_path);
        if (output == NULL) {
            fprintf(stderr, "Failed to open external file. Are you sure you inputted the right path, and that you have write permissions to the folder?\n");
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
        free(file_path);
    } else if (
        strcmp(input_tokens[0], "registers") == 0 ||
        strcmp(input_tokens[0], "register")  == 0 ||
        strcmp(input_tokens[0], "registe")   == 0 ||
        strcmp(input_tokens[0], "regist")    == 0 ||
        strcmp(input_tokens[0], "regis")     == 0 ||
        strcmp(input_tokens[0], "regi")      == 0 ||
        strcmp(input_tokens[0], "reg")       == 0 ||
        strcmp(input_tokens[0], "re")        == 0 ||
        strcmp(input_tokens[0], "r")         == 0
    ) {
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
        printf("    Index 0: C (Carry)             : %d | %s\n", (test_flag(main, CARRY))             ? 1 : 0, (test_flag(main, CARRY))             ? "true" : "false");
        printf("    Index 1: Z (Zero)              : %d | %s\n", (test_flag(main, ZERO))              ? 1 : 0, (test_flag(main, ZERO))              ? "true" : "false");
        printf("    Index 2: I (Interrupt Disable) : %d | %s\n", (test_flag(main, INTERRUPT_DISABLE)) ? 1 : 0, (test_flag(main, INTERRUPT_DISABLE)) ? "true" : "false");
        printf("    Index 3: D (Decimal Mode (BCD)): %d | %s\n", (test_flag(main, DECIMAL_MODE))      ? 1 : 0, (test_flag(main, DECIMAL_MODE))      ? "true" : "false");
        printf("    Index 4: B (Break Command)     : %d | %s\n", (test_flag(main, BRK_COMMAND))       ? 1 : 0, (test_flag(main, BRK_COMMAND))       ? "true" : "false");
        printf("    Index 5: _ (Not Used)          : _  | _\n");
        printf("    Index 6: V (Overflow)          : %d | %s\n", (test_flag(main, OVERFLOW))          ? 1 : 0, (test_flag(main, OVERFLOW))          ? "true" : "false");
        printf("    Index 7: N (Negative)          : %d | %s\n", (test_flag(main, NEGATIVE))          ? 1 : 0, (test_flag(main, NEGATIVE))          ? "true" : "false");
    } else if (
        strcmp(input_tokens[0], "memory") == 0 ||
        strcmp(input_tokens[0], "memor")  == 0 ||
        strcmp(input_tokens[0], "memo")   == 0 ||
        strcmp(input_tokens[0], "mem")    == 0 ||
        strcmp(input_tokens[0], "me")     == 0 ||
        strcmp(input_tokens[0], "m")      == 0
    ) {
        if (num_tokens == 1) {
            ushort start = main->PC - (main->PC % 16);
            /* Get the 16-byte block of memory that the Program Counter is in. */
            ushort end = start + 16;
            u_byte pos = 0;
            printf("0x%04x:: ", start);

            for (ushort i = start; i <= end; i++) {
                printf("%02x ", main->mem[i]);
                if (i == main->PC)
                    pos = (i % 16) * 3;
            }
            for (byte_raw i = 0; i <= pos; i++)
                printf(" ");
            printf("^PC");
            return 0;
        }

        ushort start, end;
        char *start_char, *end_char;
        regex_t regex;
        regmatch_t capture_groups[3];
        const char* regex_pattern = "^\\s*(0x[0-9a-fA-F]+|0b[01]+|\\d+)?\\s*-\\s*(0x[0-9a-fA-F]+|0b[01]+|\\d+)?\\s*$";
        int reti = regcomp(&regex, regex_pattern, REG_EXTENDED);
        if (reti)
            FATAL_ERROR(ERR_REGEX_COMPILATION_FAILURE);
        
        if (regexec(&regex, input_tokens[1], 3, capture_groups, 0) == 0) {
            if (capture_groups[1].rm_so != -1) {
                int len = capture_groups[1].rm_eo - capture_groups[1].rm_so;
                snprintf(start_char, len + 1, "%.*s", len, input_tokens[1] + capture_groups[1].rm_so);
            } else {
                start_char = "0x0000";
            }

            if (capture_groups[2].rm_so != -1) {
                int len = capture_groups[2].rm_eo - capture_groups[2].rm_so;
                snprintf(end_char, len + 1, "%.*s", len, input_tokens[2] + capture_groups[2].rm_so);
            } else {
                end_char = "0xFFFF";
            }
        } else {
            fprintf(stderr, "Input does not match format. Hint: run 'help memory'\n");
            goto command_parse_failed;
        }

        const char start_header[2] = {start_char[0], start_char[1]}, end_header[2] = {end_char[0], end_char[1]};
        char start_body[strlen(start_char) - 1], end_body[strlen(end_char - 1)];
        strcpy(start_body, start_char + 2);
        strcpy(end_body, end_char + 2);

        if (strcmp(start_header, "0x") == 0) {
            start = strtol(start_body, NULL, 16);
        } else if (strcmp(start_header, "0b") == 0) {
            start = strtol(start_body, NULL, 2);
        } else {
            start = atoi(start_body);
        }

        if (strcmp(end_header, "0x") == 0) {
            end = strtol(end_body, NULL, 16);
        } else if (strcmp(end_header, "0b") == 0) {
            end = strtol(end_body, NULL, 2);
        } else {
            end = atoi(end_body);
        }

        printf("Printing memory contents from 0x%04x (decimal: %*d) to 0x%04x (decimal: %*d):\n", 
               start, count_digits(start), start, end, count_digits(end), end);

        // Process the memory contents from start to end
        for (int i = start - (start % 16); i < end + (16 - (end % 16)) % 16; i++) {
            if (i % 16 == 0) {
                if (i != start) {
                    printf("|\n");  // End the previous line
                }
                printf("0x%04x (decimal: %*d) -> 0x%04x (decimal: %*d):: ", 
                       i, count_digits(start), i, (i > end) ? end : i + 15, count_digits(end), (i > end) ? end : i + 15);
            }
            if (i < start)
                printf("-- ");  // Print the memory contents
            else if (i > end)
                printf("-- ");  // Print the memory contents
            else
                printf("%02x ", (unsigned char)main->mem[i]);  // Print the memory contents
        }
            printf("|\n");  // Final newline after loop completes
    } else if (
        strcmp(input_tokens[0], "help") == 0 ||
        strcmp(input_tokens[0], "hel")  == 0 ||
        strcmp(input_tokens[0], "he")   == 0 ||
        strcmp(input_tokens[0], "h")    == 0
    ) {
        if (num_tokens == 1) {
            printf("Help Interface:\n");
            printf("_______________\n\n");

            printf("Use this command to get help explaining many different parts of the overall emulator.\n");
            printf("Some of the commands you can use are:\n");
            printf("    cli | Get help on the commands you can run in this interface\n");
            printf("    cpu | Get help on different parts of the CPU.");
        } else {
            if (strcmp(input_tokens[1], "cli") == 0) {
                printf("CLI commands:\n");
                printf("_____________\n\n");

                printf("    registers | Print out contents of CPU registers and status flags\n");
                printf("    memory [0x]   | Dump mem");
                printf("    dump      | Dump formatted hex data for file into external file (can be stdout if requested [/dev/stdout])\n");
                printf("    help      | Show this message\n");
                printf("    exit      | Exit the program\n");
            } else if (strcmp(input_tokens[1], "cpu") == 0) {
                if (num_tokens == 2) {
                    printf("CPU help interface:\n");
                    printf("___________________\n\n");

                    printf("instruction [instruction]    | Print information about a specific CPU instruction\n");
                    printf("addressing_modes [addr_mode] | Print information about the various addressing modes the CPU uses\n");
                    printf("registers [register]         | Print information about the various registers in the CPU\n\n");
                }
                if (strcmp(input_tokens[2], "instruction") == 0) {
                    bool found_instruction = false;
                    for (int i = 0; i < sizeof(instructions) / sizeof(help_message); i++) {
                        if (strcmp(instructions[i].mnemonic, input_tokens[3]) == 0) {
                            print_instruction_help(instructions[i]);
                            found_instruction = true;
                        }
                    }
                    if (found_instruction == false) {
                        fprintf(stderr, "Invalid instruction. \"%s\" is not a valid instruction. (hint: run \"help cpu\"\n", input_tokens[3]);
                    }
                } else if (
                    strcmp(input_tokens[2], "addressing_modes") == 0 ||
                    strcmp(input_tokens[2], "addressing_mode")  == 0 ||
                    strcmp(input_tokens[2], "addr_modes")       == 0 ||
                    strcmp(input_tokens[2], "addr_mode")        == 0
                ) {
                    if (num_tokens == 4) {
                        toupper_char(input_tokens[3]);
                        if (strcmp(input_tokens[3], "ACCUMULATOR") == 0) {
                            printf("ACCUMULATOR (XXX A) | Instruction takes the accumulator as input. As a result, the input isn't hardcoded into the bytecode.\n");
                        } else if (strcmp(input_tokens[3], "IMPLIED") == 0) {
                            printf("IMPLIED (XXX) | Instruction takes no arguments. Instruction's functionality is \"implied\".\n");
                        } else if (strcmp(input_tokens[3], "IMMEDIATE")) {
                            printf("IMMEDIATE (XXX #$nn) | Instruction parameter is directly inputted in the bytecode. No other input is provided.\n");
                        } else if (strcmp(input_tokens[3], "ABSOLUTE") == 0) {
                            printf("ABSOLUTE (XXX $nnnn) | Instruction parameter is at the memory location pointed at by the two-byte operand.\n");
                        } else if (strcmp(input_tokens[3], "X_INDEXED_ABSOLUTE")) {
                            printf("X_INDEXED_ABSOLUTE (XXX $nnnn,X) | Instruction parameter is located at the memory location pointed at by the two-byte operand, plus the value in the index register X.\n");
                        } else if (strcmp(input_tokens[3], "Y_INDEXED_ABSOLUTE")) {
                            printf("Y_INDEXED_ABSOLUTE (XXX $nnnn,Y) | Instruction parameter is located at the memory location pointed at by the two-byte operand, plus the value in the index register Y.\n");
                        } else if (strcmp(input_tokens[3], "ABSOLUTE_INDIRECT")) {
                            printf("ABSOLUTE_INDIRECT (XXX ($nnnn)) | Instruction parameter is at the memory location at the memory location pointed at by the included two-byte operand.\n");
                        } else if (strcmp(input_tokens[3], "ZERO_PAGE")) {
                            printf("ZERO_PAGE | Instruction parameter is located in the first page of memory. Address is directly given as an operand. Like Absolute, but faster.\n");
                        } else if (strcmp(input_tokens[3], "X_INDEXED_ZERO_PAGE")) {
                            printf("X_INDEXED_ZERO_PAGE | Instruction parameter is located in the first page of memory, and is the address directly given as an operand, plus the value in the index register X. Like X_INDEXED_ABSOLUTE, but faster.\n");
                        } else if (strcmp(input_tokens[3], "Y_INDEXED_ZERO_PAGE")) {
                            printf("Y_INDEXED_ZERO_PAGE | Instruction parameter is located in the first page of memory, and is the address directly given as an operand, plus the value in the index register Y. Like Y_INDEXED_ABSOLUTE, but faster.\n");
                        } else if (strcmp(input_tokens[3], "X_INDEXED_ZERO_PAGE_INDIRECT")) {
                            printf("X_INDEXED_ZERO_PAGE_INDIRECT | Instruction parameter is located at the memory location plus the value in the index register X, pointed at by the value in memory pointed at by the two-byte operand.\n");
                        } else {
                            printf("ZERO_PAGE_INDIRECT_Y_INDEXED | Instruction parameter is located at the memory location pointed at by the value in memory pointed at by the two-byte operand, plus the value in the Index register Y.\n");
                        }
                    }
                } else if (
                    strcmp(input_tokens[2], "registers") == 0 ||
                    strcmp(input_tokens[2], "register")  == 0 ||
                    strcmp(input_tokens[2], "regs")      == 0 ||
                    strcmp(input_tokens[2], "reg")       == 0
                ) {
                    if (num_tokens == 3) {
                        printf("Registers:\n");
                        printf("__________\n\n");

                        printf("A (Accumulator) (8 Bits) | The one and only general purpose register in the entire CPU. A lot of instructions (Like CMP, SBC, PHA, etc.) have an implied second input, which is usually the Accumulator.\n");
                        printf("X, Y (Index registers X and Y) (8 Bits) | Often used as offsets for some addressing modes. They CAN be used as general purpose registers, but their functionality is limited, and all complex arithmetic will need to have the moved to the Accumulator before it can be worked on.\n");
                        printf("PC (Program Counter) (16 Bits) | Contains the value that the CPU is currently accessing. Can be an instruction, or a value, depending on the context The only 16 Bit register in the entire CPU, giving it a max addressable memory space of 65535 Bytes (64Kb).\n");
                        printf("SP (Stack Pointer) (8 Bits) | Points to the current highest loaded value in the stack. The stack makes up the bottom 256 bytes of memory.\n");
                        printf("SR (Status register) (8 Bits) | The bits of this register contains the current status of the CPU, which are always up to date up to the most recent instruction, which may or may not change them.\n\n");
                        printf("Status register breakdown:\n");
                        printf("Bit position | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |\n");
                        printf("CPU flags    | C | Z | I | D | B | - | V | N |\n\n");

                        printf("0: C (Carry)             |");
                        printf("1: Z (Zero)              |");
                        printf("2: I (Interrupt Disable) |");
                        printf("3: D (Decimal Mode (BCD))|");
                        printf("4: B (Break Command)     |");
                        printf("5: - (Not Used)          |");
                        printf("6: V (Overflow)          |");
                        printf("7: N (Negative)          |");
                    } else {
                        if (strcmp(input_tokens[2], "a") == 0 || strcmp(input_tokens[2], "accumulator") == 0) {
                            printf("A (Accumulator) (8 Bits) | The one and only general purpose register in the entire CPU. A lot of instructions (Like CMP, SBC, PHA, etc.) have an implied second input, which is usually the Accumulator.\n");
                        } else if (strcmp(input_tokens[2], "x") || strcmp(input_tokens[2], "y")) {
                            printf("X, Y (Index registers X and Y) (8 Bits) | Often used as offsets for some addressing modes. They CAN be used as general purpose registers, but their functionality is limited, and all complex arithmetic will need to have the moved to the Accumulator before it can be worked on.\n");
                        } else if (strcmp(input_tokens[2], "pc") == 0) {
                            printf("PC (Program Counter) (16 Bits) | Contains the value that the CPU is currently accessing. Can be an instruction, or a value, depending on the context The only 16 Bit register in the entire CPU, giving it a max addressable memory space of 65535 Bytes (64Kb).\n");
                        } else if (strcmp(input_tokens[2], "sr") == 0) {
                            printf("SR (Status register) (8 Bits) | The bits of this register contains the current status of the CPU, which are always up to date up to the most recent instruction, which may or may not change them.\n\n");
                            printf("Status register breakdown:\n");
                            printf("Bit position | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |\n");
                            printf("CPU flags    | C | Z | I | D | B | - | V | N |\n\n");

                            printf("0: C (Carry)             |");
                            printf("1: Z (Zero)              |");
                            printf("2: I (Interrupt Disable) |");
                            printf("3: D (Decimal Mode (BCD))|");
                            printf("4: B (Break Command)     |");
                            printf("5: - (Not Used)          |");
                            printf("6: V (Overflow)          |");
                            printf("7: N (Negative)          |");
                        }
                    }
                }
            } else if (strcmp(input_tokens[1], "memory") == 0) {
                printf("Memory interface:\n");
                printf("_________________\n\n");

                printf("Usage: memory ([0x|0b|])?value-([0x|0b|])?value\n");
                printf("The start and end values are separated by a \"-\".\n");
                printf("Either values can be either decimal, hexadecimal, or binary.\n");
                printf("If it is hexadecimal, it must be prefixed with a \"0x\". Similarly,\n");
                printf("Binary numbers must be prefixed with a \"0b\". Decimal doesn't need this.\n");
            }
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