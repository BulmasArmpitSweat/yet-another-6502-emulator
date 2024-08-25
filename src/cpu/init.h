#ifndef JFJKJ_DJSJ3N_DJ3UN_38UMNVII
#define JFJKJ_DJSJ3N_DJ3UN_38UMNVII

#include "../include/6502-types.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <string.h>
#include "mem.h"
#include "thread.h"

void _6502_start_cpu(cpu* main);
void _6502_prepopulate_values(cpu* cpu);
void _6502_set_reset_vec(cpu* cpu, byte_raw lo, byte_raw hi);
void _6502_set_brk_vec(cpu* cpu, byte_raw lo, byte_raw hi);
void _6502_calculate_nanoseconds_per_cycle(cpu *main);
void _6502_mount_external_file(cpu* main);
void _6502_un_start_cpu();

#endif /* JFJKJ_DJSJ3N_DJ3UN_38UMNVII */