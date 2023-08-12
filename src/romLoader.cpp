//
// Created by eafc6 on 8/10/2023.
//


#include "global.h"
#define URL "" //TEMP

bool is_iNES_format(){
    if(rom.header[0] != 0x4E){
        return false;
    }
    if(rom.header[1] != 0x45){
        return false;
    }
    if(rom.header[2] != 0x53){
        return false;
    }
    if(rom.header[3] != 0x1A){
        return false;
    }
    return true;
}

void load_flag_6(){
    uint8_t* byte = &rom.header[6];
    rom.nametable_mirroring = is_bit_set(*byte,0);
    rom.persistent_memory = is_bit_set(*byte,1);
    rom.trainer_present = is_bit_set(*byte,2);
    rom.ignore_mirroring_control = is_bit_set(*byte,3);
    rom.lower_nybble = *byte >> 4;

}

void load_flag_7(){
    uint8_t* byte = &rom.header[7];
    rom.unisystem = is_bit_set(*byte,0);
    rom.upper_nybble = *byte >> 4;
}


void load_flags(){
    rom.PRG_ROM_SIZE = rom.header[4];
    rom.CHR_ROM_SIZE = rom.header[5];
    load_flag_6();
    load_flag_7();
    rom.PRG_RAM_SIZE = rom.header[8];
}

void load_rom_fd(){
    rom.rom_file.open(URL, std::ifstream::in | std::ifstream::binary);
    if(!rom.rom_file.is_open()){
        //TODO throw error
    }
    char buff[16];
    rom.rom_file.read(buff,16);
    memcpy(rom.header,buff,16);
}

void load_rom(){
    load_rom_fd();
    if(!is_iNES_format()){
        //TODO throw error
        return;
    }
    load_flags();
    internal_mem[0xFFFC] = rom.lower_nybble;
    internal_mem[0xFFFD] = rom.upper_nybble;
}
