#include "../include/execute.h"

int handleRType(decoded_fields instr) {
    uint32_t rs1 = regs[instr.r.rs1]; // Soucre register
    uint32_t rs2 = regs[instr.r.rs2]; // Source register
    uint32_t result = 0; // The value to place in the destination register

    switch (instr.r.funct3) {
        case F3_000: // ADD or SUB
            // ADD: funct7 = 0x00
            // SUB: funct7 = 0x20
            result = (instr.r.funct7 == F7_0100000)
                ? rs1 - rs2
                : rs1 + rs2; 
            break;
    
        case F3_001: // SLL
            // Shift logical left
            result = rs1 << (rs2 & 0x1F);
            break;
    
        case F3_010: // SLT
            // SLT: set less than (signed)
            result = ((int32_t) rs1 < (int32_t) rs2) ? 1 : 0;
            break;
    
        case F3_011: // SLTU
            // SLTU: set less than (unsigned)
            result = (rs1 < rs2) ? 1 : 0;
            break;
    
        case F3_100: // XOR
            result = rs1 ^ rs2;
            break;
    
        case F3_101: // SRL or SRA
            // SRL: funct7 = 0x00
            // SRA: funct7 = 0x20
            result = (instr.r.funct7 == F7_0100000)
                ? ((int32_t) rs1 >> (rs2 & 0x1F))   // SRA: shift right arithmetic (sign preserved)
                : (rs1 >> (rs2 & 0x1F));    // SRL: shift right logical
            break;
    
        case F3_110: // OR
            result = rs1 | rs2;
            break;
    
        case F3_111: // AND
            result = rs1 & rs2;
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
int handleIArithmetic(decoded_fields instr){
    return 1;
}
int handleILoad(decoded_fields instr, uint8_t *memory){
    return 1;
}
int handleJALR(decoded_fields instr){
    uint32_t rs1 = regs[instr.i.rs1]; // Soucre register
    imm_t offset = instr.i.imm; 
    uint32_t target = (rs1 + offset) & 0xFFFFFFFE;

    // Like JAL, if we are jumping we need to store the return address (current PC + 4) 
    if (instr.i.rd != ZERO) { // If rd is x0, then we cannot override it (ie. we aren't returning)
        regs[instr.i.rd] = PC + 4;
    }

    PC = target;
    return 0;
}
int handleIType(decoded_fields instr, uint8_t *memory){
    switch (instr.opcode) {
        case IMM: // Arithmetic/logical immediates
            return handleIArithmetic(instr);

        case LOAD: // Loads from memory
            return handleILoad(instr, memory);

        case JALR: // Jump and link register
            return handleJALR(instr);

        default:
            return -1; // Unknown I-type opcode
    }
}
int handleSType(decoded_fields instr, uint8_t *memory){
    uint32_t rs1 = regs[instr.s.rs1];
    uint32_t rs2 = regs[instr.s.rs2];
    imm_t offset = instr.s.imm;
    uint32_t address = rs1 + offset;

    switch (instr.s.funct3) {
        case F3_000: // Store byte
            // rs2 & 0xFF (8 bit mask), ensures we only store 8 bits
            storeByte(memory, address,  rs2 & 0xFF);
            break;
        case F3_001: // Store halfword
            // rs2 & 0xFFFF (16 bit mask), ensures we only store 16 bits
            storeHalfword(memory, address,  rs2 & 0xFFFF);
            break;
        case F3_010: // Store word
            // rs2 (without a mask), as we want the whole 32 bit word
            storeWord(memory, address,  rs2 );
            break;
        default:
            return -1; // Invalid S-Type funct3
    }

    return 0;

}
int handleUType(decoded_fields instr){
    switch (instr.opcode) {
        case LUI: // Load Upper Immediate (loads 20 bit imm (<< 12) into upper 20 bits of reg)
            // rd = imm
            if(instr.u.rd != ZERO){
                regs[instr.u.rd] = instr.u.imm;
            }
            break;
        case AUIPC: // Add Upper Immediate to PC (adds 20 bit imm (<< 12) to PC)
            if(instr.u.rd != ZERO){
                // rd = PC + imm
                regs[instr.u.rd] = PC + instr.u.imm;
            }
            break;
        default:
            return -1; // Invalid U-Type opcode
    }

    return 0;
}
int handleBType(decoded_fields instr) {
    uint32_t rs1 = regs[instr.b.rs1]; // Source register
    uint32_t rs2 = regs[instr.b.rs2]; // Source register
    imm_t pcOffset = instr.b.imm; //

    int shouldBranch = 0;

    switch(instr.b.funct3){
        case F3_000: // BEQ
            // Branch if equal
            shouldBranch = (rs1 == rs2);
            break;
        case F3_001: // BNE
            // Branch if not equal
            shouldBranch = (rs1 != rs2);
            break;
        case F3_100: // BLT
            // Branch if less than (signed)
            shouldBranch = ((int32_t)(rs1) < (int32_t)(rs2));
            break;
        case F3_101: // BGE
            // Branch if greater than or equal (signed)
            shouldBranch = ((int32_t)(rs1) >= (int32_t)(rs2));
            break;
        case F3_110: // BLTU
            // Branch if less than (unsigned)
            shouldBranch = (rs1 < rs2);
            break;
        case F3_111: // BGEU
            // Branch if greater tahn or equal (unsigned)
            shouldBranch = (rs1 > rs2);
            break;
        default:
            return -1; // Invalid B-Type funct3
    }

    if(shouldBranch == 1){
        PC += pcOffset;
    }else{
        PC += 4; // DONT ALSO INCREMENT PC IN MAIN
    }

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