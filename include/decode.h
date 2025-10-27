#ifndef DECODE_H
#define DECODE_H

#include <stdint.h>
#include "registers.h"
#include <stdio.h>
#include "instruction.h"

typedef struct{
    instruction_t instrType;
    opcode_t opcode;
    union {
        r_fields r;
        i_fields i;
        s_fields s;
        u_fields u;
        b_fields b;
        j_fields j;
    };
} decoded_fields;


decoded_fields decodeInstruction(uint32_t instr);

instruction_t getInstructionType(opcode_t opcode);
decoded_fields getRelevantFields(instruction_t instrType, opcode_t opcode, uint32_t instr);

static inline opcode_t getOpcode(uint32_t instr)  { return (opcode_t)(instr & 0x7F); }
static inline reg_t getRd(uint32_t instr)         { return (reg_t)((instr >> 7) & 0x1F); }
static inline funct3_t getFunct3(uint32_t instr)  { return (funct3_t)((instr >> 12) & 0x7); }
static inline reg_t getRs1(uint32_t instr)        { return (reg_t)((instr >> 15) & 0x1F); }
static inline reg_t getRs2(uint32_t instr)        { return (reg_t)((instr >> 20) & 0x1F); }
static inline funct7_t getFunct7(uint32_t instr)  { return (funct7_t)((instr >> 25) & 0x7F); }


// I-type: bits [31:20], sign-extended
static inline imm_t getImmIFormat(uint32_t instr) {
    uint32_t imm_11_0 = (instr >> 20) & 0xFFF;
    return (imm_t)((int32_t)(imm_11_0 << 20) >> 20);
}

// S-type: imm[11:5|4:0] = bits [31:25|11:7], sign-extended
static inline imm_t getImmSFormat(uint32_t instr) {
    uint32_t imm_11_5 = (instr >> 25) & 0x7F;
    uint32_t imm_4_0  = (instr >> 7) & 0x1F;
    uint32_t imm = (imm_11_5 << 5) | imm_4_0;
    return (imm_t)((int32_t)(imm << 20) >> 20);
}
// U-type: imm[31:12] << 12
static inline imm_t getImmUFormat(uint32_t instr) {
    uint32_t imm_31_12 = (instr >> 12) & 0xFFFFF;
    return (imm_t)(imm_31_12 << 12);
}

// B-type: imm[12|10:5|4:1|11], sign-extended
static inline imm_t getImmBFormat(uint32_t instr) {
    uint32_t imm12   = (instr >> 31) & 0x1;
    uint32_t imm11   = (instr >> 7) & 0x1;
    uint32_t imm10_5 = (instr >> 25) & 0x3F;
    uint32_t imm4_1  = (instr >> 8) & 0xF;

    uint32_t imm = (imm12 << 12) | (imm11 << 11) | (imm10_5 << 5) | (imm4_1 << 1);
    return (imm_t)((int32_t)(imm << 19) >> 19);
}

// J-type: imm[20|10:1|11|19:12], sign-extended
static inline imm_t getImmJFormat(uint32_t instr) {
    uint32_t imm20    = (instr >> 31) & 0x1;
    uint32_t imm10_1  = (instr >> 21) & 0x3FF;
    uint32_t imm11    = (instr >> 20) & 0x1;
    uint32_t imm19_12 = (instr >> 12) & 0xFF;

    uint32_t imm = (imm20 << 20) | (imm19_12 << 12) | (imm11 << 11) | (imm10_1 << 1);
    return (imm_t)((int32_t)(imm << 11) >> 11);
}


#endif
