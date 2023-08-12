#include "cpu.cpp"
#include "romLoader.cpp"


void power_up(){ //TODO Complete
    registers.sr = 0x34;
    registers.ac,registers.x,registers.y = 0;
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
}

void reset(){ //TODO Complete
    registers.sp -= 3;
    registers.sr |= 0x4;
}

int main() {
}
