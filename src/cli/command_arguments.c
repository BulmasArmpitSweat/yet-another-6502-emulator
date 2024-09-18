#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "command_arguments.h"
#include "../cpu/mem.h"

int resolve_external_cli_input(int argc, char** argv) {
    
    for (int i = 0; i < argc; i++)
        printf(" %s |", argv[i]);
    printf("\n%s\n", argv[1]);

    if (
        strcmp(argv[1], "-h") == 0 ||
        strcmp(argv[1], "-?") == 0 ||
        strcmp(argv[1], "--help") == 0
    ) {
        print_help(argv[0]);
        exit(EXIT_OK);
    } 
    if (
        strcmp(argv[1], "-d") == 0 ||
        strcmp(argv[1], "--dump") == 0
    ) {
        if (argc < 4) {
            fprintf(stderr, "[ERROR]: Malformed command arguments. Expected input and output filepath, but got none.\n");
            print_help(argv[0]);
            exit(EXIT_MALFORMED_COMMAND_LINE);
            if (access(argv[2], F_OK) != 0) {
                fprintf(stderr, "[ERROR]: Cannot find input file.\n");
                exit(EXIT_CANNOT_OPEN_EXTERNAL_FILE);
            }
            dump_hex_data(argv[2], argv[3]);
            exit(EXIT_OK);
        }
    } 
    if (
        strcmp(argv[1], "-C") == 0 ||
        strcmp(argv[1], "--compile") == 0
    ) {
        if (access("./bin/timu6502asm", F_OK) != 0) {
            if (argc < 4) {
                printf("[Error]: Malformed command arguments. Expected input and output filepath, but got none.\n");
                print_help(argv[0]);
                exit(EXIT_MALFORMED_COMMAND_LINE);
            }
            char command[200] = {};
            sprintf(command, "./bin/timu6502asm %s %s", argv[2], argv[3]);
            // Will replace with a better implementation later
            system(command);
        }
        exit(EXIT_OK);
    }
    return EXIT_NO_MATCH_FOUND;
}

/* Help, Headless, Dump, Connect, Compile, register, unregister */
void print_help(char* command_name) {
    printf("Usage: %d [OPTION](-h?dc)... [FILE]\n", *command_name);
    printf("Accurately emulate a MOS 6502 processor\n\n");

    printf("-h, -?, --help      Show this message\n");
    printf("-d, --dump          Dump hex data of a file to an external .hex file\n");
    printf("-c, --compile       Compile 6502 assembly to a 6502 bytecode .bin file\n");

    printf("Exit codes:\n\n");
    printf("  0 if OK,\n");
    printf("  1 if Malformed command-line arguments,\n");
    printf("  2 if Failed to start CPU (Initialization failed),\n");
    printf("  3 if Cannot open external file\n");
}

void dump_hex_data(char *input_path, char *output_path) {
    FILE* input = fopen(input_path, "r");
    FILE* output = fopen(output_path, "w");

    char data[] = {};
    int ptr = 0;
    while (!feof(input)) {
        data[ptr++] = fgetc(input);
    }
    int start, end, num_digits;
    start = sizeof(data);
    end = 0;
    num_digits = count_digits(end);
    
        for (int i = start - (start % 16); i < end + (16 - (end % 16)) % 16; i++) {
            if (i % 16 == 0) {
                if (i != start) {
                    fprintf(output, "|\n");
                }
                fprintf(output, "0x%04x (decimal: %*d) -> 0x%04x (decimal: %*d):: ", 
                       i, num_digits, i, (i > end) ? end : i + 15, num_digits, (i > end) ? end : i + 15);
            }
            if (i < start)
                fprintf(output, "-- ");
            else if (i > end)
                fprintf(output, "-- ");
            else
                fprintf(output, "%02x ", (unsigned char)data[i]);
        }
    fprintf(output, "|\n");
}