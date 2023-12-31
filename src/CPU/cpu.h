//
// Created by eafc6 on 8/19/2023.
//

#ifndef EMULATOR_CPU_H
#define EMULATOR_CPU_H
#include <cstdint>
#include "spdlog/spdlog.h"

#define RESET_VECTOR ((((uint16_t) cpu_mem[0xFFFD]) << 8) | cpu_mem[0xFFFC])

#define PPUCTRL_ADDR 0x2000
#define PPUMASK_ADDR 0x2001
#define PPUSTATUS_ADDR 0x2002
#define OAMADDR_ADDR 0x2003
#define OAMDATA_ADDR 0x2004
#define PPUSCROLL_ADDR 0x2005
#define PPUADDR_ADDR 0x2006
#define PPUDATA_ADDR 0x2007
#define OAMDMA_ADDR 0x4014

#define ZERO_PAGE_BEGIN 0x0
#define ZERO_PAGE_END 0xFF
#define STACK_START 0x1FF
#define STACK_END 0x100
#define NES_PPU_REGISTER_START 0x2000
#define NES_PPU_REGISTER_END 0x2007
#define NES_APU_IO_REGISTERS_START 0x4000
#define NES_APU_IO_REGISTERS_END 0x4017
#define CARTRIDGE_SPACE_START 0x4020
#define NES_PPU_REGISTER_MIRRORS_START 0x2008
#define NES_PPU_REGISTER_MIRRORS_END 0x3FFF

#define READ 0x1
#define WRITE 0x2

extern uint8_t cpu_mem[];

struct REGISTERS {
    uint8_t ac; //accumulator
    uint8_t x; //x index
    uint8_t y; //y  index
    uint16_t pc; //program counter
    uint8_t sp; //stack pointer
    uint8_t sr; //sr register (flags)
    // NOT ACTUAL REGISTERS VVVVVVV
    uint32_t cycles;
    uint8_t rw_register_mode; // read/write register mode to 16 bit addresses -- horrible hack, but it'll do
};

extern REGISTERS registers;

const uint8_t CPU_PPU_PERM[] = {WRITE,WRITE,READ,WRITE,READ | WRITE, WRITE * WRITE, WRITE * WRITE, READ | WRITE, WRITE};

void execute_opcode(int opcode);

void cpu_write(uint16_t address, uint8_t operand);

uint8_t cpu_read(uint16_t address);

extern void cpu_power_up();

#endif //EMULATOR_CPU_H
