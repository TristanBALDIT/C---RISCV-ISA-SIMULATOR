#include "../include/execute.h"


int handleRType(decoded_fields instr) {
    uint32_t rs1_val = regs[instr.r.rs1]; // Soucre register
    uint32_t rs2_val = regs[instr.r.rs2]; // Source register
    uint32_t result = 0; // The value to place in the destination register

    switch (instr.r.funct3) {
        case F3_000: // ADD or SUB
            // ADD: funct7 = 0x00
            // SUB: funct7 = 0x20
            result = (instr.r.funct7 == F7_0100000)
                ? rs1_val - rs2_val
                : rs1_val + rs2_val; 
            break;
    
        case F3_001: // SLL
            // Shift logical left
            result = rs1_val << (rs2_val & 0x1F);
            break;
    
        case F3_010: // SLT
            // SLT: set less than (signed)
            result = ((int32_t) rs1_val < (int32_t) rs2_val) ? 1 : 0;
            break;
    
        case F3_011: // SLTU
            // SLTU: set less than (unsigned)
            result = (rs1_val < rs2_val) ? 1 : 0;
            break;
    
        case F3_100: // XOR
            result = rs1_val ^ rs2_val;
            break;
    
        case F3_101: // SRL or SRA
            // SRL: funct7 = 0x00
            // SRA: funct7 = 0x20
            result = (instr.r.funct7 == F7_0100000)
                ? ((int32_t) rs1_val >> (rs2_val & 0x1F))   // SRA: shift right arithmetic (sign preserved)
                : (rs1_val >> (rs2_val & 0x1F));    // SRL: shift right logical
            break;
    
        case F3_110: // OR
            result = rs1_val | rs2_val;
            break;
    
        case F3_111: // AND
            result = rs1_val & rs2_val;
            break;
    
        default: // Unknown funct3
            return -1;
    }
    
    // Prevents destination register from updating if its the x0 (ZERO) register
    if (instr.r.rd != ZERO) {
        regs[instr.r.rd] = result;
    }

    return 0;
}
int handleIType(decoded_fields instr, uint8_t *memory){
    return 0;
}
int handleSType(decoded_fields instr, uint8_t *memory){
    return 0;
}
int handleUType(decoded_fields instr){
    return 0;
}
int handleBType(decoded_fields instr) {
    return 0;
}
int handleJType(decoded_fields instr) {
    imm_t pcOffset = instr.j.imm;

    // Since we are jumping we need to store the return address (current PC + 4) 
    if (instr.j.rd != ZERO){  // If rd is x0, then we cannot override it (ie. we aren't returning)
        regs[instr.j.rd] = PC + 4;
    }
    PC += pcOffset;

    return 0;
}

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