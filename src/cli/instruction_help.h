#ifndef DFDJK_dJI3JFNJE_DNKSFNEI
#define DFDJK_dJI3JFNJE_DNKSFNEI
#include "../config.h"
#include "../include/6502-types.h"

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

void print_instruction_help(help_message instruction);
#endif /* DFDJK_dJI3JFNJE_DNKSFNEI */