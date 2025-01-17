#include "../include/6502-types.h"
#include "../include/stack-manip.h"
#include "../instruction-table.c"
#include "mem.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "thread.h"

void* cpu_thread_func(void) {
    if (thread_status.destroyed == DESTROYED_TRUE) {
        return NULL;
    }

    struct timespec ts;

    int base_cycles   = instructionTable[main_cpu->mem[main_cpu->PC]].cycles;
    bool page_crossed = !(instructionTable[main_cpu->mem[main_cpu->PC]].page_crossed_cycle_exception) ? ((main_cpu->mem[main_cpu->PC] % 256) ? 1 : 0) : 0;
    int extra_cycles  = page_crossed && instructionTable[main_cpu->mem[main_cpu->PC]].page_crossed_cycle_exception ? 1 : 0;

    ts.tv_sec = (base_cycles + extra_cycles) * (main_cpu->nanoseconds_per_cycle / NANOSECOND_MULTIPLIER);
    ts.tv_nsec = (base_cycles + extra_cycles) * (main_cpu->nanoseconds_per_cycle % NANOSECOND_MULTIPLIER);

    switch (thread_status.load_data) {
        case LOAD_DATA_NONE:
            break;
        
        case LOAD_DATA_F_STACK:
            if (f_stack_transfer_type == f_stack_transfer_8) {
                f_stack_push(main_cpu, (byte_raw)(f_stack_transfer_buffer));
            } else if (f_stack_transfer_type == f_stack_transfer_16) {
                f_stack_push_16(main_cpu, f_stack_transfer_buffer);
            }
            break;
        
        case LOAD_MEMORY:
            main_cpu->mem = memory_transfer_buffer;
            break;
        
        case LOAD_CPU_ATTR:
            main_cpu = &cpu_transfer_buffer;
            break;
    }

    while (thread_status.status == STATUS_SINGLE_STEP && step == 0)
            ;;

    while (thread_status.status == STATUS_PAUSED)
            ;;

    if (thread_status.status == STATUS_HALTED)
        for (;;)
            ;;
    
    if (thread_status.status == STATUS_SINGLE_STEP)
        step--;

    instructionTable[main_cpu->mem[main_cpu->PC]].InstructionPointer(instructionTable[main_cpu->mem[main_cpu->PC]].mode,
    instructionTable[main_cpu->mem[main_cpu->PC]].cycles, main_cpu,
    instructionTable[main_cpu->mem[main_cpu->PC]].page_crossed_cycle_exception);

    main_cpu->PC++;

    nanosleep(&ts, NULL);
    cpu_thread_func();
    wontreturn;
}