//
// Created by eafc6 on 8/12/2023.
//

#ifndef EMULATOR_MAPPER0_H
#define EMULATOR_MAPPER0_H

#include "mapper.h"
#include "../global.h"
#include "../CPU/cpu.h"

class mapper0 : public mapper{

public:

    mapper0(CPU *cpu);
};


#endif //EMULATOR_MAPPER0_H
