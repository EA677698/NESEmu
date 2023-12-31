//
// Created by eafc6 on 7/1/2023.
//


#include <cstdint>
#include "global.h"
#include "ppu.h"

void PPU_INIT(){
    PPUCTRL = 0x0;
    PPUMASK = 0x0;
    PPUSTATUS = 0x0; //TODO investigate correct setting at power up
    OAMADDR = 0x0;
    PPUSCROLL = 0x0;
    PPUADDR = 0x0;
    PPUDATA = 0x0;
}


//PPUCTRL
uint16_t get_base_nametable_address(){
    switch (PPUCTRL & 0x3) {
        case 0x1:
            return 0x2400;
        case 0x2:
            return 0x2800;
        case 0x3:
            return 0x2C00;
        default:
            return 0x2000;
    }
}

uint8_t get_vram_address_increment(){
    if(is_bit_set(PPUCTRL,3)){
        return 32;
    } else{
        return 1;
    }
}

uint16_t get_sprite_pattern_table_address(){
    if(is_bit_set(PPUCTRL,3)){
        return 0x1000;
    } else{
        return 0x0000;
    }
}

uint16_t get_background_pattern_table_address(){
    if(is_bit_set(PPUCTRL,4)){
        return 0x1000;
    } else{
        return 0x0000;
    }
}

uint8_t get_sprite_size(){
    if(is_bit_set(PPUCTRL,5)){
        return 16;
    } else{
        return 8;
    }
}

uint8_t get_PPU_select(){
    return is_bit_set(PPUCTRL,6);
}

uint8_t get_NMI(){
    return is_bit_set(PPUCTRL,7);
}

//PPUMASK

uint8_t is_greyscale(){
    return is_bit_set(PPUMASK,0);
}

uint8_t background_visibility(){
    return is_bit_set(PPUMASK,1);
}

uint8_t sprites_visibility(){
    return is_bit_set(PPUMASK,2);
}

uint8_t background_shown(){
    return is_bit_set(PPUMASK,3);
}

uint8_t sprites_shown(){
    return is_bit_set(PPUMASK,4);
}

uint8_t is_red_emphasized(){
    return is_bit_set(PPUMASK,5);
}

uint8_t is_green_emphasized(){
    return is_bit_set(PPUMASK,6);
}

uint8_t is_blue_emphasized(){
    return is_bit_set(PPUMASK,7);
}

//PPUSTATUS

uint8_t PPU_open_bus(){
    return PPUSTATUS & 0x1F;
}

uint8_t sprite_overflow(){
    return is_bit_set(PPUSTATUS,5);
}

uint8_t sprite_zero_hit(){
    return is_bit_set(PPUSTATUS,6);
}

uint8_t is_in_vblank(){
    return is_bit_set(PPUSTATUS,7);
}
