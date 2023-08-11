//
// Created by eafc6 on 8/10/2023.
//


#include "global.h"
#define URL "" //TEMP

bool is_iNES_format(){
    if(rom.header[0] != 0x4E){
        return 0;
    }
    if(rom.header[1] != 0x45){
        return 0;
    }
    if(rom.header[2] != 0x53){
        return 0;
    }
    if(rom.header[3] != 0x1A){
        return 0;
    }
    return 1;
}

void load_flags(){

}

void load_rom_fd(){
    rom.rom_file.open(URL, std::ifstream::in | std::ifstream::binary);
    if(!rom.rom_file.is_open()){
        //TODO throw error
    }
    rom.rom_file.read(rom.header,16);
}
