#ifndef DJF_3IJ4_FJ44JWJF_DFJI4
#define DJF_3IJ4_FJ44JWJF_DFJI4

#include "../include/6502-types.h"

#define COMMAND_PARSE_FAILED -127

int resolve_cli_input(char* input, cpu* main);
bool get_yes_no_response(char* prompt, bool _default);

#endif /* DJF_3IJ4_FJ44JWJF_DFJI4 */