#include "load.h"

void LDA(AddressingModes addressingMode, int cycles, cpu *cpu, bool page_crossed_cycle_exception) {
    switch (addressingMode) {
        case IMMEDIATE: {
            cpu->A = f_stack_pull(cpu);
        }
    }
}