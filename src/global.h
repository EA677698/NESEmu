//
// Created by eafc6 on 2/28/2023.
//

#ifndef EMULATOR_GLOBAL_H
#define EMULATOR_GLOBAL_H

#include <cstdio>
#include <fstream>
#include "CPU/cpu.h"

#define ZERO_PAGE_BEGIN 0x0
#define ZERO_PAGE_END 0xFF
#define STACK_START 0x1FF
#define STACK_END 0x100
#define NES_PPU_REGISTER_START 0x2000
#define NES_PPU_REGISTER_END 0x2007
#define NES_APU_IO_REGISTERS_START 0x4000
#define NES_APU_IO_REGISTERS_END 0x4017
#define CARTRIDGE_SPACE_START 0x4020

extern uint8_t RAM[65536];
extern uint8_t *internal_mem;

struct ROM{
    std::ifstream rom_file;
    uint8_t header[16];
    uint16_t PRG_ROM_SIZE;
    uint16_t CHR_ROM_SIZE;
    bool nametable_mirroring;
    bool persistent_memory;
    bool trainer_present;
    bool ignore_mirroring_control;
    uint8_t lower_nybble;
    uint8_t upper_nybble;
    uint16_t mode;
    uint16_t unisystem;
    uint8_t PRG_RAM_SIZE;
};

extern ROM rom;

extern bool is_bit_set(uint8_t operand, char bit);

extern void exit();



#endif //EMULATOR_GLOBAL_H
