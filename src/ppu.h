//
// Created by eafc6 on 7/21/2023.
//

#ifndef EMULATOR_PPU_H
#define EMULATOR_PPU_H


#define PPUCTRL internal_mem[0x2000]
#define PPUMASK internal_mem[0x2001]
#define PPUSTATUS internal_mem[0x2002]
#define OAMADDR internal_mem[0x2003]
#define OAMDATA internal_mem[0x2004]
#define PPUSCROLL internal_mem[0x2005]
#define PPUADDR internal_mem[0x2006]
#define PPUDATA internal_mem[0x2007]
#define OAMDMA internal_mem[0x4014]

const uint8_t cpu_access[] = {WRITE,WRITE,READ,WRITE,READ | WRITE, WRITE * WRITE, WRITE * WRITE, READ | WRITE, WRITE};

void io_db_write(uint16_t ppu_register);

uint8_t io_db_read(uint16_t ppu_register);

uint8_t VRAM_address_increment();

uint16_t base_nametable_address();

void PPU_INIT();

bool is_NMI_enabled();

uint8_t sprite_height();

uint16_t background_pattern_table_address();

uint16_t sprite_pattern_table_address();

bool greyscale_mode();

bool background_leftmost_columns();

bool sprites_leftmost_columns();

bool render_background();

bool render_sprites();

bool emphasize_red();

bool emphasize_green();

bool emphasize_blue();

#endif //EMULATOR_PPU_H
