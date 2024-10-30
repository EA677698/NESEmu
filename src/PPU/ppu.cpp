//
// Created by eafc6 on 7/1/2023.
//


#include "../global.h"
#include "ppu.h"
#include "../CPU/cpu.h"


PPU::PPU() {
}

void PPU::write(uint16_t address, uint8_t operand) {
    switch (address) {
        case PPUCTRL_ADDR:
            registers.ppuctrl = operand;
            break;
        case PPUMASK_ADDR:
            registers.ppumask = operand;
            break;
        case OAMADDR_ADDR:
            registers.oamaddr = operand;
            break;
        case OAMDATA_ADDR:
            registers.oamdata = operand;
            OAM[registers.oamaddr] = registers.oamdata;
            registers.oamaddr++;
            break;
        case PPUSCROLL_ADDR:
            registers.w = !registers.w;
            registers.ppuscroll = operand;
            break;
        case PPUADDR_ADDR:
            registers.w = !registers.w;
            registers.ppuaddr |= operand << (8 * registers.w);
            break;
        case PPUDATA_ADDR:
            registers.ppudata = operand;
            if (registers.ppuaddr >= NAMETABLE_MIRRORS && registers.ppuaddr <= PALETTE_RAM) {
                ppu_mem[registers.ppuaddr - 0x2000] = registers.ppudata;
            }
            if (registers.ppuaddr >= PALETTE_RAM_MIRRORS) {
                ppu_mem[registers.ppuaddr - 0x3F00] = registers.ppudata;
            }
            registers.ppuaddr += get_vram_address_increment();
            break;
        default:
            registers.oamdma = operand;
            break;
    }
}


uint8_t PPU::read(uint16_t address) {
    switch (address) {
        case PPUSTATUS_ADDR:
            registers.ppustatus &= 0x7F;
            registers.w = 0x0;
            return registers.ppustatus;
        case OAMDATA_ADDR:
            return OAM[registers.oamaddr];
        default:
            registers.ppudata = ppudata_buffer;
            registers.ppuaddr += get_vram_address_increment();
            ppudata_buffer = ppu_mem[registers.ppuaddr];
            return registers.ppudata;
    }
}

void PPU::set_cpu(CPU *cpu) {
    this->cpu = cpu;
}


void PPU::ppu_power_up() {
    registers.ppuctrl = 0x0;
    registers.ppumask = 0x0;
    registers.ppustatus = 0x0; //TODO investigate correct setting at power up
    registers.oamaddr = 0x0;
    registers.ppuscroll = 0x0;
    registers.ppuaddr = 0x0;
    registers.ppudata = 0x0;
    registers.w = 0x0;
    ppudata_buffer = 0x0;
    nmi_triggered = 0;
}

void PPU::execute_cycle() {


    if (cycles >= 0 && cycles < 256) {

    } else if (cycles >= 256 && cycles < 321) {

    } else if (cycles >= 321 && cycles < 341) {

    } else if (cycles >= 341) {
        cycles = 0;
        scanline++;
    }


    if (scanline == 241 && cycles == 1) {
        set_vblank();
    }
    if(is_in_vblank() && get_NMI() && !nmi_triggered){
        cpu->NMI_handler();
        nmi_triggered = 1;
    }

    if (scanline == 261 && cycles == 1) {
        clear_vblank();
        nmi_triggered = 0;
    }
}

void PPU::render_background() {
    uint16_t table_address = get_base_nametable_address();
    uint16_t pattern_table_address = get_background_pattern_table_address();
    static uint8_t i = 0;
    static uint16_t address;
    static uint8_t tile;
    static uint8_t attribute;
    static uint16_t pattern_address;
    static uint8_t data[2][8];
    switch (cycles % 8) {
        case 0:
            i = i % 32;
            address = table_address + (scanline * 0x20) + i;
            i++;
            break;
        case 1:
            tile = read(address);
            break;
        case 2:
            address = table_address + ((scanline - (scanline % 2)) * (0x20 / 4)) + i;
            break;
        case 4:
            pattern_address = pattern_table_address + (tile * 16);
            for(int j = 0; j < 8; j++){
                data[0][j] = read(pattern_address + j);
            }
            break;
        case 5:
            for(int j = 0; j < 8; j++){
                data[1][j] = read(pattern_address + (j + 8));
            }

    }

}

//PPUCTRL
uint16_t PPU::get_base_nametable_address() const {
    switch (registers.ppuctrl & 0x3) {
        case 0x1:
            return NAMETABLE_1;
        case 0x2:
            return NAMETABLE_2;
        case 0x3:
            return NAMETABLE_3;
        default:
            return NAMETABLE_0;
    }
}

uint8_t PPU::get_vram_address_increment() const {
    if (is_bit_set(registers.ppuctrl, 2)) {
        return 32;
    }
    return 1;
}

uint16_t PPU::get_sprite_pattern_table_address() const {
    if (is_bit_set(registers.ppuctrl, 3)) {
        return PATTERN_TABLE_1;
    }
    return PATTERN_TABLE_0;
}

uint16_t PPU::get_background_pattern_table_address() const {
    if (is_bit_set(registers.ppuctrl, 4)) {
        return 0x1000;
    }
    return 0x0000;
}

uint8_t PPU::get_sprite_size() const {
    if (is_bit_set(registers.ppuctrl, 5)) {
        return 16;
    }
    return 8;
}

uint8_t PPU::get_PPU_select() const {
    return is_bit_set(registers.ppuctrl, 6);
}

uint8_t PPU::get_NMI() const {
    return is_bit_set(registers.ppuctrl, 7);
}

//PPUMASK

uint8_t PPU::is_greyscale() const {
    return is_bit_set(registers.ppumask, 0);
}

uint8_t PPU::background_visibility() const {
    return is_bit_set(registers.ppumask, 1);
}

uint8_t PPU::sprites_visibility() const {
    return is_bit_set(registers.ppumask, 2);
}

uint8_t PPU::background_shown() const {
    return is_bit_set(registers.ppumask, 3);
}

uint8_t PPU::sprites_shown() const {
    return is_bit_set(registers.ppumask, 4);
}

uint8_t PPU::is_red_emphasized() const {
    return is_bit_set(registers.ppumask, 5);
}

uint8_t PPU::is_green_emphasized() const {
    return is_bit_set(registers.ppumask, 6);
}

uint8_t PPU::is_blue_emphasized() const {
    return is_bit_set(registers.ppumask, 7);
}

//PPUSTATUS

uint8_t PPU::PPU_open_bus() const {
    return registers.ppustatus & 0x1F;
}

uint8_t PPU::sprite_overflow() const {
    return is_bit_set(registers.ppustatus, 5);
}

uint8_t PPU::sprite_zero_hit() const {
    return is_bit_set(registers.ppustatus, 6);
}

uint8_t PPU::is_in_vblank() const {
    return is_bit_set(registers.ppustatus, 7);
}

// NMI

void PPU::set_vblank(){
    registers.ppustatus |= 0x80;
}

void PPU::clear_vblank(){
    registers.ppustatus &= 0x7F;
}
