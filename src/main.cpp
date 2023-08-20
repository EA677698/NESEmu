#include "global.h"
#include "romLoader.h"
#include <iostream>

uint8_t RAM[65536];
uint8_t *internal_mem = RAM;
ROM rom;
REGISTERS registers;

bool is_bit_set(uint8_t operand, char bit){
    return (operand & (0x1 << bit)) >> bit;
}

void power_up(){ //TODO Complete
    registers.sr = 0x34;
    registers.ac = 0,registers.x = 0,registers.y = 0;
    registers.sp = 0xFD;
    internal_mem[0x4017] = 0;
    internal_mem[0x4015] = 0;
    for(int i = 0x4000; i <= 0x400F; i++){
        internal_mem[i] = 0;
    }
    for(int i = 0x4010; i <= 0x4013; i++){
        internal_mem[i] = 0;
    }
    load_rom();
    registers.pc = RESET_VECTOR;
    printf("0xFFFD: 0x%X\n",internal_mem[0xFFFD]);
    printf("0xFFFC: 0x%X\n",internal_mem[0xFFFC]);
    printf("RESET_VECTOR: 0x%X\n",RESET_VECTOR);
}

void reset(){ //TODO Complete
    registers.sp -= 3;
    registers.sr |= 0x4;
}

int main() {
    power_up();
    printf("PC REGISTER: 0x%X\n",registers.pc);
    printf("INITIAL OPCODE: 0x%X\n",internal_mem[registers.pc]);
    printf("ROM HEADER: %s\n",rom.header);
    while (1){
        printf("EXECUTING OPCODE: 0x%X\n",internal_mem[registers.pc]);
        printf("PC REGISTER: 0x%X\n",registers.pc);
        execute_opcode(internal_mem[registers.pc++]);
    }
}
