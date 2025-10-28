#ifndef REGS_H
#define REGS_H

#include <stdint.h>

#define NUM_REGS 32

// Register file: x0 to x31
extern uint32_t regs[NUM_REGS];

// 32-bit program counter
extern uint32_t PC;

// Enum for readability 
typedef enum {
    ZERO = 0,  // x0: Constant zero
    RA   = 1,  // x1: Return address
    SP   = 2,  // x2: Stack pointer
    GP   = 3,  // x3: Global pointer
    TP   = 4,  // x4: Thread pointer
    T0   = 5,  // x5: Temporary register 0
    T1   = 6,  // x6: Temporary register 1
    T2   = 7,  // x7: Temporary register 2
    S0   = 8,  // x8: Saved register 0
    FP   = 8,  // x8: Alias for frame pointer
    S1   = 9,  // x9: Saved register 1
    A0   = 10, // x10: Function argument 0 / Return value
    A1   = 11, // x11: Function argument 1
    A2   = 12, // x12: Function argument 2
    A3   = 13, // x13: Function argument 3
    A4   = 14, // x14: Function argument 4
    A5   = 15, // x15: Function argument 5
    A6   = 16, // x16: Function argument 6
    A7   = 17, // x17: Function argument 7
    S2   = 18, // x18: Saved register 2
    S3   = 19, // x19: Saved register 3
    S4   = 20, // x20: Saved register 4
    S5   = 21, // x21: Saved register 5
    S6   = 22, // x22: Saved register 6
    S7   = 23, // x23: Saved register 7
    S8   = 24, // x24: Saved register 8
    S9   = 25, // x25: Saved register 9
    S10  = 26, // x26: Saved register 10
    S11  = 27, // x27: Saved register 11
    T3   = 28, // x28: Temporary register 3
    T4   = 29, // x29: Temporary register 4
    T5   = 30, // x30: Temporary register 5
    T6   = 31  // x31: Temporary register 6
} reg_t;

static inline const char *regName(reg_t r) {
    static const char *names[NUM_REGS] = {
        "ZERO", "RA", "SP", "GP", "TP", "T0", "T1", "T2",
        "S0/FP", "S1", "A0", "A1", "A2", "A3", "A4", "A5",
        "A6", "A7", "S2", "S3", "S4", "S5", "S6", "S7",
        "S8", "S9", "S10", "S11", "T3", "T4", "T5", "T6"
    };
    return names[r];
}


#endif
