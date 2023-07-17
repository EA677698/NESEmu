//
// Created by eafc6 on 2/28/2023.
//

#ifndef EMULATOR_GLOBAL_H
#define EMULATOR_GLOBAL_H

#include <cstdlib>
#include <cstdio>

#define ZERO_PAGE_BEGIN 0x0
#define ZERO_PAGE_END 0xFF
#define STACK_START 0x1FF
#define STACK_END 0x100
#define NES_PPU_REGISTER_START 0x2000
#define NES_PPU_REGISTER_END 0x2007
#define NES_APU_IO_REGISTERS_START 0x4000
#define NES_APU_IO_REGISTERS_END 0x4017
#define CARTRIDGE_SPACE_START 0x4020

uint8_t RAM[65536];
uint8_t *internal_mem = RAM;


#endif //EMULATOR_GLOBAL_H
