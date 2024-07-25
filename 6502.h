#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

typedef signed char byte;
typedef unsigned char u_byte;
typedef bool bit;

typedef unsigned int __useconds_t;
typedef unsigned int uint;

typedef unsigned short ushort;

/* 0xFFFC */
#define RESET_VEC                   UINT16_MAX - 3

#define CARRY_POS                   0
#define ZERO_POS                    1
#define INERRUPTDISABLE_POS         2
#define DECIMALMODE_POS             3
#define BRKCOMMAND_POS              4
/* Nothing */
#define OVERFLOW_POS                6
#define NEGATIVE_POS                7

#define Z_PAGE_END                  0x100
#define STACK_END                   0x200

#define MAX_FUNCTION_STACK_SIZE     5
#define MAX_MEM_SIZE                UINT16_MAX

typedef enum
{
    IMPLIED,                        // N/A
    ACCUMULATOR,                    // A
    IMMEDIATE,                      // #$nn
    ABSOLUTE,                       // $nnnn
    X_INDEXED_ABSOLUTE,             // $nnnn,X
    Y_INDEXED_ABSOLUTE,             // $nnnn,Y
    ABSOLUTE_INDIRECT,              // ($nnnn)
    ZERO_PAGE,                      // $nn
    X_INDEXED_ZERO_PAGE,            // $nn,X
    Y_INDEXED_ZERO_PAGE,            // $nn,Y
    X_INDEXED_ZERO_PAGE_INDIRECT,   // ($nn,X)
    ZERO_PAGE_INDIRECT_Y_INDEXED,   // (&nn),Y
    RELATIVE,                       // $nnnn
    NOTHING,                        // N/A
} AddressingModes;

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
    ERR_TESTING_ERROR_CODE
} ERROR_CODES;

extern char* ADDRESSING_MODES_STRING[15];
extern char* ERROR_CODE_STRING[9];

#if defined(_WIN32) || defined(_WIN64)

    typedef struct
    {
        byte bus;
        bit C, Z, I, D, B, V, N;
        u_byte A, *SP;
        byte X, Y;
        ushort PC;
        byte memory[MAX_MEMORY_SIZE];

        byte function_stack[MAX_FUNCTION_STACK_SIZE];
        byte FSP;
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
#elif defined(__linux__)

    typedef struct __attribute__((packed, aligned(4)))
    {
        byte SR;
        u_byte A;
        byte X, Y;
        ushort PC;
        byte* mem;

        byte function_stack[MAX_FUNCTION_STACK_SIZE];
        byte FSP;
        __useconds_t microsecondsPerCycle;
        bool quit;

        ushort last_accessed_memory_location;
    } cpu;


    typedef struct __attribute__((packed, aligned(4)))
    {
        void (*InstructionPointer)(AddressingModes, int cycles, cpu *, bool page_crossed_cycle_exception);
        AddressingModes mode;
        int cycles;
        bool page_crossed_cycle_exception;
    } InstructionInfo;
#else
#error "Unsupported Platform"
#endif

/*
    ------------------------------------------------------------------------------------------------------
    Websites used:
        https://www.princeton.edu/~mae412/HANDOUTS/Datasheets/6502.pdf
        https://www.masswerk.at/6502/6502_instruction_set.html#SLO
        https://www.pagetable.com/c64ref/6502/?tab=3#

        And a little help from...

        https://chat.openai.com
        https://en.wikipedia.org/wiki/MOS_Technology_6502
        https://github.com/jdah/doomenstein-3d/blob/main/src/main_wolf.c

        Yes, I have been using ChatGPT, but only when i'm too lazy to figure out some more complex things,
        namely how the instruction function pointers work, however the details of how the CPU itself works
        has been pretty much exclusively collected from the three topmost websites.
    ------------------------------------------------------------------------------------------------------
*/

void ADC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Add memory to accumulator with carry
void AND(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // "AND" memory with accumulator
void ASL(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Shift left one bit (memory or accumulator)

void BCC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Branch on carry clear
void BCS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Branch on carry set
void BEQ(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Branch on result zero
void BIT(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Test bits in memory with accumulator
void BMI(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Branch on result minus
void BNE(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Branch on result not zero
void BPL(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Branch on result plus
void BRK(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Force break
void BVC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Branch on overflow clear
void BVS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Branch on overflow set

void CLC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Clear carry flag
void CLD(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Clear decimal mode flag
void CLI(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Clear Interrupt disable flag
void CLV(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Clear overflow flag
void CMP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Compare memory and accumulator
void CPX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Compare memory and index X
void CPY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Compare memory and index Y

void DEC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Decrement memory by one
void DEX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Decrement X index by one
void DEY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Decrement Y index by one

void EOR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // "Exclusive Or" memory with accumulator

void INC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Increment memory by one
void INX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Increment X index by one
void INY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Increment Y index by one

void JMP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Jump to new location
void JSR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Jump to subroutine

void LDA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Load accumulator with memory
void LDX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Load Index X with memory
void LDY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Load Index Y with memory
void LSR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Shift one bit right (Memory or accumulator)

void NOP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // No operation

void ORA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // "Or" memory with accumulator

void PHA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Push accumulator on stack
void PHP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Push processor status on stack
void PLA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Pull accumulator from stack
void PLP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Pull processor status from stack

void ROL(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Rotate one bit left (Memory or accumulator)
void ROR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Rotate one bit right (Memory or accumulator)
void RTI(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Return from interrupt
void RTS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Return from subroutine

void SBC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Subtract memory from accumulator with borrow
void SEC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Set carry flag

void SED(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Set decimal mode flag
void SEI(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Set interrupt disable mode flag
void STA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Store accumulator in memory
void STX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Store index X in memory
void STY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Store index Y in memory

void TAX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Transfer accumulator to index X
void TAY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Transfer accumulator to index Y
void TSX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Transfer stack pointer to index X
void TXA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Transfer index X to accumulator
void TXS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Transfer index X to stack pointer
void TYA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Transfer index Y to accumulator

// --------------------------------------------------- //
// ----------------- Illegal Opcodes ----------------- //
// --------------------------------------------------- //

void JAM(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // For realism; Freezes the CPU

void SLO(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void ANC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void RLA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void SRE(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);

void ASR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void RRA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void ARR(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void SAX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);

void XXA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void SHA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void TAS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void SHY(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);

void SHX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void LAX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void LXA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void LAS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);

void DCP(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void SBX(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void ISC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);
void USBC(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);

void SHS(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception);   // Transfer Accumulator "AND" Index Register X to Stack Pointer then Store Stack Pointer "AND" Hi-Byte In Memory