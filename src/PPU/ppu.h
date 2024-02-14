//
// Created by eafc6 on 7/21/2023.
//

#ifndef EMULATOR_PPU_H
#define EMULATOR_PPU_H

#include <cstdint>

#define PATTERN_TABLE_0 0x0000
#define PATTERN_TABLE_1 0x1000
#define NAMETABLE_0 0x2000
#define NAMETABLE_1 0x2400
#define NAMETABLE_2 0x2800
#define NAMETABLE_3 0x2C00
#define NAMETABLE_MIRRORS 0x3000
#define PALETTE_RAM 0x3F00
#define PALETTE_RAM_MIRRORS 0x4000

class PPU {

    uint8_t OAM[256]; // object attribute memory (OAM)
    uint8_t ppu_mem[16384]; // VRAM
    uint8_t ppudata_buffer; // For accuracy, buffer prefetches for next PPUDATA read


public:

    PPU();

    struct {
        uint8_t ppuctrl;
        uint8_t ppumask;
        uint8_t ppustatus;
        uint8_t oamaddr;
        uint8_t oamdata;
        uint8_t ppuscroll;
        uint16_t ppuaddr;
        uint8_t ppudata;
        uint8_t oamdma;
        // Internal registers VVVVVV
        uint16_t v; //current vram address
        uint16_t t; //temporary vram address
        uint8_t x; //fine x scroll
        uint8_t w; //write toggle
    } registers;


    void write(uint16_t address, uint8_t operand);
    uint8_t read(uint16_t address);

    void ppu_power_up();
    uint16_t get_base_nametable_address();
    uint8_t get_vram_address_increment();
    uint16_t get_sprite_pattern_table_address();
    uint16_t get_background_pattern_table_address();
    uint8_t get_sprite_size();
    uint8_t get_PPU_select();
    uint8_t get_NMI();
    uint8_t is_greyscale();
    uint8_t background_visibility();
    uint8_t sprites_visibility();
    uint8_t background_shown();
    uint8_t sprites_shown();
    uint8_t is_red_emphasized();
    uint8_t is_green_emphasized();
    uint8_t is_blue_emphasized();
    uint8_t PPU_open_bus();
    uint8_t sprite_overflow();
    uint8_t sprite_zero_hit();
    uint8_t is_in_vblank();

};

#endif //EMULATOR_PPU_H
