#include "../include/registers.h"

void dumpRegisterContents(){
    for (int i = 0; i < NUM_REGS; i++) {
        printf("x%d (%s): 0x%08X\n", i, regName((reg_t)i), regs[i]);
    }
    return;
}

// Returns new filename with <basename>-answer.res
char *makeDumpFilename(const char *input) {
    const char *dot = strrchr(input, '.'); // Strip ext
    size_t len;

    if (dot != NULL){ // If ext exists get the length from start to .
        len = (size_t)(dot - input); 
    }else{
        len = strlen(input); // else just get the length of the whole filename
    } 

    const char *suffix = "-answer.res";

    // Total length = prefix + base + suffix + null terminator
    size_t totalLen = + len + strlen(suffix) + 1;

    char *output = malloc(totalLen);
    if (output == NULL){
        return NULL;
    } 

    strncpy(output, input, len);
    output[len] = '\0'; // terminate the str
    strcat(output, suffix); // put the new name + ext


    return output;
}

int dumpRegisterContentsFile(const char *filename) {
    char *dumpFilename = makeDumpFilename(filename);
    if (!dumpFilename) {
        fprintf(stderr, "Failed to allocate dump filename\n");
        return 1;
    }
    
    FILE *file = fopen(dumpFilename, "wb");
    if (!file) {
        perror("Failed to open file");
        return -1;
    }

    size_t written = fwrite(regs, sizeof(uint32_t), NUM_REGS, file);
    fclose(file);

    if (written != NUM_REGS) {
        fprintf(stderr, "Incomplete dump, only wrote %zu of %d\n", written, NUM_REGS);
        free(dumpFilename);
        return -1;
    }

    free(dumpFilename);
    return 0;
}