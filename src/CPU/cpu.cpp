#include "cpu.h"
#include "../romLoader.h"


CPU::CPU(PPU ppu) : ppu(ppu){
}

void CPU::write(uint16_t address, uint8_t operand){
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
                exit();
        }
        return;
    }
    if(address >= NES_PPU_REGISTER_START && address <= NES_PPU_REGISTER_MIRRORS_END){
        address = NES_PPU_REGISTER_START + (address % 8);
        if(CPU_PPU_PERM[address % 8] > READ){
            ppu.write(address, operand);
        } else{
            spdlog::error("Invalid CPU write to PPU: 0x{:X}", address);
        }
    } else{
        mem[address] = operand;
    }
}


uint8_t CPU::read(uint16_t address){
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
                exit();
        }
    }
    if(address >= NES_PPU_REGISTER_START && address <= NES_PPU_REGISTER_MIRRORS_END){
        address = NES_PPU_REGISTER_START + (address % 8);
        if(CPU_PPU_PERM[address % 8] & READ){
            return ppu.read(address);
        }
        spdlog::error("Invalid CPU read to PPU: 0x{:X}", address);
        return 0;
    }
    return mem[address];
}

void CPU::power_up(const std::string &rom_path){
    memset(mem, 0, 65536);
    registers.sr = 0x34;
    registers.ac = 0, registers.x = 0, registers.y = 0;
    registers.sp = 0xFD;
    registers.rw_register_mode = 0x0;
    mem[0x4017] = 0;
    mem[0x4015] = 0;
    for(int i = 0x4000; i <= 0x400F; i++){
        mem[i] = 0;
    }
    for(int i = 0x4010; i <= 0x4013; i++){
        mem[i] = 0;
    }
    load_rom(this, rom_path);
    registers.pc = RESET_VECTOR;
    printf("PC Register: 0x%X\n", registers.pc);
    spdlog::info("RESET_VECTOR: 0x{:X}",RESET_VECTOR);
    registers.cycles = 0;
}