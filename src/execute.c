#include "../include/execute.h"


int handleRType(decoded_fields instr) {
    uint32_t rs1_val = regs[instr.r.rs1];
    uint32_t rs2_val = regs[instr.r.rs2];
    uint32_t result = 0;

    switch (instr.r.funct3) {
        case F3_ADD_SUB:
            result = (instr.r.funct7 == F7_SUB)
                ? rs1_val - rs2_val
                : rs1_val + rs2_val;
            break;
        case F3_SLL:
            result = rs1_val << (rs2_val & 0x1F);
            break;
        case F3_SLT:
            result = ((int32_t) rs1_val < (int32_t)rs2_val ) ? 1 : 0;
            break;
        case F3_SLTU:
            result = (rs1_val < rs2_val) ? 1 : 0;
            break;
        case F3_XOR:
            result = rs1_val ^ rs2_val;
            break;
        case F3_SRL_SRA:
            result = (instr.r.funct7 == F7_SRA)
                ? ((int32_t) rs1_val >> (rs2_val & 0x1F))
                : (rs1_val >> (rs2_val & 0x1F));
            break;
        case F3_OR:
            result = rs1_val | rs2_val;
            break;
        case F3_AND:
            result = rs1_val & rs2_val;
            break;
        default:
            return -1;
    }

    if (instr.r.rd != ZERO) {
        regs[instr.r.rd] = result;
    }

    return 0;
}

int handleBType(decoded_fields instr) {return 0;}
int handleJType(decoded_fields instr) {return 0;}

int executeInstruction(decoded_fields instr, uint8_t *memory){
    switch(instr.instrType){
    case R_TYPE:
        handleRType(instr);
        break;
    case I_TYPE:
        handleIType(instr, memory);
        break;
    case S_TYPE:
        handleSType(instr, memory);
        break;
    case U_TYPE:
        handleUType(instr);
        break;
    case B_TYPE:
        handleBType(instr);
        break;
    case J_TYPE:
        handleJType(instr);
        break;
    default:
        return -1;
    }
    return 0;
}