#include <stdio.h>
#include <stdlib.h>

typedef unsigned char u_byte;
typedef signed char byte_raw;

static inline float Q_rsqrt(float number)
{
  long i;
  float x2, y;
  const float threehalfs = 1.5F;

  x2 = number * 0.5F;
  y  = number;
  i  = * ( long * ) &y;                       // evil floating point bit level hacking
  i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
  y  = * ( float * ) &i;
  y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
  // y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

  return y * 100;
}

u_byte XXA() {
    u_byte result = 0, off = rand();
    // Create potential options as base value, and bitmask
    const u_byte vectors[5] = { 0x00, 0xEE, 0XEF, 0XFE, 0XFF };
    const u_byte bitmask = 0b010111000;

    // Leaving random variables uninitialized to increase potential randomness
    // Keep things volatile to make GCC and other compilers happy
    volatile byte_raw operation_result;
    volatile u_byte vector_offset, vector_selection;
    
    // Generate shitty random number
    u_byte low_quality_rand = rand();

    // Extract value from UNIX standard entropy pool
    FILE* random = fopen("/dev/urandom", "r");
    if (random == NULL)
        exit(-1);
    fscanf(random, "%c", &operation_result);
    fclose(random);

    // XOR result because I'm ✨️ fancy ✨️
    operation_result ^= bitmask;

    // AND low quality random number with high-quality one.
    vector_offset = low_quality_rand & operation_result;
    
    // Decide chosen vector from array using high quality random
    vector_selection = vector_offset % 5;

    // Add offset to chosen vector to high quality vector
    operation_result += vectors[vector_selection] + vector_offset;
    
    // Finally, perform core operation
    result = (result & operation_result ^ low_quality_rand) & off & rand();

    // Fuck you. (fast inverse square root's your ass)
    // result = (u_byte)Q_rsqrt((float)result++);
    return result;
}

int main() {
    printf("0x%02x\n", XXA());
    return 0;
}