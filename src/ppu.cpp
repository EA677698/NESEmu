//
// Created by eafc6 on 7/1/2023.
//


#include <cstdint>
#include "global.h"
#include "ppu.h"

void io_db_write(uint16_t ppu_register, uint8_t operand){
    if(ppu_register >= 0x2008 && ppu_register <= 0x3FFF) {
        ppu_register = 0x2000 + (ppu_register % 8);
    }
    internal_mem[ppu_register] = operand;
}

uint8_t io_db_read(uint16_t ppu_register){
    if(ppu_register >= 0x2008 && ppu_register <= 0x3FFF) {
        ppu_register = 0x2000 + (ppu_register % 8);
    }
    return internal_mem[ppu_register];
}

void PPU_INIT(){
    PPUCTRL = 0x0;
    PPUMASK = 0x0;
    PPUSTATUS = 0x0; //TODO investigate correct setting at power up
    OAMADDR = 0x0;
    PPUSCROLL = 0x0;
    PPUADDR = 0x0;
    PPUDATA = 0x0;
}


// CONTROLLER
bool is_NMI_enabled(){
    return is_bit_set(PPUCTRL,7);
}

uint8_t sprite_height(){
    if(is_bit_set(PPUCTRL,5)){
        return 16;
    } else{
        return 8;
    }
}

uint16_t background_pattern_table_address(){
    if(is_bit_set(PPUCTRL,4)){
        return 0x1000;
    } else{
        return 0x0000;
    }
}

uint16_t sprite_pattern_table_address(){
    if(is_bit_set(PPUCTRL,3)){
        return 0x1000;
    } else{
        return 0x0000;
    }
}

uint8_t VRAM_address_increment(){
    if(is_bit_set(PPUCTRL,2)){
        return 32;
    } else{
        return 1;
    }
}

uint16_t base_nametable_address(){
    uint8_t mode = PPUCTRL & 0x3;
    switch (mode) { // NOLINT(hicpp-multiway-paths-covered)
        case 0: return 0x2000;
        case 1: return 0x2400;
        case 2: return 0x2800;
        case 3: return 0x2C00;
    }
}


// MASK

bool greyscale_mode(){
    return is_bit_set(PPUMASK,0);
}

bool background_leftmost_columns(){
    return is_bit_set(PPUMASK,1);
}

bool sprites_leftmost_columns(){
    return is_bit_set(PPUMASK,2);
}

bool render_background(){
    return is_bit_set(PPUMASK,3);
}

bool render_sprites(){
    return is_bit_set(PPUMASK,4);
}

bool emphasize_red(){
    return is_bit_set(PPUMASK,5);
}

bool emphasize_green(){
    return is_bit_set(PPUMASK,6);
}

bool emphasize_blue(){
    return is_bit_set(PPUMASK,7);
}


// STATUS
