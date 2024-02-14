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

extern uint8_t OAM[];
extern uint8_t ppu_mem[];

struct {
    uint8_t ppuctrl;
    uint8_t ppumask;
    uint8_t ppustatus;
    uint8_t oamaddr;
    uint8_t oamdata;
    uint8_t ppuscroll;
    uint8_t ppuaddr;
    uint8_t ppudata;
    uint8_t oamdma;
    // Internal registers VVVVVV
    uint16_t v; //current vram address
    uint16_t t; //temporary vram address
    uint8_t x; //fine x scroll
    uint8_t w; //write toggle
    uint8_t rw_register_mode;
} ppu_registers;

void ppu_write(uint16_t address, uint8_t operand);
uint8_t ppu_read(uint16_t address);

void ppu_power_up();

#endif //EMULATOR_PPU_H
