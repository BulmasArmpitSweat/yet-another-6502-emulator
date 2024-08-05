#ifndef C8014FEB_51B9_4C92_98BF_E7C5D92B2E36
#define C8014FEB_51B9_4C92_98BF_E7C5D92B2E36
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef signed char byte_raw;
typedef unsigned char u_byte;

typedef unsigned int __useconds_t;
typedef unsigned int uint;

typedef unsigned short ushort;

typedef struct foo
{
    unsigned x:1;
} bit;

typedef enum
{
    IMPLIED,                           // N/A
    ACCUMULATOR,                       // A
    IMMEDIATE,                         // #$nn
    ABSOLUTE,                          // $nnnn
    X_INDEXED_ABSOLUTE,                // $nnnn,X
    Y_INDEXED_ABSOLUTE,                // $nnnn,Y
    ABSOLUTE_INDIRECT,                 // ($nnnn)
    ZERO_PAGE,                         // $nn
    X_INDEXED_ZERO_PAGE,               // $nn,X
    Y_INDEXED_ZERO_PAGE,               // $nn,Y
    X_INDEXED_ZERO_PAGE_INDIRECT,      // ($nn,X)
    ZERO_PAGE_INDIRECT_Y_INDEXED,      // (&nn),Y
    RELATIVE,                          // $nnnn
    NOTHING,                           // N/A
} AddressingModes;

#define MAX_FUNCTION_STACK_LEN         5
#define MAX_MEM_LEN                    UINT16_MAX

/* 0xFFFC */
#define RESET_VECTOR                   MAX_MEM_LEN - 3
/* 0xFFFE */
#define BRK_VECTOR                     MAX_MEM_LEN - 1

/* 256 */
#define Z_PAGE_END                     0x100
/* 512 */
#define STACK_END                      0x200

typedef enum ERROR_CODES
{
    ERR_UNSUPPORTED_ADDR_MODE,
    ERR_NO_INPUT_PROVIDED,
    ERR_REGISTERS_NOT_PRESENT,
    ERR_FUNCTION_POINTER_MISALIGNED,
    ERR_SDL_INIT_FAILED,
    ERR_SDL_WINDOW_ASSERTION_FAILED,
    ERR_SDL_RENDERER_ASSERTION_FAILED,
    ERR_SDL_TEXTURE_ASSERTION_FAILED,
    ERR_BUFFER_OVERFLOW,
    ERR_TESTING_ERROR_CODE
} ERROR_CODES;

    typedef struct
    {
        byte_raw SR;
        u_byte A;
        u_byte X, Y;
        u_byte SP;
        ushort PC;
        byte_raw* mem;

        byte_raw *f_stack;
        byte_raw FSP;
        __useconds_t microsecondsPerCycle;
        bool quit;

        ushort last_accessed_memory_location;
    } cpu;

    typedef struct
    {
        void (*InstructionPointer)(AddressingModes, int cycles, cpu *, bool page_crossed_cycle_exception);
        AddressingModes mode;
        int cycles;
        bool page_crossed_cycle_exception;
    } InstructionInfo;

// TODO: Update implementation to have better error codes

static inline void FATAL_ERROR(ERROR_CODES code) {
    switch (code) {
        case ERR_UNSUPPORTED_ADDR_MODE:
            fprintf(stderr, "Unsupported addr mode!\n");
            exit(-1);
            break;
        case ERR_BUFFER_OVERFLOW:
            fprintf(stderr, "Buffer overflow!\n");
            exit(-1);
            break;
        default:
            fprintf(stderr, "Unknown error!\n");
            exit(-1);
            break;
    }
}
#endif /* C8014FEB_51B9_4C92_98BF_E7C5D92B2E36 */
