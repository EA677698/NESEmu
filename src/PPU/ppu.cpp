//
// Created by eafc6 on 7/1/2023.
//


#include "../global.h"
#include "ppu.h"
#include "../CPU/cpu.h"


PPU::PPU() = default;

void PPU::write(const uint16_t address, const uint8_t operand) {
    if (address != OAMDMA_ADDR) {
        ppu_io_bus = operand;
    }
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
            if (!registers.w) {
                // 0
                registers.t = (registers.t & 0x7FE0) | ((operand & 0xF8) >> 3); // Coarse X
                registers.x = operand & 0x07;
            } else {
                // 1
                registers.t = (registers.t & 0x8FFF) | ((operand & 0x07) << 12); // Fine Y
                registers.t = (registers.t & 0xFC1F) | ((operand & 0xF8) << 2); // Coarse Y
            }
            registers.w = !registers.w;
            break;
        case PPUADDR_ADDR:
            if (!registers.w) {
                // 0
                registers.t = (registers.t & 0xFF) | ((operand & 0x3F) << 8);
            } else {
                // 1
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
            registers.oamaddr = 0;
            registers.oamdma = operand;
            break;
    }
}

void PPU::cpu_write(uint16_t address, uint8_t operand) {
    write(address, operand);
}

void PPU::OAM_write(uint8_t data) {
    OAM[registers.oamaddr++] = data;
}


uint8_t PPU::read(uint16_t address) {
    uint8_t status;
    uint16_t addr;
    uint8_t value;
    uint8_t result;
    switch (address) {
        case PPUSTATUS_ADDR:
            status = (registers.ppustatus & 0xE0) | (ppu_io_bus & 0x1F);
            registers.ppustatus &= 0x7F;
            registers.w = 0x0;
            return status;
        case OAMDATA_ADDR:
            return OAM[registers.oamaddr];
        case PPUDATA_ADDR:
            addr = registers.v & 0x3FFF;
            value = direct_read(addr);
            if (addr < 0x3F00) {
                result = registers.ppudata;
                registers.ppudata = value;
            } else {
                result = value;
                registers.ppudata = direct_read(addr & 0x2FFF);
            }
            registers.v += get_vram_address_increment();
            return result;
        default:
            return ppu_io_bus;
    }
}

uint8_t PPU::direct_read(uint16_t address) {
    if (address > 16384 || address < 0) {
        reminescent::critical_error("Invalid PPU memory address", 1);
    }
    return ppu_mem[address];
}

uint8_t PPU::cpu_read(uint16_t address) {
    ppu_io_bus = read(address);
    return ppu_io_bus;
}

void PPU::set_cpu(CPU *cpu) {
    this->cpu = cpu;
}

// yyy NN YYYYY XXXXX
// ||| || ||||| +++++-- coarse X scroll
// ||| || +++++-------- coarse Y scroll
// ||| ++-------------- nametable select
// +++----------------- fine Y scroll
// if increment is ever more than 1, overflows may not necessarily be detected
void PPU::increment_register_scrolls(const uint8_t section, uint16_t *internal_register, const uint8_t increment) {
    uint16_t temp;
    switch (section) {
        case COARSE_X_SCROLL:
            if ((*internal_register & 0x001F) == 31) {
                *internal_register &= ~0x001F;
                *internal_register ^= 0x0400;
            } else {
                *internal_register += increment;
            }
            break;
        case COARSE_Y_SCROLL:
            temp = (*internal_register & 0x03E0);
            *internal_register = *internal_register & 0xFC1F;
            if (temp == 0x3A0) {
                *internal_register &= 0xFC1F;
                *internal_register ^= 0x0800;
            } else {
                temp += (increment << 5);
                *internal_register |= temp & 0x3E0;
            }
            break;
        case NAMETABLE_SELECT:
            temp = (*internal_register & 0x0C00) + (increment << 10);
            *internal_register = *internal_register & 0xF3FF;
            *internal_register |= temp & 0x0C00;
            break;
        case FINE_Y_SCROLL:
            temp = (*internal_register & 0x7000) + (increment << 12);
            *internal_register = *internal_register & 0x8FFF;
            *internal_register |= temp & 0x7000;
            if (temp == 0x8000) {
                increment_register_scrolls(COARSE_Y_SCROLL, internal_register, 1);
                // Increment coarse Y if fine Y overflows
            }
            break;
        default:
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
    registers.ppudata = 0x0;
    render.column = 0;
    render.row = 0;
    render.s_column = 0;
    render.index = 0;
    render.buffer = 0;
    render.can_write = true;
    scanline = 0;
    cycles = 0;
    frames = 0;
    t_frame = 0;
    memset(frame, 0, sizeof(frame));
    load_system_palette("Composite_wiki.pal");
}

void PPU::sprite_evaluation() {
    if (render.s_column == 4) {
        render.s_column = 0;
    }
    if (!are_sprites_rendered()) {
        return;
    }

    if (cycles % 2 == 1) {
        render.buffer = OAM[render.row * 4];
        return;
    }

    uint16_t next_scanline = scanline + 1;
    if (next_scanline == 262) {
        next_scanline = 0;
    }

    const uint8_t sprite_y = render.buffer;
    const uint8_t sprite_height = get_sprite_size();
    const bool is_in_range = (uint8_t) (next_scanline - sprite_y) < sprite_height;

    if (render.can_write) {
        if (is_in_range) {
            OAM_secondary[render.index * 4] = sprite_y;
            render.s_column++;
            OAM_secondary[render.index * 4 + 1] = OAM[render.row * 4 + render.s_column++];
            OAM_secondary[render.index * 4 + 2] = OAM[render.row * 4 + render.s_column++];
            OAM_secondary[render.index * 4 + 3] = OAM[render.row * 4 + render.s_column++];
            render.index++;

            render.can_write = render.index != 8;
        } else {
            render.row++;
            render.s_column++;
        }
    } else if (is_in_range) {
        registers.ppustatus |= 0x20; // sprite overflow
    }
    render.row++;
    if (render.row == 64) {
        render.row = 0;
        render.index = 0;
        render.can_write = true;
    }
}


void PPU::execute_cycle() {
    // spdlog::debug("Cycle: {}, Scanline: {}, V: {:04X}, T: {:04X}, VBlank set: {}", cycles, scanline, registers.v, registers.t, is_in_vblank());
    // Advance the cycle and manage scanline/cycle reset
    cycles++;
    if (cycles > 340) {
        // End of scanline
        cycles = 0;
        scanline++;
        if (scanline > 261) {
            scanline = 0; // Wrap around to the first scanline of the next frame
            frames++;
        }
    }
    if (t_frame != frames) {
        spdlog::debug("PPU Breakpoint Reached");
        t_frame = frames;
    }
    if (scanline < 240) {
        // Visible scanlines
        if (cycles < 256) {
            render_background();
            if (cycles == 1) {
                memset(OAM_secondary, 0xFF, sizeof(OAM_secondary)); // Clear OAM secondary buffer
            }
        } else if (cycles >= 65 && cycles < 256) {
            sprite_evaluation();
        } else if (cycles == 256) {
            sprite_evaluation();
            // Increment horizontal scroll
            if (is_background_rendered()) {
                increment_register_scrolls(COARSE_X_SCROLL, &registers.v);
                increment_register_scrolls(FINE_Y_SCROLL, &registers.v);
            }
        } else if (cycles == 257 && is_background_rendered()) {
            registers.v = (registers.v & 0xFBE0) | (registers.t & 0x041F); // copy coarse x t to v
        } else if (cycles >= 258 && cycles < 321) {
            fetch_sprite();
        } else if (cycles >= 321 && cycles < 337) {
            render_background();
        }
    } else if (scanline == 241 && cycles == 1) {
        // Begin VBlank
        set_vblank(); // Set VBlank flag in PPUSTATUS (signals CPU rendering is done)
        if (get_NMI()) {
            cpu->nmi_requested = true;
        }
    } else if (scanline == 261) {
        // Pre-render scanline
        if (cycles == 1) {
            clear_vblank(); // Clear the VBlank flag to prepare for the next frame
        }
        if (cycles < 256) {
            render_background();
        } else if (cycles >= 65 && cycles < 256) {
        } else if (cycles == 256) {
            if (is_background_rendered()) {
                increment_register_scrolls(COARSE_X_SCROLL, &registers.v);
                increment_register_scrolls(FINE_Y_SCROLL, &registers.v);
            }
        } else if (cycles == 257) {
            registers.v = (registers.v & 0xFFE0) | (registers.t & 0x001F); // copy coarse x t to v
        } else if (cycles >= 258 && cycles < 321) {
            if (cycles >= 280 && cycles <= 304 && is_background_rendered()) {
                registers.v = (registers.v & 0x41F) | (registers.t & 0xFBE0); // copy all but x (CORRECT)
                // registers.v = (registers.v & 0xFC20) | (registers.t & 0x3DF); // copy fine y t to v
            }
            fetch_sprite();
        } else if (cycles >= 321 && cycles < 337) {
            render_background();
        }
    }
    if (scanline == 0 && cycles == 0) {
        spdlog::info("Frame {}, v={:04X} t={:04X} x={} w={}", frames, registers.v, registers.t, registers.x,
                     registers.w);
    }
}


void PPU::fetch_sprite() {
    if (!are_sprites_rendered()) {
        return;
    }

    switch (cycles % 8) {
        case 1:
            sprite[render.sprite_index].y_coordinate = OAM_secondary[0];
            break;
        case 2:
            sprite[render.sprite_index].s_tile = OAM_secondary[1];
            break;
        case 3:
            sprite[render.sprite_index].s_attribute = OAM_secondary[2];
            break;
        case 4:
            sprite[render.sprite_index].x_coordinate = OAM_secondary[3];
            render.sprite_index += render.sprite_index == 7 ? -7 : 1;
            break;
        default:
            break;
    }
}

void PPU::render_background() {
    if (!is_background_rendered()) {
        return;
    }

    const uint16_t pattern_addr = get_background_pattern_table_address();

    if (cycles == 0) {
        return;
    }

    switch (cycles % 8) {
        case 0: // Increment horizontal scroll
            increment_register_scrolls(COARSE_X_SCROLL, &registers.v);
            break;
        case 1: // Retrieve nametable tile
            retrieve_nametable_tile(background.address, background.tile);
            break;
        case 3: // Retrieve attribute byte (2x2 tile quadrant)
            retrieve_attribute_byte(background.address, background.attribute);
            break;
        case 5: // Fetch Tile pattern low byte
            retrieve_pattern_lsb(pattern_addr, background.address, background.tile, background.pattern_lsb);
            break;
        case 7: // Fetch Tile pattern high byte
            retrieve_pattern_msb(pattern_addr, background.address, background.pattern_msb);
            priority_mux();
            render_pixels();
            break;
        default:
            break;
    }
}

void PPU::retrieve_nametable_tile(uint16_t &address, uint8_t &tile) {
    address = 0x2000 | (registers.v & 0x0FFF);
    tile = direct_read(address);
#ifndef NDEBUG
    if (tile < 0 || tile > 255) {
        spdlog::critical("Out of bound tile index {:d}", tile);
    }
#endif
}

void PPU::retrieve_attribute_byte(uint16_t &address, uint8_t &attribute) {
    address = 0x23C0 | (registers.v & 0x0C00) | ((registers.v >> 4) & 0x38) | ((registers.v >> 2) & 0x07);
    attribute = direct_read(address);
#ifndef NDEBUG
    // spdlog::info("Attribute Index: {:X}", tile);
    if ((address & 0x03C0) != 0x03C0 || address < 0x2000 || address >= 0x3000) {
        spdlog::critical("Invalid attribute byte address: ${:04X}", address);
    }
#endif
}

void PPU::retrieve_pattern_lsb(uint16_t pattern_addr, uint16_t &address, uint8_t& tile, uint8_t &pattern_lsb) {
    address = pattern_addr + tile * 16 + get_fine_y_scroll();
    pattern_lsb = direct_read(address);
#ifndef NDEBUG
    if (address < pattern_addr || address >= pattern_addr + 0x1000) {
        spdlog::critical("Pattern address out of bounds: ${:04X}", address);
    }
    if (address >= 0x2000) {
        spdlog::critical("Pattern table read beyond 0x1FFF: ${:04X}", address);
    }
#endif
}

void PPU::retrieve_pattern_msb(uint16_t pattern_addr, uint16_t &address, uint8_t &pattern_msb) {
    address += 8;
    pattern_msb = direct_read(address);
#ifndef NDEBUG
    if (address < pattern_addr || address >= pattern_addr + 0x1000) {
        spdlog::critical("Pattern address out of bounds: ${:04X}", address);
    }
    if (address >= 0x2000) {
        spdlog::critical("Pattern table read beyond 0x1FFF: ${:04X}", address);
    }
#endif
}

void PPU::priority_mux() {
    render.select[0] = background.pattern_lsb;
    render.select[1] = background.pattern_msb;
    render.select[2] = background.attribute;
}


void PPU::render_pixels() {
    uint8_t data[8];
    uint8_t render_line = scanline;
    uint8_t pattern_lsb = render.select[0];
    uint8_t pattern_msb = render.select[1];
    uint8_t attribute = render.select[2];
    for (int i = 0; i < 8; i++) {
        // combine planes
        const uint8_t low = (pattern_lsb >> (7 - i)) & 1;
        const uint8_t high = (pattern_msb >> (7 - i)) & 1;
        data[i] = (high << 1) | low;
    }
    if (cycles == 327) {
        render.column = 0;
    }
    if (cycles >= 327) {
        render_line++;
        if (render_line == 262) {
            render_line = 0;
        }
    }
    for (int i = registers.x; i < 8; i++) {
        const int y = get_coarse_y_scroll() % 4;
        const int x = get_coarse_x_scroll() % 4;
        const int shift = 4 * (y > 1) + 2 * (x > 1);
        uint16_t index = PALETTE_BACKGROUND;
        index = index + (((attribute >> shift) & 0x3) * 0x4); // palette set selection
        index = index + (data[i] & 0x3); // color selection
        if (scanline < 240 || cycles >= 327) {
            const uint8_t color_index = direct_read(index);
            frame[render_line][i + render.column] = get_rgb_from_palette(color_index);
        }
    }
    render.column += 8 - registers.x;
}



// Tile column within a nametable (0-31)
uint8_t PPU::get_coarse_x_scroll() const {
    return registers.v & 0x1F;
}

// Tile row within a nametable (0-29)
uint8_t PPU::get_coarse_y_scroll() const {
    return (registers.v & 0x3E0) >> 5;
}

// Selects one of 4 nametables
uint8_t PPU::get_nametable_select() const {
    return (registers.v & 0xC00) >> 10;
}

// Row within the tile (0-7)
uint8_t PPU::get_fine_y_scroll() const {
    return (registers.v & 0x7000) >> 12;
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

uint8_t PPU::is_background_rendered() const {
    return is_bit_set(registers.ppumask, 3);
}

uint8_t PPU::are_sprites_rendered() const {
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

void PPU::set_vblank() {
    registers.ppustatus |= 0x80;
}

void PPU::clear_vblank() {
    registers.ppustatus &= 0x7F;
}

void PPU::load_system_palette(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.read((char *) system_palette, 0xC0);
        file.close();
    } else {
        reminescent::critical_error("Failed to load system palette", 1);
    }
}

RGBA PPU::get_rgb_from_palette(uint8_t nes_color) {
    if (nes_color > 0x3F) {
        reminescent::critical_error("Invalid NES color index", 1);
    }
    uint8_t index = nes_color * 3;
    return {system_palette[index], system_palette[index + 1], system_palette[index + 2]};
}

uint8_t *PPU::get_OAM() {
    return OAM;
}

uint8_t *PPU::get_palette_ram() {
    return ppu_mem + 0x3F00;
}

uint8_t *PPU::get_pattern_1_table() {
    return ppu_mem;
}

uint8_t *PPU::get_pattern_2_table() {
    return ppu_mem + 0x1000;
}
