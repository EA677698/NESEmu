//
// Created by eafc6 on 7/21/2023.
//

#ifndef EMULATOR_PPU_H
#define EMULATOR_PPU_H

#include <cstdint>

#define PPUCTRL cpu_mem[0x2000]
#define PPUMASK cpu_mem[0x2001]
#define PPUSTATUS cpu_mem[0x2002]
#define OAMADDR cpu_mem[0x2003]
#define OAMDATA internal_mem[0x2004]
#define PPUSCROLL cpu_mem[0x2005]
#define PPUADDR cpu_mem[0x2006]
#define PPUDATA cpu_mem[0x2007]
#define OAMDMA internal_mem[0x4014]

extern uint8_t OAM[];
extern uint8_t ppu_mem[];

void ppu_write(uint16_t address, uint8_t operand);
void ppu_read(uint16_t address);

void ppu_power_up();

#endif //EMULATOR_PPU_H
