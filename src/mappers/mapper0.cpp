//
// Created by eafc6 on 8/12/2023.
//

#include <cstring>
#include "mapper0.h"

mapper0::mapper0() {
    uint32_t ROM_SIZE = rom.PRG_ROM_SIZE * 16384;
    uint16_t PRG_RAM_LOC = 16 + (rom.trainer_present * 512);
    rom.rom_file.seekg(PRG_RAM_LOC);
    char *buffer = new char[ROM_SIZE];
    rom.rom_file.read(buffer, ROM_SIZE);

    memcpy(cpu_mem + 0x8000, buffer, 16384);

    if (ROM_SIZE == 16384) {
        memcpy(cpu_mem + 0xC000, buffer, 16384);
    } else if (ROM_SIZE == 32768) {
        memcpy(cpu_mem + 0xC000, buffer + 16384, 16384);
    }


    delete[] buffer;
}
