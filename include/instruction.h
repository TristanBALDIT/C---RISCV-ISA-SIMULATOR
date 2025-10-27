#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>
#include "registers.h"
#include <stdio.h>

// Alias for RISCV-I immediate (since all imm are 32 bits, sign-extended) 
typedef int32_t imm_t; 

// Interface for instruction types, RISCV-I only has R,I,S,and U types
typedef enum{
    R_TYPE,
    I_TYPE,
    S_TYPE,
    U_TYPE,
    B_TYPE,
    J_TYPE,
    UNKNOWN_TYPE
} instruction_t;

// Interface for the opcodes, RISCV-I opcodes are 7 bits
typedef enum{
    LOAD = 0x03,    // 0000011 -> LB, LH, LW, LBU, LHU 
    IMM = 0x13,     // 0010011 -> ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI
    AUIPC = 0x17,   // 0010111 -> AUIPC
    STORE = 0x23,   // 0100011 -> SB, SH, SW
    BRANCH = 0x63,  // 1100011 -> BEQ, BNE, BLT, BGE, BLTU, BGEU
    JALR = 0x67,    // 1100111 => JALR
    NONIMM = 0x33,  // 0110011 -> ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND
    LUI = 0x37,     // 0110111 -> LUI
    JAL = 0x6F,     // 1101111 -> JAL
    SYSTEM = 0x73,  // 1110011 -> ECALL
} opcode_t;

// Interface for funct3, RISCV-I are 3 bits
typedef enum{
    F3_ADD_SUB = 0x0,  // 000 -> ADD/SUB
    F3_SLL = 0x1,      // 001 -> shift left
    F3_SLT = 0x2,      // 010 -> set less than
    F3_SLTU = 0x3,     // 011 -> set less than unsigned
    F3_XOR = 0x4,      // 100 -> XOR
    F3_SRL_SRA = 0x5,  // 101 -> shift right logical/ shift right arithmetic
    F3_OR = 0x6,       // 110 -> OR
    F3_AND = 0x7,      // 111 -> AND
    F3_SW = 0x2,       // 010 -> store word in S format
    F3_LW = 0x2        // 010 -> load word in I format
} funct3_t;

// Interface for funct7, are 7 bits in RISCV-I 
typedef enum{
    F7_ADD = 0x0,   // 0000000 -> ADD
    F7_SUB = 0x20,  // 0100000 -> SUB
    F7_SLL = 0x0,   // 0000000 -> SLL (no additional funct7 bits)
    F7_SLT = 0x0,   // 0000000 -> SLT (no additional funct7 bits)
    F7_SLTU = 0x0,  // 0000000 -> SLTU (no additional funct7 bits)
    F7_XOR = 0x0,   // 0000000 -> XOR (no additional funct7 bits)
    F7_SRL = 0x0,   // 0000000 -> SRL 
    F7_SRA = 0x20,  // 0100000 -> SRA 
    F7_OR = 0x0,    // 0000000 -> OR (no additional funct7 bits)
    F7_AND = 0x0,   // 0000000 -> AND (no additional funct7 bits)
    F7_SLLI = 0x00, // 0000000 -> Constant shift left (specificaly I-Format instructions)
    F7_SRLI = 0x00, // 0000000 -> Constant shift right (specificaly I-Format instructions)
    F7_SRAI = 0x20, // 0000000 -> Constant shift right arithmetic (specificaly I-Format instructions)
} funct7_t;

static inline const char *funct3Name(funct3_t f3) {
    switch (f3) {
        case F3_ADD_SUB: 
            return "ADD/SUB";
        case F3_SLL:
            return "SLL";
        case F3_SLT:      
            return "SLT";
        case F3_SLTU:     
            return "SLTU";
        case F3_XOR:      
            return "XOR";
        case F3_SRL_SRA:  
            return "SRL/SRA";
        case F3_OR: 
            return "OR";
        case F3_AND:      
            return "AND";
        default:
            return "UNKNOWN";
    }
}
static inline const char *opcodeName(opcode_t opcode) {
    switch (opcode) {
        case LOAD:   
            return "LOAD";    // LB, LH, LW, LBU, LHU
        case IMM:    
            return "IMM";     // ADDI, SLTI, etc.
        case AUIPC:  
            return "AUIPC";
        case STORE:  
            return "STORE";   // SB, SH, SW
        case NONIMM: 
            return "NONIMM";  // ADD, SUB, etc.
        case LUI:    
            return "LUI";
        case SYSTEM: 
            return "SYSTEM";  // ECALL
        default:     
            return "UNKNOWN";
    }
}


typedef struct {
    reg_t rd;
    reg_t rs1;
    reg_t rs2;
    funct3_t funct3;
    funct7_t funct7;
} r_fields;

typedef struct {
    reg_t rd;
    reg_t rs1;
    funct3_t funct3;
    imm_t imm;
} i_fields;

typedef struct {
    reg_t rs1;
    reg_t rs2;
    funct3_t funct3;
    imm_t imm;
} s_fields;

typedef struct {
    reg_t rd;
    imm_t imm;
} u_fields;

typedef struct {
    reg_t rs1;
    reg_t rs2;
    funct3_t funct3;
    imm_t imm;
} b_fields;

typedef struct {
    reg_t rd;
    imm_t imm;
} j_fields;


#endif