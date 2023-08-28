//
// Created by eafc6 on 7/21/2023.
//

#ifndef EMULATOR_PPU_H
#define EMULATOR_PPU_H


#define PPUCTRL internal_mem[0x2000]
#define PPUMASK internal_mem[0x2001]
#define PPUSTATUS internal_mem[0x2002]
#define OAMADDR internal_mem[0x2003]
#define OAMDATA internal_mem[0x2004]
#define PPUSCROLL internal_mem[0x2005]
#define PPUADDR internal_mem[0x2006]
#define PPUDATA internal_mem[0x2007]
#define OAMDMA internal_mem[0x4014]

#define PPUCTRL_ADDR 0x2000
#define PPUMASK_ADDR 0x2001
#define PPUSTATUS_ADDR 0x2002
#define OAMADDR_ADDR 0x2003
#define OAMDATA_ADDR 0x2004
#define PPUSCROLL_ADDR 0x2005
#define PPUADDR_ADDR 0x2006
#define PPUDATA_ADDR 0x2007
#define OAMDMA_ADDR 0x4014


#endif //EMULATOR_PPU_H
