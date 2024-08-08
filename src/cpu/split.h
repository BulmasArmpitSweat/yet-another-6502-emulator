#ifndef DHFJH4J_NNNJ3NF_33NDKJS
#define DHFJH4J_NNNJ3NF_33NDKJS

#include "../include/6502-types.h"

static inline void split_ushort(ushort input, byte_raw hi, byte_raw lo) {
    hi = (byte_raw)input >> 8;
    lo = (byte_raw)input;
}

#endif /* DHFJH4J_NNNJ3NF_33NDKJS */