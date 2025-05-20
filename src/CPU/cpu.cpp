#include "cpu.h"
#include "../romLoader.h"
#include "../global.h"


CPU::CPU(PPU *ppu) : ppu(ppu) {
    if (ppu) {
        ppu->set_cpu(this);
        ppu->ppu_power_up();
    }
}

void CPU::increment_cycle_counter(uint32_t cycles) {
    this->cycles += cycles;
    total_cycles += cycles;
    if (ppu) {
        unsigned int ppu_cycles = cycles * 3;
        for (unsigned int i = 0; i < ppu_cycles; i++) {
            ppu->execute_cycle();
        }
    }
}

void CPU::write(uint16_t address, uint8_t operand) {

    if (address < 0 || address > sizeof(mem)) {
        spdlog::error("Invalid CPU write to address: 0x{:X}", address);
        reminescent::exit(1);
    }

    if (rw_register_mode) {
        switch (address) {
            case AC_ADDRESS:
                registers.ac = operand;
                break;
            case X_ADDRESS:
                registers.x = operand;
                break;
            case Y_ADDRESS:
                registers.y = operand;
                break;
            case PC_ADDRESS:
                registers.pc = operand;
                break;
            case SP_ADDRESS:
                registers.sp = operand;
                break;
            case SR_ADDRESS:
                registers.sr = operand;
                break;
            default:
                spdlog::error("Invalid CPU write to illegal register: 0x{:X}", address);
                reminescent::exit(1);
        }
        return;
    }
    increment_cycle_counter();
    if (((address >= NES_PPU_REGISTER_START && address <= NES_PPU_REGISTER_MIRRORS_END) || address == OAMDMA_ADDR) &&
        ppu) {
        if (address != OAMDMA_ADDR) {
            address = NES_PPU_REGISTER_START + (address % 8);
        }
        ppu->cpu_write(address, operand);
        if (address == OAMDMA_ADDR) {
            uint16_t addr = operand * 0x100;
            increment_cycle_counter();
            if (total_cycles % 2 != 0) {
                increment_cycle_counter();
            }
            for (int i = 0; i < 256; i++) {
                uint8_t buffer = read(addr + i);
                ppu->OAM_write(buffer);
                increment_cycle_counter();
            }
        }
    } else if (address >= NES_APU_IO_REGISTERS_START && address <= NES_APU_IO_REGISTERS_END){
        io_bus = operand;
    } else {
        mem[address] = operand;
    }
}


uint8_t CPU::read(uint16_t address) {

    if (address < 0 || address > sizeof(mem)) {
        spdlog::error("Invalid CPU write to address: 0x{:X}", address);
        reminescent::exit(1);
    }

    if (rw_register_mode) {
        switch (address) {
            case AC_ADDRESS:
                return registers.ac;
            case X_ADDRESS:
                return registers.x;
            case Y_ADDRESS:
                return registers.y;
            case PC_ADDRESS:
                return registers.pc;
            case SP_ADDRESS:
                return registers.sp;
            case SR_ADDRESS:
                return registers.sr;
            default:
                spdlog::error("Invalid CPU read to illegal register: 0x{:X}", address);
                reminescent::exit(1);
        }
    }
    increment_cycle_counter();
    if (((address >= NES_PPU_REGISTER_START && address <= NES_PPU_REGISTER_MIRRORS_END) || address == OAMDMA_ADDR) &&
        ppu) {
        address = NES_PPU_REGISTER_START + (address % 8);
        return ppu->cpu_read(address);
    }
    if (address >= NES_APU_IO_REGISTERS_START && address <= NES_APU_IO_REGISTERS_END) {
        return 0x40;
    }
    return mem[address];
}

void CPU::NMI_handler() {
    increment_cycle_counter();
    uint8_t front = (registers.pc) >> 8;
    uint8_t back = (registers.pc) & 0xFF;
    push(front);
    push(back);
    php();
    registers.sr |= 0x04;
    jmp(NMI_VECTOR);
    increment_cycle_counter();
}

void CPU::power_up(const std::string &rom_path) {
    memset(mem, 0, sizeof(mem));
    instruction_counter = 1;
    cycles = 0;
    total_cycles = 0;
    // registers.sr = 0x34;
    registers.sr = 0x4;
    registers.ac = 0, registers.x = 0, registers.y = 0;
    registers.sp = 0xFD;
    rw_register_mode = 0x0;
    current_operand = 0;
    nmi_requested = false;
    mem[0x4017] = 0;
    mem[0x4015] = 0;
    for (int i = 0x4000; i <= 0x400F; i++) {
        mem[i] = 0;
    }
    for (int i = 0x4010; i <= 0x4013; i++) {
        mem[i] = 0;
    }
    increment_cycle_counter(5);
    if (ppu) {
        ppu->execute_cycle();
        ppu->execute_cycle();
        ppu->execute_cycle();
        ppu->execute_cycle();
        ppu->execute_cycle();
        ppu->execute_cycle();
    }
    if (!args.instruction_cycles) {
        load_rom(this, rom_path);
    }
    registers.pc = RESET_VECTOR;
    spdlog::info("RESET_VECTOR: 0x{:X}", registers.pc);
}
