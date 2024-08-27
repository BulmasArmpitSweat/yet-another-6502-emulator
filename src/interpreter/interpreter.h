#ifndef FDJKDJF_DJ333HFJD_DJW
#define FDJKDJF_DJ333HFJD_DJW

#include "../include/6502-types.h"

typedef enum {
	MNEMONIC,
	VALUE_DEFINITION,
	IMMEDIATE_VALUE_DEFINITION,
	HEXADECIMAL_VALUE_8BIT,
	HEXADECIMAL_VALUE_16BIT,
	COMMA,
	INDEX_REGISTER,
	OPENING_BRACKET,
	CLOSING_BRACKET,
	ACCUMULATOR_DEFINITION,
	ADDR_MODE_DEFINITION
} token_type;

typedef struct instruction_token {
	token_type type;
    void* data;
    struct instruction_token* next;
} instruction_token;

#define COMPILER_ERROR(message, line_number, ...) fprintf(stderr, "Error on line %d: " message, line_number, __VA_ARGS__); exit(-1)
#define ALLOC_NODE(node) node.next = malloc(sizeof(instruction_token)); node = *node.next

void compile_file(char* in_file, FILE* out_file, const char* out_file_path, instruction_token token_list);
instruction_token parse_line(char* line, int line_num);

#endif /* FDJKDJF_DJ333HFJD_DJW */