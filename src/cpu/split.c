#include "../include/6502-types.h"
#include "split.h"

void split_ushort(ushort input, byte_raw hi, byte_raw lo) {
    hi = (byte_raw)input >> 8;
    lo = (byte_raw)input;
}