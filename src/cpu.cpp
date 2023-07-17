//
// Created by eafc6 on 2/28/2023.
//


#include <cstdint>
#include "global.h"

struct {
    uint8_t ac; //accumulator
    uint8_t x; //x index
    uint8_t y; //y  index
    uint16_t pc; //program counter
    uint8_t sp; //stack pointer
    uint8_t sr; //sr register (flags)
} registers;

