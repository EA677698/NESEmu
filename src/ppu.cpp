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


// MASK



// STATUS
