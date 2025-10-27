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




#endif