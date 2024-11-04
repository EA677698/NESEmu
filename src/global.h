//
// Created by eafc6 on 2/28/2023.
//

#ifndef EMULATOR_GLOBAL_H
#define EMULATOR_GLOBAL_H

#include <fstream>
#include <cstdint>

#define VIDEO_WIDTH 256
#define VIDEO_HEIGHT 240

struct RGB{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

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
