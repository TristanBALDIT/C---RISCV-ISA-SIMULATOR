#include <float.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "decoder.h"

// Decode a single 32-bit RISC-V instruction
Instruction decode_riscv(uint32_t instr) {
    Instruction decoded;
    decoded.opcode = instr & 0x7F;
    decoded.rd     = (instr >> 7) & 0x1F;
    decoded.funct3 = (instr >> 12) & 0x07;
    decoded.rs1    = (instr >> 15) & 0x1F;
    decoded.rs2    = (instr >> 20) & 0x1F;
    decoded.funct7 = (instr >> 25) & 0x7F;
    decoded.imm    = 0;

    // Decode immediate based on instruction type
    switch(decoded.opcode) {
        case 0x03: // I-type: load
        case 0x13: // I-type: ALU immediate
        case 0x67: // I-type: JALR
            decoded.imm = (int32_t)instr >> 20; // bits [31:20], sign-extended
            break;
        case 0x23: { // S-type: store
            int32_t imm11_5 = (instr >> 25) & 0x7F;
            int32_t imm4_0  = (instr >> 7) & 0x1F;
            decoded.imm = (imm11_5 << 5) | imm4_0;
            if (decoded.imm & 0x800) decoded.imm |= 0xFFFFF000; // sign-extend
            break;
        }
        case 0x63: { // B-type: branch
            int32_t imm12   = (instr >> 31) & 0x1;
            int32_t imm10_5 = (instr >> 25) & 0x3F;
            int32_t imm4_1  = (instr >> 8) & 0x0F;
            int32_t imm11   = (instr >> 7) & 0x01;
            decoded.imm = (imm12 << 12) | (imm11 << 11) | (imm10_5 << 5) | (imm4_1 << 1);
            if (decoded.imm & 0x1000) decoded.imm |= 0xFFFFE000; // sign-extend
            break;
        }
        case 0x17: // U-type: AUIPC
        case 0x37: // U-type: LUI
            decoded.imm = instr & 0xFFFFF000;
            break;
        case 0x6F: { // J-type: JAL
            int32_t imm20   = (instr >> 31) & 0x1;
            int32_t imm10_1 = (instr >> 21) & 0x3FF;
            int32_t imm11   = (instr >> 20) & 0x1;
            int32_t imm19_12= (instr >> 12) & 0xFF;
            decoded.imm = (imm20 << 20) | (imm19_12 << 12) | (imm11 << 11) | (imm10_1 << 1);
            if (decoded.imm & 0x100000) decoded.imm |= 0xFFE00000; // sign-extend
            break;
        }
        default: // R-type or unknown
            decoded.imm = 0;
    }

    return decoded;
}


