//
// Created by eafc6 on 7/23/2023.
//


#include <cstdint>
#include "global.h"
#include "ppu.h"

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