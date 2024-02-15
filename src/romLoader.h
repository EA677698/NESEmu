//
// Created by eafc6 on 8/19/2023.
//

#ifndef EMULATOR_ROMLOADER_H
#define EMULATOR_ROMLOADER_H

#include "CPU/cpu.h"


bool is_iNES_format();

void load_flag_6();

void load_flag_7();

void load_flags();

void load_rom_fd(std::string rom_path);

void load_rom(CPU *cpu, std::string rom_path);


#endif //EMULATOR_ROMLOADER_H
