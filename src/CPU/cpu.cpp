#include "cpu.h"
#include "../global.h"
#include "../romLoader.h"
#include "../PPU/ppu.h"

uint8_t cpu_mem[65536];

void cpu_write(uint16_t address, uint8_t operand){
    if(cpu_registers.rw_register_mode){
        switch (address) {
            case 0x0000:
                cpu_registers.ac = operand;
                break;
            case 0x0001:
                cpu_registers.x = operand;
                break;
            case 0x0002:
                cpu_registers.y = operand;
                break;
            case 0x0003:
                cpu_registers.pc = operand;
                break;
            case 0x0004:
                cpu_registers.sp = operand;
                break;
            case 0x0005:
                cpu_registers.sr = operand;
                break;
            default:
                spdlog::error("Invalid CPU write to illegal register: 0x{:X}", address);
                exit();
        }
        return;
    }
    if(address >= NES_PPU_REGISTER_START && address <= NES_PPU_REGISTER_MIRRORS_END){
        address = NES_PPU_REGISTER_START + (address % 8);
        if(CPU_PPU_PERM[address % 8] > READ){
            ppu_registers.rw_register_mode = 0x1;
            ppu_write(address, operand);
        } else{
            spdlog::error("Invalid CPU write to PPU: 0x{:X}", address);
        }
    } else{
        cpu_mem[address] = operand;
    }
}


uint8_t cpu_read(uint16_t address){
    if(cpu_registers.rw_register_mode){
        switch (address) {
            case 0x0000:
                return cpu_registers.ac;
            case 0x0001:
                return cpu_registers.x;
            case 0x0002:
                return cpu_registers.y;
            case 0x0003:
                return cpu_registers.pc;
            case 0x0004:
                return cpu_registers.sp;
            case 0x0005:
                return cpu_registers.sr;
            default:
                spdlog::error("Invalid CPU read to illegal register: 0x{:X}", address);
                exit();
        }
    }
    if(address >= NES_PPU_REGISTER_START && address <= NES_PPU_REGISTER_MIRRORS_END){
        address = NES_PPU_REGISTER_START + (address % 8);
        if(CPU_PPU_PERM[address % 8] & READ){
            ppu_registers.rw_register_mode = 0x1;
            return ppu_read(address);
        } else{
            spdlog::error("Invalid CPU read to PPU: 0x{:X}", address);
            return 0;
        }
    } else{
        return cpu_mem[address];
    }

}

void cpu_power_up(){
    memset(cpu_mem, 0, 65536);
    cpu_registers.sr = 0x34;
    cpu_registers.ac = 0, cpu_registers.x = 0, cpu_registers.y = 0;
    cpu_registers.sp = 0xFD;
    cpu_registers.rw_register_mode = 0x0;
    cpu_mem[0x4017] = 0;
    cpu_mem[0x4015] = 0;
    for(int i = 0x4000; i <= 0x400F; i++){
        cpu_mem[i] = 0;
    }
    for(int i = 0x4010; i <= 0x4013; i++){
        cpu_mem[i] = 0;
    }
    load_rom();
    cpu_registers.pc = RESET_VECTOR;
    spdlog::info("RESET_VECTOR: 0x{:X}",RESET_VECTOR);
}