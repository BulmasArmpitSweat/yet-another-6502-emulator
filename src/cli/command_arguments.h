#ifndef DFJKDF_jD4JFJD_dSJKJI3J
#define DFJKDF_jD4JFJD_dSJKJI3J

#include <time.h>
#define EXIT_OK 0
#define EXIT_MALFORMED_COMMAND_LINE 1
#define EXIT_CANNOT_CONNECT_TO_HEADLESS_INSTANCE 2
#define EXIT_CANNOT_START_CPU 3
#define EXIT_CANNOT_OPEN_EXTERNAL_FILE 4
#define EXIT_NO_MATCH_FOUND 5

int resolve_external_cli_input(int argc, char** argv);
void print_help(char* command_name);

// TODO
void dump_hex_data(char* input_path, char* output_path);

#endif /* DFJKDF_jD4JFJD_dSJKJI3J */