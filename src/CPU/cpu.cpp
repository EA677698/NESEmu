#include "cpu.h"
#include "../global.h"

void cpu_write(uint16_t address, uint8_t operand){
    if(registers.rw_register_mode){
        switch (address) {
            case 0x0000:
                registers.ac = operand;
                break;
            case 0x0001:
                registers.x = operand;
                break;
            case 0x0002:
                registers.y = operand;
                break;
            case 0x0003:
                registers.pc = operand;
                break;
            case 0x0004:
                registers.sp = operand;
                break;
            case 0x0005:
                registers.sr = operand;
                break;
            default:
                spdlog::error("Invalid CPU write to illegal register: 0x{:X}", address);
                exit(1);
        }
        return;
    }
    if(address >= 0x2000 && address <= 0x3FFF){
        address = 0x2000 + (address % 8);
        if(CPU_PPU_PERM[address % 8] > READ){
            internal_mem[address] = operand;
        } else{
            spdlog::error("Invalid CPU write to PPU: 0x{:X}", address);
        }
    } else{
        internal_mem[address] = operand;
    }
}

uint8_t cpu_read(uint16_t address){
    if(registers.rw_register_mode){
        switch (address) {
            case 0x0000:
                return registers.ac;
            case 0x0001:
                return registers.x;
            case 0x0002:
                return registers.y;
            case 0x0003:
                return registers.pc;
            case 0x0004:
                return registers.sp;
            case 0x0005:
                return registers.sr;
            default:
                spdlog::error("Invalid CPU read to illegal register: 0x{:X}", address);
                exit(1);
        }
    }
    if(address >= 0x2000 && address <= 0x3FFF){
        address = 0x2000 + (address % 8);
        if(CPU_PPU_PERM[address % 8] & READ){
            return internal_mem[address];
        } else{
            spdlog::error("Invalid CPU read to PPU: 0x{:X}", address);
            return 0;
        }
    } else{
        return internal_mem[address];
    }

}