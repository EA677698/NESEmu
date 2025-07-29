//
// Created by eafc6 on 8/12/2023.
//

#include <cstring>
#include "mapper0.h"

mapper0::mapper0(CPU *cpu, PPU *ppu) {
    // CPU RAM
    uint32_t ROM_SIZE = rom.PRG_ROM_SIZE * 16384;
    uint32_t CHR_SIZE = rom.CHR_ROM_SIZE * 8192;  // CHR ROM in bytes
    uint16_t PRG_RAM_LOC = 16 + (rom.trainer_present * 512);
    rom.rom_file.seekg(PRG_RAM_LOC);
    char *buffer = new char[ROM_SIZE];
    rom.rom_file.read(buffer, ROM_SIZE);

    memcpy(cpu->mem + 0x8000, buffer, 16384);

    if (ROM_SIZE == 16384) {
        memcpy(cpu->mem + 0xC000, buffer, 16384);
    } else if (ROM_SIZE == 32768) {
        memcpy(cpu->mem + 0xC000, buffer + 16384, 16384);
    }


    delete[] buffer;

    // PPU VRAM
    if (CHR_SIZE > 0) {
        uint32_t CHR_START = PRG_RAM_LOC + ROM_SIZE;
        rom.rom_file.seekg(CHR_START);
        char* chr_buffer = new char[CHR_SIZE];
        rom.rom_file.read(chr_buffer, CHR_SIZE);

        memcpy(ppu->ppu_mem, chr_buffer, CHR_SIZE);

        delete[] chr_buffer;

        // spdlog::info("Loaded CHR ROM ({} bytes) into PPU memory", CHR_SIZE);
    } else {
        spdlog::warn("CHR ROM size is 0! Expecting CHR RAM writes from game");
    }

    // nametable mirroring
    if (rom.nametable_mirroring) {
        // horizontal
        memcpy(ppu->ppu_mem + NAMETABLE_1, ppu->ppu_mem + NAMETABLE_0, 0x400);
        memcpy(ppu->ppu_mem + NAMETABLE_3, ppu->ppu_mem + NAMETABLE_2, 0x400);
    } else {
        // vertical
        memcpy(ppu->ppu_mem + NAMETABLE_2, ppu->ppu_mem + NAMETABLE_0, 0x400);
        memcpy(ppu->ppu_mem + NAMETABLE_3, ppu->ppu_mem + NAMETABLE_1, 0x400);
    }

}
