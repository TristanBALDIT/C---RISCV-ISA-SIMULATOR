#include "../include/memory.h"

uint32_t loadW(Memory *mem, uint32_t addr) {
    return mem->data[addr] |
           (mem->data[addr + 1] << 8) |
           (mem->data[addr + 2] << 16) |
           (mem->data[addr + 3] << 24);
}