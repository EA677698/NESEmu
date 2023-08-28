//
// Created by eafc6 on 8/19/2023.
//

#ifndef EMULATOR_CPU_H
#define EMULATOR_CPU_H
#include <cstdint>

#define RESET_VECTOR ((((uint16_t) internal_mem[0xFFFD]) << 8) | internal_mem[0xFFFC])


struct REGISTERS {
    uint8_t ac; //accumulator
    uint8_t x; //x index
    uint8_t y; //y  index
    uint16_t pc; //program counter
    uint8_t sp; //stack pointer
    uint8_t sr; //sr register (flags)
    uint32_t cycles;
};

extern REGISTERS registers;

void execute_opcode(int opcode);



#endif //EMULATOR_CPU_H
