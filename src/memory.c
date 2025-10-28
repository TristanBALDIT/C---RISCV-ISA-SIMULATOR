#include "../include/memory.h"


uint32_t loadB(Memory *mem, uint32_t addr){
    // Load 8 bits (signed), then sign-extend to 32 bits (implicitly)
    return (int8_t)mem->data[addr];
}

uint32_t loadHW(Memory *mem, uint32_t addr){
    // Load 16 bits (signed), then sign-extend to 32 bits (implicitly)
    return (int16_t)(mem->data[addr] | (mem->data[addr + 1] << 8));
}

uint32_t loadW(Memory *mem, uint32_t addr) {
    return mem->data[addr] |
           (mem->data[addr + 1] << 8) |
           (mem->data[addr + 2] << 16) |
           (mem->data[addr + 3] << 24);
}

uint32_t loadBU(Memory *mem, uint32_t addr){
     // Load 8 bits and zero-extend to 32 bits (implicitly)
     return mem->data[addr];
}

uint32_t loadHWU(Memory *mem, uint32_t addr){
    // Load 2 bytes and zero-extend to 32 bits (implicitly)
    return mem->data[addr] | (mem->data[addr + 1] << 8);
}

void storeByte(uint8_t *memory, uint32_t addr, uint8_t value) {
    memory[addr] = value;
}

void storeHalfword(uint8_t *memory, uint32_t addr, uint16_t value) {
    memory[addr]     = value & 0xFF;
    memory[addr + 1] = (value >> 8) & 0xFF;
}

void storeWord(uint8_t *memory, uint32_t addr, uint32_t value) {
    memory[addr]     = value & 0xFF;
    memory[addr + 1] = (value >> 8) & 0xFF;
    memory[addr + 2] = (value >> 16) & 0xFF;
    memory[addr + 3] = (value >> 24) & 0xFF;
}