//
// Created by eafc6 on 7/21/2023.
//

#ifndef EMULATOR_PPU_H
#define EMULATOR_PPU_H

#include <cstdint>

class CPU;

#define PATTERN_TABLE_0 0x0000
#define PATTERN_TABLE_1 0x1000
#define NAMETABLE_0 0x2000
#define NAMETABLE_1 0x2400
#define NAMETABLE_2 0x2800
#define NAMETABLE_3 0x2C00
#define NAMETABLE_MIRRORS 0x3000
#define PALETTE_BACKGROUND 0x3F00
#define PALETTE_RAM_MIRRORS 0x4000

#define RW_HALT 0xFFFF

// register addresses
#define PPUCTRL_ADDR 0x2000
#define PPUMASK_ADDR 0x2001
#define PPUSTATUS_ADDR 0x2002
#define OAMADDR_ADDR 0x2003
#define OAMDATA_ADDR 0x2004
#define PPUSCROLL_ADDR 0x2005
#define PPUADDR_ADDR 0x2006
#define PPUDATA_ADDR 0x2007
#define OAMDMA_ADDR 0x4014

class PPU {

    uint8_t OAM[256]; // object attribute memory (OAM)
    uint8_t ppu_mem[16384]; // VRAM
    uint8_t ppudata_buffer; // For accuracy, buffer prefetches for next PPUDATA read
    uint8_t frame[256][240];
    uint8_t frame_buffer[256][240];
    uint16_t scanline;
    uint8_t nmi_triggered;
    CPU* cpu;

    void set_vblank();
    void clear_vblank();


public:

    uint32_t cycles;

    PPU();

    struct {
        uint8_t ppuctrl;
        uint8_t ppumask;
        uint8_t ppustatus;
        uint8_t oamaddr;
        uint8_t oamdata;
        uint8_t ppuscroll;
        uint16_t ppuaddr;
        uint8_t ppudata;
        uint8_t oamdma;

        // Internal registers ----------------

        /* During rendering, used for the scroll position.
         * Outside of rendering, used as the current VRAM address. */
        uint16_t v;

        /* During rendering, specifies the starting coarse-x
         * scroll for the next scanline and the starting y scroll
         * for the screen. Outside of rendering, holds the scroll
         * or VRAM address before transferring it to v.*/
        uint16_t t;

        /* The fine-x position of the current scroll, used
         * during rendering alongside v. */
        uint8_t x;

        /* Toggles on each write to either PPUSCROLL or PPUADDR,
         * indicating whether this is the first or second write.
         * Clears on reads of PPUSTATUS. Sometimes called the
         * 'write latch' or 'write toggle'. */
        uint8_t w;
    } registers;


    void write(uint16_t address, uint8_t operand);
    uint8_t read(uint16_t address);

    void execute_cycle();
    void set_cpu(CPU* cpu);

    void ppu_power_up();

    void render_background();
    uint16_t get_base_nametable_address() const;
    uint8_t get_vram_address_increment() const;
    uint16_t get_sprite_pattern_table_address() const;
    uint16_t get_background_pattern_table_address() const;
    uint8_t get_sprite_size() const;
    uint8_t get_PPU_select() const;
    uint8_t get_NMI() const;
    uint8_t is_greyscale() const;
    uint8_t background_visibility() const;
    uint8_t sprites_visibility() const;
    uint8_t background_shown() const;
    uint8_t sprites_shown() const;
    uint8_t is_red_emphasized() const;
    uint8_t is_green_emphasized() const;
    uint8_t is_blue_emphasized() const;
    uint8_t PPU_open_bus() const;
    uint8_t sprite_overflow() const;
    uint8_t sprite_zero_hit() const;
    uint8_t is_in_vblank() const;

};

#endif //EMULATOR_PPU_H
