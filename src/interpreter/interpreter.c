#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "interpreter.h"
#include "../cpu/mem.h"

void compile_file(FILE* in_file, FILE* out_file, instruction_token token_list) {
	
}

instruction_token parse_line(char* line, int line_num) {
    instruction_token result = {0};  // Initialize result
    instruction_token *ptr = &result;
    byte_raw line_ptr = 0;
    char last_char = ' ';

    // Parse the mnemonic (first 3 characters)
    sscanf(line, "%3c ", (char*)ptr->data);
    ptr->type = MNEMONIC;
    ptr->next = NULL;
    line_ptr = 3;

    while (line[line_ptr] != '\0' && line[line_ptr] != '\n') {
        last_char = line[line_ptr];
        switch (line[line_ptr]) {
            case 'A': {
                if (last_char != ' ') {
                    COMPILER_ERROR("Unexpected Accumulator definition at line %d, position %d.", line_num, line_num, line_ptr);
                } else {
                    ptr->next = malloc(sizeof(instruction_token));
                    ptr = ptr->next;
                    ptr->type = ACCUMULATOR_DEFINITION;
                    ptr->data = NULL;
                    ptr->next = NULL;
                    return result;
                }
                break;
            }
            case '$': {
                // Ensure $ is preceded by space, #, or (
                if (last_char != ' ' && last_char != '#' && last_char != '(') {
                    COMPILER_ERROR("Unexpected Value definition at line %d, position %d.", line_num, line_num, line_ptr);
                } else {
                    ptr->next = malloc(sizeof(instruction_token));
                    ptr = ptr->next;
                    char value[5] = {0};
                    int i = 0;

                    // Parse the value, allow up to 4 hex digits
                    for (i = 0; i < 4 && isxdigit(line[line_ptr + i]); i++) {
                        value[i] = line[line_ptr + i];
                    }

                    long val = strtol(value, NULL, 16);
                    if (val <= 0xFF) {
                        ptr->type = HEXADECIMAL_VALUE_8BIT;
                    } else if (val <= 0xFFFF) {
                        ptr->type = HEXADECIMAL_VALUE_16BIT;
                    } else {
                        COMPILER_ERROR("Value out of range at line %d, position %d.", line_num, line_num, line_ptr);
                    }

                    ptr->data = (void*)val;
                    ptr->next = NULL;
                    line_ptr += i;
                }
                break;
            }
            case '#': {
                if (last_char != ' ') {
                    COMPILER_ERROR("Unexpected immediate value definition at line %d, position %d.", line_num, line_num, line_ptr);
                } else {
                    ptr->next = malloc(sizeof(instruction_token));
                    ptr = ptr->next;
                    ptr->type = IMMEDIATE_VALUE_DEFINITION;
                    ptr->data = NULL;
                    ptr->next = NULL;
                    line_ptr++;  // Move past the '#'
                }
                break;
            }
            case '(': {
                if (last_char != ' ') {
                    COMPILER_ERROR("Unexpected opening bracket at line %d, position %d.", line_num, line_num, line_ptr);
                } else {
                    ptr->next = malloc(sizeof(instruction_token));
                    ptr = ptr->next;
                    ptr->type = OPENING_BRACKET;
                    ptr->data = NULL;
                    ptr->next = NULL;
                    line_ptr++;  // Move past the '('
                }
                break;
            }
            case ')': {
                char tests[] = "\n,";
                if (!test_char(line[line_ptr + 1], tests)) {
                    COMPILER_ERROR("Unexpected closing bracket at line %d, position %d.", line_num, line_num, line_ptr);
                } else {
                    ptr->next = malloc(sizeof(instruction_token));
                    ptr = ptr->next;
                    ptr->type = CLOSING_BRACKET;
                    ptr->data = NULL;
                    ptr->next = NULL;

                    if (line[line_ptr + 1] == '\n') {
                        return result;
                    }

                    line_ptr++;  // Move past the ')'
                }
                break;
            }
            case ',': {
                char tests[] = "XYxy";
                if (!test_char(line[line_ptr + 1], tests)) {
                    COMPILER_ERROR("Unexpected comma at line %d, position %d. Expected Index register definition after the ','.", line_num, line_num, line_ptr);
                } else {
                    ptr->next = malloc(sizeof(instruction_token));
                    ptr = ptr->next;
                    ptr->type = COMMA;
                    ptr->data = NULL;
                    ptr->next = NULL;
                    line_ptr++;  // Move past the ','
                }
                break;
            }
            case 'X':
            case 'Y':
            case 'x':
            case 'y': {
                char tests[] = "\n)";
                if (!test_char(line[line_ptr + 1], tests)) {
                    COMPILER_ERROR("Unexpected index register definition at line %d, position %d.", line_num, line_num, line_ptr);
                } else {
                    ptr->next = malloc(sizeof(instruction_token));
                    ptr = ptr->next;
                    ptr->type = INDEX_REGISTER;
                    ptr->data = (void*)(long)line[line_ptr];
                    ptr->next = NULL;

                    if (line[line_ptr + 1] == '\n') {
                        return result;
                    }

                    line_ptr++;  // Move past the index register
                }
                break;
            }
            // Handle other cases like labels, comments, etc., if needed
            default: {
                if (!isspace(line[line_ptr])) {
                    COMPILER_ERROR("Unknown character '%c' at line %d, position %d.", line[line_ptr], line_num, line_num, line_ptr);
                }
                break;
            }
        }
        last_char = line[line_ptr++];
    }

    return result;
}
