//
// Created by eafc6 on 2/28/2023.
//


#include <cstdlib>

struct {
    u_int8_t a; //acumulator
    u_int8_t x; //x index
    u_int8_t y; //y  index
    u_int16_t pc; //program counter
    u_int8_t sp; //stack pointer
} registers;

struct {
        u_int8_t C: 1; //carry
        u_int8_t Z: 1; //zero
        u_int8_t I: 1; //interrupt disable
        u_int8_t D: 1; //decimal
        u_int8_t V: 1; //overflow
        u_int8_t N: 1; //negative
        u_int8_t B: 1; //b flag
    } flags;

typedef u_int16_t opcodes;

opcodes x86[] = {

};

opcodes r6502[] {

};