//
// Created by eafc6 on 8/19/2023.
//

#ifndef EMULATOR_ROMLOADER_H
#define EMULATOR_ROMLOADER_H

#include <iostream>
#include "global.h"
#define URL "../Super_mario_brothers.nes" //TEMP

bool is_iNES_1_format();

void load_flag_6();

void load_flag_7();

void load_flags();

void load_rom_fd();

void load_rom();


#endif //EMULATOR_ROMLOADER_H
