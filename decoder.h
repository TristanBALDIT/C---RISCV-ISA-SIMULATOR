//
// Created by trist on 27/10/2025.
//

#ifndef DECODER_H
#define DECODER_H

#include <stddef.h>

typedef struct {
    uint8_t opcode;   // [6:0]
    uint8_t rd;       // [11:7]
    uint8_t funct3;   // [14:12]
    uint8_t rs1;      // [19:15]
    uint8_t rs2;      // [24:20]
    uint8_t funct7;   // [31:25]
    int32_t imm;      // immediate (I, S, B, U, J)
} Instruction;

Instruction decode_riscv(uint32_t instr);


#endif //DECODER_H
