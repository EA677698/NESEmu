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
            if (registers.w == 0) {
                registers.t = (registers.t & 0x7FE0) | ((operand & 0xF8) >> 3);  // Coarse X
                registers.x = operand & 0x07;
            } else {
                registers.t = (registers.t & 0x0C1F) | ((operand & 0x07) << 12); // Fine Y
                registers.t = (registers.t & 0xFC1F) | ((operand & 0xF8) << 2);   // Coarse Y
            }
            break;
        case PPUADDR_ADDR:
            registers.w = !registers.w;
            if(!registers.w){
                registers.t = (registers.t & 0x00FF) | ((operand & 0x3F) << 8);
            } else {
                registers.t = (registers.t & 0xFF00) | operand;
                registers.v = registers.t;
            }
            break;
        case PPUDATA_ADDR:
            registers.ppudata = operand;
            ppu_mem[registers.v & 0x3FFF] = registers.ppudata;
            registers.ppuaddr += get_vram_address_increment();
            break;
        default:
            registers.oamdma = operand;
            break;
    }
}


uint8_t PPU::read(uint16_t address) {
    uint8_t status;
    switch (address) {
        case PPUSTATUS_ADDR:
            status = registers.ppustatus;
            registers.ppustatus &= 0x7F;
            registers.w = 0x0;
            return status;
        case OAMDATA_ADDR:
            return OAM[registers.oamaddr];
        default:
            registers.ppudata = ppudata_buffer;
            ppudata_buffer = ppu_mem[registers.v & 0x3FFF];
            registers.v += get_vram_address_increment();
            return registers.ppudata;
    }
}

void PPU::set_cpu(CPU *cpu) {
    this->cpu = cpu;
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
    ppudata_buffer = 0x0;
    nmi_triggered = 0;
    scanline = 0;
    cycles = -1;
    memset(frame, 0, sizeof(frame));
    load_system_palette("../../Composite_wiki.pal");
}

void PPU::execute_cycle() {

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
        if (cycles >= 0 && cycles < 256) {
            render_background(); // Background rendering for each pixel on this scanline
        } else if (cycles >= 256 && cycles < 321) {
            // Placeholder: Sprite evaluation for the next scanline (fetching OAM data)
            // This step will help prepare sprite data for the next line, if implemented.
        } else if (cycles >= 321 && cycles < 337) {
            // Placeholder: Nametable fetching for the upcoming scanline
            // The PPU fetches data for smooth scrolling and prepares tiles for rendering.
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
        // Pre-render setup could go here, such as resetting PPU scroll registers if implemented
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
    switch (cycles % 8) {
        case 1: // Retrieve nametable tile
            tile_column = tile_column % 32;
            address = table_address + (scanline * 0x20) + tile_column;
            tile = read(address);
            tile_column++;
            break;
        case 3: // Retrieve attribute byte (2x2 tile quadrant)
            address = (table_address + 0x3C0) + ((scanline / 2) * 16) + (tile_column / 2);
            attribute = read(address);
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
                frame[scanline][i * tile_column] = get_rgb_from_composite_palette(ppu_mem[PALETTE_BACKGROUND + (attribute * 4) + data[i]]);
            }
            scanline++;
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
        spdlog::error("Error loading system palette file");
    }
}

RGB PPU::get_rgb_from_composite_palette(uint8_t nes_color_index) {
    int base_index = nes_color_index * 3;
    return { system_palette[base_index], system_palette[base_index + 1], system_palette[base_index + 2] };
}

