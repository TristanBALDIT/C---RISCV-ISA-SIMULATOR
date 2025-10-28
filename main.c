//
// Created by trist on 27/10/2025.
//
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/decode.h"
#include "include/registers.h"
#include "include/execute.h"
#include "include/memory.h"


// Register and Program Counter setup
uint32_t regs[NUM_REGS] = {0};
uint32_t PC = MEM_BASE;



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
        free(mem.data);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (fsize < 0 || fsize > mem.size) {
        fprintf(stderr, "File too big\n");
        fclose(file);
        free(mem.data);  
        return 1;
    }

    fread(&mem.data[0], 1, fsize, file);
    fclose(file);

    printf("Loaded %ld bytes into memory\n", fsize);

    while (PC < fsize) {
        uint32_t instr = loadW(&mem, PC);                 
        decoded_fields decoded = decodeInstruction(instr); 
        int status = executeInstruction(decoded, &mem); 

        if (status == 1) {
            printf("Program halted by ECALL\n");
            break;
        }

        // Advance PC unless modified by branch/jump
        if (decoded.instrType != B_TYPE && decoded.instrType != J_TYPE)
            PC += 4;
    }
    //Have some logic to flush registers to a file...

    free(mem.data);  
    return 0;
}