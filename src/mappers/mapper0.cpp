//
// Created by eafc6 on 8/12/2023.
//

#include "mapper0.h"

mapper0::mapper0() {
    uint32_t ROM_SIZE = rom.PRG_ROM_SIZE * 16384;
    uint16_t PRG_RAM_LOC = 16 + (rom.trainer_present * 512);
    rom.rom_file.seekg(PRG_RAM_LOC);
    char *buffer = new char[ROM_SIZE];
    rom.rom_file.read(buffer, ROM_SIZE);

    memcpy(internal_mem + 0x8000, buffer, 16384);

    if (ROM_SIZE == 16384) {
        memcpy(internal_mem + 0xC000, buffer, 16384);
    } else if (ROM_SIZE == 32768) {
        memcpy(internal_mem + 0xC000, buffer + 16384, 16384);
    }

    // NMI VECTOR
    internal_mem[0xFFFA] = buffer[ROM_SIZE - 6];
    internal_mem[0xFFFB] = buffer[ROM_SIZE - 5];

    // RESET VECTOR
    internal_mem[0xFFFC] = buffer[ROM_SIZE - 4];
    internal_mem[0xFFFD] = buffer[ROM_SIZE - 3];

    // IRQ/BRK VECTOR
    internal_mem[0xFFFE] = buffer[ROM_SIZE - 2];
    internal_mem[0xFFFF] = buffer[ROM_SIZE - 1];

    delete[] buffer;
}
