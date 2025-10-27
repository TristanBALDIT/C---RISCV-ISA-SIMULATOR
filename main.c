//
// Created by trist on 27/10/2025.
//
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "decoder.h"

#define MEM_SIZE (1024 * 1024)
#define MEM_BASE  0x00000000

typedef struct {
    uint8_t *data;
    uint32_t size;
} Memory;

uint32_t loadW(Memory *mem, uint32_t addr) {
    return mem->data[addr] |
           (mem->data[addr + 1] << 8) |
           (mem->data[addr + 2] << 16) |
           (mem->data[addr + 3] << 24);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <binary_file>\n", argv[0]);
        return 1;
    }

    Memory mem;
    mem.size = MEM_SIZE;
    mem.data = (uint8_t *)calloc(mem.size, sizeof(uint8_t));
    if (!mem.data) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        perror("Failed to open file");
        free(mem.data);  // <-- free here to avoid leak
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (fsize < 0 || fsize > mem.size) {
        fprintf(stderr, "File too big\n");
        fclose(file);
        free(mem.data);  // <-- free here
        return 1;
    }

    fread(&mem.data[0], 1, fsize, file);
    fclose(file);

    printf("Loaded %ld bytes into memory\n", fsize);

    // --- CPU setup ---
    uint32_t PC = MEM_BASE;
    uint32_t registers[32];

    Instruction i = decode_riscv(loadW(&mem, PC));
    printf("Instruction %zu:\n", i);
    printf("  opcode : 0x%02X\n", i.opcode);
    printf("  rd     : %u\n", i.rd);
    printf("  funct3 : %u\n", i.funct3);
    printf("  rs1    : %u\n", i.rs1);
    printf("  rs2    : %u\n", i.rs2);
    printf("  funct7 : 0x%02X\n", i.funct7);
    printf("  imm    : %d (0x%X)\n\n", i.imm, i.imm);

    free(mem.data);  // <-- free at the end to avoid leak
    return 0;
}

