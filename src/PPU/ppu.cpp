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
            registers.t = (registers.t & 0xF3FF) | ((operand & 0x3) << 10);
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
            if (!registers.w) { // 0
                registers.t = (registers.t & 0x7FE0) | ((operand & 0xF8) >> 3);  // Coarse X
                registers.x = operand & 0x07;
            } else { // 1
                registers.t = (registers.t & 0x8FFF) | ((operand & 0x07) << 12); // Fine Y
                registers.t = (registers.t & 0xFC1F) | ((operand & 0xF8) << 2);   // Coarse Y
            }
            registers.w = !registers.w;
            break;
        case PPUADDR_ADDR:
            if(!registers.w) { // 0
                registers.t = (registers.t & 0xC0FF) | ((operand & 0x3F) << 8);
            } else { // 1
                registers.t = (registers.t & 0xFF00) | operand;
                registers.v = registers.t;
            }
            registers.w = !registers.w;
            break;
        case PPUDATA_ADDR:
            registers.ppudata = operand;
            ppu_mem[registers.v & 0x3FFF] = registers.ppudata;
            registers.v += get_vram_address_increment();
            break;
        default:
            registers.oamdma = operand;
            break;
    }
}

void PPU::cpu_write(uint16_t address, uint8_t operand) {
    ppudata_buffer = operand;
    write(address, operand);
}


uint8_t PPU::read(uint16_t address) {
    uint8_t status;
    switch (address) {
        case PPUSTATUS_ADDR:
            status = (registers.ppustatus & 0xE0) | (ppudata_buffer & 0x1F);
            registers.ppustatus &= 0x7F;
            registers.w = 0x0;
            return status;
        case OAMDATA_ADDR:
            return OAM[registers.oamaddr];
        case PPUDATA_ADDR:
            if ((registers.v & 0x3FFF) >= 0x3F00 && (registers.v & 0x3FFF) <= 0x3FFF) {
                registers.ppudata = direct_read(registers.v & 0x3FFF);
            } else {
                registers.ppudata = ppudata_buffer;
            }
            registers.v += get_vram_address_increment();
            return registers.ppudata;
        default:
            return 0xFF;
    }
}

uint8_t PPU::direct_read(uint16_t address) {
    if(address > 16384 || address < 0) {
        reminescent::critical_error("Invalid PPU memory address", 1);
    }
    return ppu_mem[address];
}

uint8_t PPU::cpu_read(uint16_t address) {
    ppudata_buffer = read(address);
    return ppudata_buffer;
}

void PPU::set_cpu(CPU *cpu) {
    this->cpu = cpu;
}

void PPU::increment_register_scrolls(uint8_t section, uint8_t vram_register, uint8_t increment) {
    uint16_t* selected_register = nullptr;
    if (vram_register == PPU_V){
        selected_register = &registers.v;
    } else {
        selected_register = &registers.t;
    }

    uint8_t temp;
    switch (section) {
        case COARSE_X_SCROLL:
            temp = (*selected_register & 0x001F) + increment;
            *selected_register = *selected_register & 0xFFE0;
            *selected_register |= temp & 0x1F;
            break;
        case COARSE_Y_SCROLL:
            temp = (*selected_register & 0x03E0) + increment;
            *selected_register = *selected_register & 0xFC1F;
            *selected_register |= temp & 0x3E0;
            break;
        case NAMETABLE_SELECT:
            temp = (*selected_register & 0x0C00) + increment;
            *selected_register = *selected_register & 0xF3FF;
            *selected_register |= temp & 0x0C00;
            break;
        case FINE_Y_SCROLL:
            temp = (*selected_register & 0x7000) + increment;
            *selected_register = *selected_register & 0x8FFF;
            *selected_register |= temp & 0x7000;
            break;
    }
}


void PPU::ppu_power_up() {
    memset(ppu_mem, 0, sizeof(ppu_mem));
    registers.ppuctrl = 0x0;
    registers.ppumask = 0x0;
    registers.ppustatus = 0x0; //TODO investigate correct setting at power up
    registers.oamaddr = 0x0;
    registers.ppuscroll = 0x0;
    registers.ppuaddr = 0x0;
    registers.ppudata = 0x0;
    registers.w = 0x0;
    registers.v = 0x0;
    registers.t = 0x0;
    registers.x = 0x0;
    ppudata_buffer = 0x0;
    nmi_triggered = 0;
    scanline = 0;
    cycles = 0;
    memset(frame, 0, sizeof(frame));
    load_system_palette("Composite_wiki.pal");
}

void PPU::execute_cycle() {
    // spdlog::debug("Cycle: {}, Scanline: {}, V: {:04X}, T: {:04X}, VBlank set: {}", cycles, scanline, registers.v, registers.t, is_in_vblank());
    // Advance the cycle and manage scanline/cycle reset
    cycles++;
    if (cycles >= 341) { // End of scanline
        cycles = 0;
        scanline++;
        if (scanline > 261) {
            scanline = 0; // Wrap around to the first scanline of the next frame
        }
    }

    // Rendering cycle ranges:
    // 0-255: Visible cycles for rendering the background and potentially sprites.
    // 256-319: Cycles for sprite evaluation (preparation for next scanline).
    // 321-336: Fetch nametable data for the next scanline.
    // 337-340: Idle cycles (no visible effect, padding for timing).

    if (scanline < 240) { // Visible scanlines
        if (cycles < 256) {
            render_background(); // Background rendering for each pixel on this scanline
        } else if (cycles == 256) {
            increment_register_scrolls(COARSE_X_SCROLL, PPU_V);
            increment_register_scrolls(COARSE_Y_SCROLL, PPU_V); 
        } else if (cycles == 257) {
            registers.v = (registers.v & 0xFFE0) | (registers.t & 0x001F); // copy coarse x t to v
        } else if (cycles >= 258 && cycles < 321) {
            // Placeholder: Sprite evaluation for the next scanline (fetching OAM data)
            // This step will help prepare sprite data for the next line, if implemented.
        } else if (cycles >= 321 && cycles < 337) {
            render_background(); // Fetch nametable data for the next scanline
        }
    } else if (scanline == 240) {
        // Post-render scanline - do nothing here as rendering has finished.
    } else if (scanline == 241 && cycles == 1) {
        // Begin VBlank
        set_vblank(); // Set VBlank flag in PPUSTATUS (signals CPU rendering is done)
        if (get_NMI() && !nmi_triggered) { // Trigger NMI if enabled in PPUCTRL
            nmi_triggered = true;
            cpu->NMI_handler(); // This NMI signals the CPU to process VBlank routines
        }
    } else if (scanline == 261) { // Pre-render scanline
        if (cycles == 1) {
            clear_vblank(); // Clear the VBlank flag to prepare for the next frame
            nmi_triggered = false; // Reset NMI trigger status
        }
    }
}


void PPU::render_background() {
    uint16_t table_address = get_base_nametable_address();
    uint16_t pattern_table_address = get_background_pattern_table_address();
    static uint8_t tile_column = 0;
    static uint16_t address;
    static uint8_t tile;
    static uint8_t attribute;
    static uint8_t pattern_low_byte;
    static uint8_t pattern_high_byte;
    uint8_t data[8];

    if (cycles == 0) {
        return;
    }

    switch (cycles % 8) {
        case 0: // Increment horizontal scroll
            increment_register_scrolls(COARSE_X_SCROLL, PPU_V);
            break;
        case 1: // Retrieve nametable tile
            tile_column = tile_column % 32;
            address = table_address + (scanline * 0x20) + tile_column;
            tile = read(address);
            tile_column++;
            break;
        case 3: // Retrieve attribute byte (2x2 tile quadrant)
            address = 0x23C0 | (registers.v & 0x0C00) | ((registers.v >> 4) & 0x38) | ((registers.v >> 2) & 0x07);
            attribute = direct_read(address);
            break;
        case 5: // Fetch Tile pattern low byte
            address = pattern_table_address + tile;
            pattern_low_byte = read(address);
            break;
        case 7: // Fetch Tile pattern high byte
            address = pattern_table_address + tile + 8;
            pattern_high_byte = read(address);
            for (char i = 0; i < 8; i++) {
                data[i] = (is_bit_set(pattern_high_byte,i) << 1) | is_bit_set(pattern_low_byte,i);
            }
            for(int i = 0; i < 8; i++){
                int index = PALETTE_BACKGROUND;
                index = index + (attribute * 4);
                index = index + data[i];
                frame[scanline][i * tile_column] = get_rgb_from_composite_palette(direct_read(index));
            }
            break;
        default:
            break;
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
        return PATTERN_TABLE_1;
    }
    return PATTERN_TABLE_0;
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

void PPU::load_system_palette(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if(file.is_open()){
        file.read((char*)system_palette, 0xC0);
        file.close();
    } else{
        reminescent::critical_error("Failed to load system palette", 1);
    }
}

RGBA PPU::get_rgb_from_composite_palette(uint8_t nes_color_index) {
    int base_index = ((int)nes_color_index) * 3;
    if (base_index >= 0xC0) {
        reminescent::critical_error("Invalid NES color index", 1);
    }
    return { system_palette[base_index], system_palette[base_index + 1], system_palette[base_index + 2] };
}

