#ifndef EXEC_H
#define EXEC_H

#include <stdint.h>
#include "decode.h"
#include "instruction.h"
#include "registers.h"

int executeInstruction(decoded_fields decoded, uint8_t *mem );
    
int handleRType(decoded_fields instr);
int handleIType(decoded_fields instr, uint8_t *memory);
int handleSType(decoded_fields instr, uint8_t *memory);
int handleUType(decoded_fields instr);
int handleBType(decoded_fields instr);
int handleJType(decoded_fields instr);


#endif