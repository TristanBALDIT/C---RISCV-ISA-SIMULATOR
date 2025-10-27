#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>
#include "registers.h"
#include <stdio.h>

// Alias for RISCV-I immediate (since all imm are 32 bits, sign-extended) 
typedef int32_t imm_t; 

// Interface for instruction types, RISCV-I has R,I,S,U,B and J types
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
    // F3_ADD_SUB = 0x0,  // 000 -> ADD/SUB
    // F3_SLL = 0x1,      // 001 -> shift left
    // F3_SLT = 0x2,      // 010 -> set less than
    // F3_SLTU = 0x3,     // 011 -> set less than unsigned
    // F3_XOR = 0x4,      // 100 -> XOR
    // F3_SRL_SRA = 0x5,  // 101 -> shift right logical/ shift right arithmetic
    // F3_OR = 0x6,       // 110 -> OR
    // F3_AND = 0x7,      // 111 -> AND
    // F3_SW = 0x2,       // 010 -> Store Word in S format
    // F3_LW = 0x2        // 010 -> Load Word in I format
typedef enum {
    F3_000 = 0x0,
    F3_001 = 0x1,
    F3_010 = 0x2,
    F3_011 = 0x3,
    F3_100 = 0x4,
    F3_101 = 0x5,
    F3_110 = 0x6,
    F3_111 = 0x7
} funct3_t;

// Interface for funct7, are 7 bits in RISCV-I 
//     F7_ADD = 0x0,   // 0000000 -> ADD
//     F7_SUB = 0x20,  // 0100000 -> SUB
//     F7_SLL = 0x0,   // 0000000 -> SLL (no additional funct7 bits)
//     F7_SLT = 0x0,   // 0000000 -> SLT (no additional funct7 bits)
//     F7_SLTU = 0x0,  // 0000000 -> SLTU (no additional funct7 bits)
//     F7_XOR = 0x0,   // 0000000 -> XOR (no additional funct7 bits)
//     F7_SRL = 0x0,   // 0000000 -> SRL 
//     F7_SRA = 0x20,  // 0100000 -> SRA 
//     F7_OR = 0x0,    // 0000000 -> OR (no additional funct7 bits)
//     F7_AND = 0x0,   // 0000000 -> AND (no additional funct7 bits)
//     F7_SLLI = 0x00, // 0000000 -> Constant shift left (specificaly I-Format instructions)
//     F7_SRLI = 0x00, // 0000000 -> Constant shift right (specificaly I-Format instructions)
//     F7_SRAI = 0x20, // 0000000 -> Constant shift right arithmetic (specificaly I-Format instructions)
typedef enum {
    F7_0000000 = 0x00,
    F7_0100000 = 0x20
} funct7_t;

static inline const char *funct3Name(opcode_t opcode, funct3_t f3) {
    switch (opcode) {
        case NONIMM:
            switch (f3) {
                case F3_000: return "ADD/SUB";
                case F3_001: return "SLL";
                case F3_010: return "SLT";
                case F3_011: return "SLTU";
                case F3_100: return "XOR";
                case F3_101: return "SRL/SRA";
                case F3_110: return "OR";
                case F3_111: return "AND";
                default:     return "UNKNOWN";
            }

        case IMM:
            switch (f3) {
                case F3_000: return "ADDI";
                case F3_010: return "SLTI";
                case F3_011: return "SLTIU";
                case F3_100: return "XORI";
                case F3_110: return "ORI";
                case F3_111: return "ANDI";
                case F3_001: return "SLLI";
                case F3_101: return "SRLI/SRAI";
                default:
                    return "UNKNOWN";
            }

        case LOAD:
            switch (f3) {
                case F3_010: return "LW";
                default:
                    return "UNKNOWN";
            }

        case STORE:
            switch (f3) {
                case F3_010: return "SW";
                default:
                    return "UNKNOWN";
            }

        case BRANCH:
            switch (f3) {
                case F3_000: return "BEQ";
                case F3_001: return "BNE";
                case F3_100: return "BLT";
                case F3_101: return "BGE";
                case F3_110: return "BLTU";
                case F3_111: return "BGEU";
                default:
                    return "UNKNOWN";
            }

        default:
            return "UNKNOWN";
    }
}
static inline const char *funct7Name(opcode_t opcode, funct3_t funct3, funct7_t funct7) {
    if (opcode == NONIMM) { // R-type
        if (funct3 == F3_000) {
            return (funct7 == F7_0100000) ? "SUB" : "ADD";
        } else if (funct3 == F3_101) {
            return (funct7 == F7_0100000) ? "SRA" : "SRL";
        } else {
            return "STANDARD"; // funct7 = 0
        }
    }

    return "UNUSED";
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
        case BRANCH:
            return "BRANCH";
        case JALR:
            return "JALR";
        case NONIMM: 
            return "NONIMM";  // ADD, SUB, etc.
        case LUI:    
            return "LUI";
        case JAL:
            return "JAL"; 
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