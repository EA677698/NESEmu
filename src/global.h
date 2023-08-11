//
// Created by eafc6 on 2/28/2023.
//

#ifndef EMULATOR_GLOBAL_H
#define EMULATOR_GLOBAL_H

#include <cstdio>
#include <fstream>

#define ZERO_PAGE_BEGIN 0x0
#define ZERO_PAGE_END 0xFF
#define STACK_START 0x1FF
#define STACK_END 0x100
#define NES_PPU_REGISTER_START 0x2000
#define NES_PPU_REGISTER_END 0x2007
#define NES_APU_IO_REGISTERS_START 0x4000
#define NES_APU_IO_REGISTERS_END 0x4017
#define CARTRIDGE_SPACE_START 0x4020



#define READ 0x1
#define WRITE 0x2

uint8_t RAM[65536];
uint8_t *internal_mem = RAM;

struct{
    std::ifstream rom_file;
    char header[16];
    int PRG_ROM_SIZE;
    int CHR_ROM_SIZE;
    bool nametable_mirroring;
    bool persistent_memory;
    bool trainer_present;
    bool ignore_mirroring_control;
    int lower_nybble;
    int upper_nybble;
    int mode;
    int unisystem;
    int PRG_RAM_SIZE;
    bool region;



} rom;



bool is_bit_set(uint8_t operand, char bit){
    return (operand & (0x1 << bit)) >> bit;
}


#endif //EMULATOR_GLOBAL_H
