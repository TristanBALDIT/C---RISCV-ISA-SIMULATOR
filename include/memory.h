#ifndef MEM_H
#define MEM_H

#include <stdint.h>

#define MEM_SIZE (1024 * 1024)
#define MEM_BASE  0x00000000

typedef struct {
    uint8_t *data;
    uint32_t size;
} Memory;

uint32_t loadW(Memory *mem, uint32_t addr);
void storeByte(uint8_t *memory, uint32_t addr, uint8_t value);
void storeHalfword(uint8_t *memory, uint32_t addr, uint16_t value);
void storeWord(uint8_t *memory, uint32_t addr, uint32_t value);
   


#endif