//
// Created by eafc6 on 7/1/2023.
//


#include "../global.h"
#include "ppu.h"

uint8_t OAM[256]; //object attribute memory
uint8_t ppu_mem[16384]; //vram


void ppu_write(uint16_t address, uint8_t operand){
    if(ppu_registers.rw_register_mode){
        ppu_registers.rw_register_mode = 0x0;
        switch (address) {
            case PPUCTRL_ADDR:
                ppu_registers.ppuctrl = operand;
                break;
            case PPUMASK_ADDR:
                ppu_registers.ppumask = operand;
                break;
            case OAMADDR_ADDR:
                ppu_registers.oamaddr = operand;
                break;
            case OAMDATA_ADDR:
                ppu_registers.oamdata = operand;
                break;
            case PPUSCROLL_ADDR:
                ppu_registers.w = !ppu_registers.w;
                ppu_registers.ppuscroll = operand;
                break;
            case PPUADDR_ADDR:
                ppu_registers.w = !ppu_registers.w;
                ppu_registers.ppuaddr = operand;
                break;
            case PPUDATA_ADDR:
                ppu_registers.ppudata = operand;
                break;
            default:
                ppu_registers.oamdma = operand;
                break;
        }
    }
}


uint8_t ppu_read(uint16_t address){
    if(ppu_registers.rw_register_mode){
        ppu_registers.rw_register_mode = 0x0;
        switch (address) {
            case PPUSTATUS_ADDR:
                return ppu_registers.ppustatus;
            case OAMDATA_ADDR:
                return ppu_registers.oamdata;
            default:
                return ppu_registers.ppudata;
        }
    }
    
}

void ppu_power_up(){
    ppu_registers.ppuctrl = 0x0;
    ppu_registers.ppumask = 0x0;
    ppu_registers.ppustatus = 0x0; //TODO investigate correct setting at power up
    ppu_registers.oamaddr = 0x0;
    ppu_registers.ppuscroll = 0x0;
    ppu_registers.ppuaddr = 0x0;
    ppu_registers.ppudata = 0x0;
}

//PPUCTRL
uint16_t get_base_nametable_address(){
    switch (ppu_registers.ppuctrl & 0x3) {
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
    if(is_bit_set(ppu_registers.ppuctrl,3)){
        return 32;
    } else{
        return 1;
    }
}

uint16_t get_sprite_pattern_table_address(){
    if(is_bit_set(ppu_registers.ppuctrl,3)){
        return 0x1000;
    } else{
        return 0x0000;
    }
}

uint16_t get_background_pattern_table_address(){
    if(is_bit_set(ppu_registers.ppuctrl,4)){
        return 0x1000;
    } else{
        return 0x0000;
    }
}

uint8_t get_sprite_size(){
    if(is_bit_set(ppu_registers.ppuctrl,5)){
        return 16;
    } else{
        return 8;
    }
}

uint8_t get_PPU_select(){
    return is_bit_set(ppu_registers.ppuctrl,6);
}

uint8_t get_NMI(){
    return is_bit_set(ppu_registers.ppuctrl,7);
}

//PPUMASK

uint8_t is_greyscale(){
    return is_bit_set(ppu_registers.ppumask,0);
}

uint8_t background_visibility(){
    return is_bit_set(ppu_registers.ppumask,1);
}

uint8_t sprites_visibility(){
    return is_bit_set(ppu_registers.ppumask,2);
}

uint8_t background_shown(){
    return is_bit_set(ppu_registers.ppumask,3);
}

uint8_t sprites_shown(){
    return is_bit_set(ppu_registers.ppumask,4);
}

uint8_t is_red_emphasized(){
    return is_bit_set(ppu_registers.ppumask,5);
}

uint8_t is_green_emphasized(){
    return is_bit_set(ppu_registers.ppumask,6);
}

uint8_t is_blue_emphasized(){
    return is_bit_set(ppu_registers.ppumask,7);
}

//PPUSTATUS

uint8_t PPU_open_bus(){
    return ppu_registers.ppustatus & 0x1F;
}

uint8_t sprite_overflow(){
    return is_bit_set(ppu_registers.ppustatus,5);
}

uint8_t sprite_zero_hit(){
    return is_bit_set(ppu_registers.ppustatus,6);
}

uint8_t is_in_vblank(){
    return is_bit_set(ppu_registers.ppustatus,7);
}