#include "../include/6502-types.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <string.h>
#include "mem.h"
#include "thread.h"
#include "init.h"

void _6502_start_cpu(cpu* main) {
    init_obj thread_args;
    thread_args.CPUObjInit = *main;
    pthread_t thread;
    if (pthread_create(&thread, NULL, cpu_thread_func, (void*)&thread_args) != 0) {
        FATAL_ERROR(ERR_THREAD_INITIALIZATION_FAILED);
    }
    Message message;
    message.type = MSG_RESUME;
    message.data = NULL;
    enqueue_message(message);
}

void _6502_prepopulate_values(cpu* cpu) {
    /* Basic registers */
    cpu->A = 0;
    cpu->PC = 0;
    cpu->SP = 0;
    cpu->SR = 0;
    cpu->X = 0;
    cpu->Y = 0;
    
    /* Function stack | For internal use only */
    cpu->FSP = 0;
    cpu->f_stack = (byte_raw* )calloc(MAX_FUNCTION_STACK_LEN, sizeof(byte_raw));
    (cpu->f_stack == NULL) ? FATAL_ERROR(ERR_MEMORY_INITIALIZATION_FAILED):((void)0);

    cpu->halt = false;
    
    /* VV Fuck you I don't care that this is obsolete code VV */
    cpu->mem = (byte_raw *)malloc(MAX_MEM_LEN * sizeof(byte_raw));
    (cpu->mem == NULL) ? FATAL_ERROR(ERR_MEMORY_INITIALIZATION_FAILED):((void)0);
}

void _6502_set_reset_vec(cpu* cpu, byte_raw lo, byte_raw hi) {
    cpu->mem[RESET_VECTOR] = lo;
    cpu->mem[RESET_VECTOR+1] = hi;
}

void _6502_set_brk_vec(cpu* cpu, byte_raw lo, byte_raw hi) {
    cpu->mem[MAX_MEM_LEN-1] = lo;
    cpu->mem[MAX_MEM_LEN] = hi;
}

void _6502_calculate_nanoseconds_per_cycle(cpu *main) {
    char hertz[20];
    char hertz_level;
    int configured_hertz;

    printf("Clock Speed? (default: 1MHz): ");
    fgets(hertz, sizeof(hertz), stdin);

    // Check if the input is empty or newline (default case)
    if (hertz[0] == '\n') {
        configured_hertz = 1;  // Default to 1MHz
        hertz_level = 'M';
    } else {
        // Attempt to parse the input with a unit
        if (sscanf(hertz, "%d%cHz", &configured_hertz, &hertz_level) != 2) {
            // If parsing fails, treat input as Hz directly
            configured_hertz = atoi(hertz);
            hertz_level = ' ';
        }
    }

    // Convert to nanoseconds per cycle
    switch (hertz_level) {
        case 'K':  // Kilohertz
            main->nanoseconds_per_cycle = 1000000 / configured_hertz;
            break;
        case 'M':  // Megahertz
            main->nanoseconds_per_cycle = 1000 / configured_hertz;
            break;
        case 'G':  // Gigahertz
            main->nanoseconds_per_cycle = 1 / configured_hertz;
            break;
        default:  // Hertz (or unspecified unit)
            main->nanoseconds_per_cycle = 1000000000 / configured_hertz;
            break;
    }

    printf("Configured Hertz: %d%cHz\n", configured_hertz, hertz_level);
    printf("Nanoseconds per cycle: %ld ns\n", main->nanoseconds_per_cycle);
}

void _6502_mount_external_file(cpu* main) {
    char file_path[101];
    int file_size;
    printf("File to read: ");
    scanf("%0100s", file_path);
    if (access(file_path, F_OK)) {
        printf("ERROR: file either doesn't exist, or you don't have read permissions for it. Aborting read\n");
        return;
    }
    FILE* file = fopen(file_path, "rb");
    ushort starting_pos;
    while (true) {
        char input[20];
        printf("Starting position for data (default : 0x00FF): ");
        scanf("%s", input);
        if (strcmp(input, "\n") == 0) {
            starting_pos = 0x00FF;
            goto conversion_skip;
        }
        starting_pos = strtol(input, NULL, 16);
    conversion_skip:
        if (starting_pos < 0x00FF) {
            printf("Warning: File data encroaches on stack memory (0x0000 -> 0x00FF). File data integrity cannot be guaranteed");
        }
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        if (file_size > ((MAX_MEM_LEN - starting_pos) - 4 /* Reset and Brk vectors take up 4 bytes */)) {
            printf("ERROR: filesize too large to fit in the requested memory-space. Consider using a smaller file, or different memory location\n");
            continue;
        }
    }
        fseek(file, 0, SEEK_SET);
        printf("Writing file to memory");
        for (ushort i = starting_pos; i < (starting_pos + file_size); i++) {
            main->mem[i] = fgetc(file);
        }
        while (true) {
            char input;
            printf("Would you like to set the reset vector to jump to the start of the inputted data?: (default: yes): ");
            scanf("%c", &input);
            char tests[3] = {'y', 'n', '\n'};
            if (test_char(input, tests) == false) {
                printf("That's not a valid input, kiddo\n");
                continue;
            }
            if (input == 'n')
                break;
            /* Set reset vector to: JMP $<starting_pos> */
            _6502_set_reset_vec(main, 0x004C, starting_pos);
        }
    }

void _6502_un_start_cpu() {
    Message mem = {
        .type = MSG_DESTROY,
        .data = NULL
    };
    enqueue_message(mem);
}