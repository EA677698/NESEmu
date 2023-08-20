//
// Created by eafc6 on 2/28/2023.
//


#include <cstdint>
#include "../global.h"

#define RESET_VECTOR ((((uint16_t) internal_mem[0xFFFD]) << 8) | internal_mem[0xFFFC])

struct {
    uint8_t ac; //accumulator
    uint8_t x; //x index
    uint8_t y; //y  index
    uint16_t pc; //program counter
    uint8_t sp; //stack pointer
    uint8_t sr; //sr register (flags)
    uint8_t opcode; //fetched opcode to execute
} registers;

