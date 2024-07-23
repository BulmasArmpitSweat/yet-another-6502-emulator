#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define CARRY_POS            0
#define ZERO_POS             1
#define INERRUPTDISABLE_POS  2
#define DECIMALMODE_POS      3
#define BRKCOMMAND_POS       4
/* Nothing */
#define OVERFLOW_POS         6
#define NEGATIVE_POS         7

unsigned char A = 0,
              X = 0,
              Y = 0,
              SP = 0,
              SR = 0;

unsigned short PC = 0;

unsigned char* mem;

unsigned char set_bit(unsigned char d, char i) {
    return d | (1 << i);
}

int main() {

    /* Allocate memory block and registers */
    /* 6502 could access up to 65535 bytes (64K) of memory */
    /* This is not the 6510, shut the fuck up */
    mem = calloc(UINT16_MAX, 1);
    if (mem == NULL) return -1;

    // TODO: Do shit

    free(mem);
    return 0;
}