//
// Created by eafc6 on 8/12/2023.
//

#include "mapper0.h"

mapper0::mapper0() {
    uint16_t PRG_RAM_LOC = 16 + (rom.trainer_present * 512);
    rom.rom_file.seekg(PRG_RAM_LOC);
    char buffer[16384];
    rom.rom_file.read(buffer,16384);
    memcpy(internal_mem + 0x6000, buffer,16384);
    if((rom.PRG_ROM_SIZE * 16384) > 16384){
        memset(buffer,0,16384);
        rom.rom_file.read(buffer,16384);
        memcpy(internal_mem + 0xC000, buffer, 16384);
    } else{
        memcpy(internal_mem + 0xC000, buffer, 16384);
    }

}
