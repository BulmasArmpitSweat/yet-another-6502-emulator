#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_MEM_LEN 0xFFFF  // Maximum memory length (65535 or 0xFFFF)

typedef char byte_raw;
typedef unsigned char u_byte;

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
    bool halt;

    ushort last_accessed_memory_location;
    uint64_t nanoseconds_per_cycle;

    char* configured_hertz;
} cpu;

static inline int count_digits(int number) {
    int count = 0;

    // Handle the case where the number is 0
    if (number == 0) {
        return 1;
    }

    // Handle negative numbers by taking their absolute value
    if (number < 0) {
        number = -number;
    }

    while (number != 0) {
        number /= 10;
        count++;
    }

    return count;
}

int main() {
    cpu* main = malloc(sizeof(cpu));  // Allocate memory for the CPU structure
    if (main == NULL) {
        fprintf(stderr, "Failed to allocate memory for CPU.\n");
        return 1;
    }

    main->mem = malloc(MAX_MEM_LEN + 1);  // Allocate memory for CPU memory
    for (int i = 0; i < MAX_MEM_LEN + 1; i++) {
        main->mem[i] = (byte_raw)rand();
    }
    if (main->mem == NULL) {
        fprintf(stderr, "Failed to allocate memory for CPU memory.\n");
        free(main);
        return 1;
    }

    ushort start = 67;
    ushort end = 0x6d9a;  // The last valid memory index is 0xFFFF
    int num_digits = count_digits(end);
    printf("Printing memory contents from 0x%04x (decimal: %*d) to 0x%04x (decimal: %*d):\n", 
           start, num_digits, start, end, num_digits, end);
    
    // Process the memory contents from start to end
    for (int i = start - (start % 16); i < end + (16 - (end % 16)) % 16; i++) {
        if (i % 16 == 0) {
            if (i != start) {
                printf("|\n");  // End the previous line
            }
            printf("0x%04x (decimal: %*d) -> 0x%04x (decimal: %*d):: ", 
                   i, num_digits, i, (i > end) ? end : i + 15, num_digits, (i > end) ? end : i + 15);
        }
        if (i < start)
            printf("-- ");  // Print the memory contents
        else if (i > end)
            printf("-- ");  // Print the memory contents
        else
            printf("%02x ", (unsigned char)main->mem[i]);  // Print the memory contents
    }

    printf("|\n");  // Final newline after loop completes

    // Cleanup
    free(main->mem);
    free(main);
    
    return 0;
}
