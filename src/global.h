//
// Created by eafc6 on 2/28/2023.
//

#ifndef EMULATOR_GLOBAL_H
#define EMULATOR_GLOBAL_H

#include <fstream>
#include <cstdint>

#define VIDEO_WIDTH 256
#define VIDEO_HEIGHT 240

struct RGBA{
    uint8_t b = 0;
    uint8_t g = 0;
    uint8_t r = 0;
    uint8_t a = 0xFF;
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

struct Arguments {
    std::string rom_path;
    std::string test_type;
    bool debug_mode;
    bool dump_memory;
    bool instruction_cycles;
    bool disable_ppu;
    int breakpoint;
};

extern Arguments args;

extern ROM rom;

extern bool is_bit_set(uint8_t operand, char bit);

extern void emulator_exit(int status);



#endif //EMULATOR_GLOBAL_H
