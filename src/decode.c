#include "../include/decode.h"
#include <stdint.h>
#include <stdlib.h>

instruction_t getInstructionType(opcode_t opcode){
    switch(opcode){   
        case NONIMM: 
            return R_TYPE;
        case LOAD: 
            return I_TYPE;
        case IMM: 
            return I_TYPE;
        case JALR:
            return I_TYPE;
        case SYSTEM: 
            return I_TYPE;
        case STORE: 
            return S_TYPE;
        case AUIPC: 
            return U_TYPE;
        case LUI: 
            return U_TYPE;
        case BRANCH:
            return B_TYPE;
        case JAL:
            return J_TYPE;
        default: 
            return UNKNOWN_TYPE;
    }
}

decoded_fields getRelevantFields(instruction_t instrType, opcode_t opcode, uint32_t instr){
    decoded_fields decodedInstr;
    // Set universal fields
    decodedInstr.instrType = instrType;
    decodedInstr.opcode = opcode;

    switch(instrType){
        case R_TYPE:
            decodedInstr.r.rd = getRd(instr);
            decodedInstr.r.rs1 = getRs1(instr);
            decodedInstr.r.rs2 = getRs2(instr);
            decodedInstr.r.funct3 = getFunct3(instr);
            decodedInstr.r.funct7 = getFunct7(instr);
            break;
        case I_TYPE:
            decodedInstr.i.rd = getRd(instr);
            decodedInstr.i.rs1 = getRs1(instr);
            decodedInstr.i.funct3 = getFunct3(instr);
            decodedInstr.i.imm = getImmIFormat(instr);
            break;
        case S_TYPE:
            decodedInstr.s.rs1 = getRs1(instr);
            decodedInstr.s.rs2 = getRs2(instr);
            decodedInstr.s.funct3 = getFunct3(instr);
            decodedInstr.s.imm = getImmSFormat(instr);
            break;
        case U_TYPE:
            decodedInstr.u.rd = getRd(instr);
            decodedInstr.u.imm = getImmUFormat(instr);
            break;
        case B_TYPE:
            decodedInstr.b.rs1 = getRs1(instr);
            decodedInstr.b.rs2 = getRs2(instr);
            decodedInstr.b.funct3 = getFunct3(instr);
            decodedInstr.b.imm = getImmBFormat(instr);
            break;
        case J_TYPE:
            decodedInstr.j.rd = getRd(instr);
            decodedInstr.j.imm = getImmJFormat(instr);
            break;
        case UNKNOWN_TYPE:
            // TODO BAD INSTRUCTION
            break;
        default:
            // TODO BAD INSTRUCTION
            break;
    }
    return decodedInstr;
}

static void debugPrintInstructionFields(decoded_fields decoded) {

    switch (decoded.instrType) {
        case R_TYPE:
            printf("R-type:\n");
            printf("Opcode: 0x%02X (%s)\n", decoded.opcode, opcodeName(decoded.opcode));
            printf("  rd     = x%d (%s)\n", decoded.r.rd, regName(decoded.r.rd));
            printf("  rs1    = x%d (%s)\n", decoded.r.rs1, regName(decoded.r.rs1));
            printf("  rs2    = x%d (%s)\n", decoded.r.rs2, regName(decoded.r.rs2));
            printf("  funct3 = 0x%X (%s)\n", decoded.r.funct3, funct3Name(decoded.opcode, decoded.r.funct3));
            printf("  funct7 = 0x%X (%s)\n", decoded.r.funct7, funct7Name(decoded.opcode, decoded.r.funct3, decoded.r.funct7));
            break;

        case I_TYPE:
            printf("I-type:\n");
            printf("Opcode: 0x%02X (%s)\n", decoded.opcode, opcodeName(decoded.opcode));
            printf("  rd     = x%d (%s)\n", decoded.i.rd, regName(decoded.i.rd));
            printf("  rs1    = x%d (%s)\n", decoded.i.rs1, regName(decoded.i.rs1));
            printf("  funct3 = 0x%X (%s)\n", decoded.i.funct3, funct3Name(decoded.opcode, decoded.i.funct3));
            printf("  imm    = %d (%#010x) \n",decoded.i.imm, (uint32_t)decoded.i.imm);
            break;

        case S_TYPE:
            printf("S-type:\n");
            printf("Opcode: 0x%02X (%s)\n", decoded.opcode, opcodeName(decoded.opcode));
            printf("  rs1    = x%d (%s)\n", decoded.s.rs1, regName(decoded.s.rs1));
            printf("  rs2    = x%d (%s)\n", decoded.s.rs2, regName(decoded.s.rs2));
            printf("  funct3 = 0x%X (%s)\n", decoded.s.funct3, funct3Name(decoded.opcode, decoded.s.funct3));
            printf("  imm    = %d (%#010x) \n",decoded.s.imm, (uint32_t)decoded.s.imm);
            break;

        case U_TYPE:
            printf("U-type:\n");
            printf("Opcode: 0x%02X (%s)\n", decoded.opcode, opcodeName(decoded.opcode));
            printf("  rd     = x%d (%s)\n", decoded.u.rd, regName(decoded.u.rd));
            printf("  imm    = %d (%#010x) \n",decoded.u.imm, (uint32_t)decoded.u.imm);
            break;
        
        case B_TYPE:
            printf("B-type:\n");
            printf("Opcode: 0x%02X (%s)\n", decoded.opcode, opcodeName(decoded.opcode));
            printf("  rs1    = x%d (%s)\n", decoded.b.rs1, regName(decoded.b.rs1));
            printf("  rs2    = x%d (%s)\n", decoded.b.rs2, regName(decoded.b.rs2));
            printf("  funct3 = 0x%X (%s)\n", decoded.b.funct3, funct3Name(decoded.opcode, decoded.b.funct3));
            printf("  imm    = %d (%#010x) \n", decoded.b.imm, (uint32_t)decoded.b.imm);
            break;

        case J_TYPE:
            printf("J-type:\n");
            printf("Opcode: 0x%02X (%s)\n", decoded.opcode, opcodeName(decoded.opcode));
            printf("  rd     = x%d (%s)\n", decoded.j.rd, regName(decoded.j.rd));
            printf("  imm    = %d (%#010x) \n", decoded.j.imm, (uint32_t)decoded.j.imm);
            break;

        default:
            printf("Unknown format.\n");
            break;
    }
    printf("\n");
}


decoded_fields decodeInstruction(uint32_t instr){
    // Extra debug information - Prints the formated instruction
    // printf("Instruction: 0x%08X\n", instr);
    
    opcode_t opcode = getOpcode(instr);
    instruction_t instrType = getInstructionType(opcode);
    decoded_fields decoded = getRelevantFields(instrType, opcode, instr);

    // Extra debug information - Prints the values stored in decoded_fields
    // debugPrintInstructionFields(decoded);
    
    return decoded;
}



