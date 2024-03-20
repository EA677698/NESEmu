//
// Created by eafc6 on 8/19/2023.
//

#ifndef EMULATOR_CPU_H
#define EMULATOR_CPU_H
#include <cstdint>
#include "../PPU/ppu.h"
#include "spdlog/spdlog.h"
#include "../global.h"

#define RESET_VECTOR ((((uint16_t) mem[0xFFFD]) << 8) | mem[0xFFFC])

#define PPUCTRL_ADDR 0x2000
#define PPUMASK_ADDR 0x2001
#define PPUSTATUS_ADDR 0x2002
#define OAMADDR_ADDR 0x2003
#define OAMDATA_ADDR 0x2004
#define PPUSCROLL_ADDR 0x2005
#define PPUADDR_ADDR 0x2006
#define PPUDATA_ADDR 0x2007
#define OAMDMA_ADDR 0x4014

#define ZERO_PAGE_BEGIN 0x0
#define ZERO_PAGE_END 0xFF
#define STACK_START 0x1FF
#define STACK_END 0x100
#define NES_PPU_REGISTER_START 0x2000
#define NES_PPU_REGISTER_END 0x2007
#define NES_APU_IO_REGISTERS_START 0x4000
#define NES_APU_IO_REGISTERS_END 0x4017
#define CARTRIDGE_SPACE_START 0x4020
#define NES_PPU_REGISTER_MIRRORS_START 0x2008
#define NES_PPU_REGISTER_MIRRORS_END 0x3FFF

#define READ 0x1
#define WRITE 0x2

class CPU {

private:

    PPU ppu;

    const uint8_t CPU_PPU_PERM[9] = {WRITE,WRITE,READ,WRITE,READ | WRITE, WRITE, WRITE, READ | WRITE, WRITE};

    void clear_negative_flag();

    void clear_zero_flag();

    void set_zero_flag();

    void set_negative_flag();

    void set_overflow_flag();

    void assign_zero_status(uint8_t operand); // Sets the zero flag based on whether the operand is zero.
    void assign_negative_status(uint8_t operand); // Sets the negative flag based on operand's sign.
    void stack_decrement(); // Decrements the stack pointer.
    void stack_increment(); // Increments the stack pointer.
    void push(uint8_t operand);
    uint8_t pop();

public:
    uint8_t mem[65536];

    CPU(PPU ppu);

    struct Register {
        uint8_t ac; //accumulator
        uint8_t x; //x index
        uint8_t y; //y  index
        uint16_t pc; //program counter
        uint8_t sp; //stack pointer

        //bits: Negative flag, Overflow flag, Reserved, Break command, Decimal mode, IRQ disable, Zero flag, Carry flag
        uint8_t sr; //sr register (flags)

        // NOT ACTUAL REGISTERS VVVVVVV
        uint32_t cycles;
        uint8_t rw_register_mode; // read/write register mode to 16 bit addresses -- horrible hack, but it'll do
        uint16_t operand; // operand for the current instruction
    };

    Register registers;

    void execute_opcode(int opcode);

    void write(uint16_t address, uint8_t operand);

    uint8_t read(uint16_t address);

    void power_up(const std::string &rom_path);

    //--------------------------------INSTRUCTIONS ---------------------------------


    // Instruction Declarations

    /* Add with Carry
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Immediate       | 0x69
     * Zero Page       | 0x65
     * Zero Page,X     | 0x75
     * Absolute        | 0x6D
     * Absolute,X      | 0x7D
     * Absolute,Y      | 0x79
     * (Indirect,X)    | 0x61
     * (Indirect),Y    | 0x71
     */
    void adc(uint8_t operand);

    /* Logical AND
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Immediate       | 0x29
     * Zero Page       | 0x25
     * Zero Page,X     | 0x35
     * Absolute        | 0x2D
     * Absolute,X      | 0x3D
     * Absolute,Y      | 0x39
     * (Indirect,X)    | 0x21
     * (Indirect),Y    | 0x31
     */
    void logical_and(uint8_t operand);

    /* Arithmetic Shift Left
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Accumulator     | 0x0A
     * Zero Page       | 0x06
     * Zero Page,X     | 0x16
     * Absolute        | 0x0E
     * Absolute,X      | 0x1E
     */
    void asl(uint16_t address);

    /* Branch if Carry Clear
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Relative        | 0x90
     */
    void bcc(int8_t operand);

    /* Branch if Carry Set
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Relative        | 0xB0
     */
    void bcs(int8_t operand);

    /* Branch if Equal
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Relative        | 0xF0
     */
    void beq(int8_t operand);

    /* Bit Test
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Zero Page       | 0x24
     * Absolute        | 0x2C
     */
    void bit(uint8_t operand);

    /* Branch if Minus
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Relative        | 0x30
     */
    void bmi(int8_t operand);

    /* Branch if Not Equal
    *
    * Addressing Mode | Opcode
    * ------------------------
    * Relative        | 0xD0
    */
    void bne(int8_t operand);

    /* Branch if Positive
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Relative        | 0x10
     */
    void bpl(int8_t operand);

    /* Force Interrupt
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0x00
     */
    void brk();

    /* Branch if Overflow Clear
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Relative        | 0x50
     */
    void bvc(int8_t operand);

    /* Branch if Overflow Set
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Relative        | 0x70
     */
    void bvs(int8_t operand);

    /* Clear Carry Flag
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0x18
     */
    void clc();

    /* Clear Decimal Mode
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0xD8
     */
    void cld();

    /* Clear Interrupt Disable
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0x58
     */
    void cli();

    /* Clear Overflow Flag
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0xB8
     */
    void clv();

    /* Compare
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Immediate       | 0xC9
     * Zero Page       | 0xC5
     * Zero Page,X     | 0xD5
     * Absolute        | 0xCD
     * Absolute,X      | 0xDD
     * Absolute,Y      | 0xD9
     * (Indirect,X)    | 0xC1
     * (Indirect),Y    | 0xD1
     */
    void cmp(uint8_t operand);

    /* Compare X Register
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Immediate       | 0xE0
     * Zero Page       | 0xE4
     * Absolute        | 0xEc
     */
    void cpx(uint8_t operand);

    /* Compare Y Register
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Immediate       | 0xC0
     * Zero Page       | 0xC4
     * Absolute        | 0xCC
     */
    void cpy(uint8_t operand);

    /* Decrement Memory
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Zero Page       | 0xC6
     * Zero Page,X     | 0xD6
     * Absolute        | 0xCE
     * Absolute,X      | 0xDE
     */
    void dec(uint8_t operand);

    /* Decrement X Register
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0xCA
     */
    void dex();

    /* Decrement Y Register
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0x88
     */
    void dey();

    /* Exclusive OR
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Immediate       | 0x49
     * Zero Page       | 0x45
     * Zero Page,X     | 0x55
     * Absolute        | 0x4D
     * Absolute,X      | 0x5D
     * Absolute,Y      | 0x59
     * (Indirect,X)    | 0x41
     * (Indirect),Y    | 0x51
     */
    void eor(uint8_t operand);

    /* Increment Memory
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Zero Page       | 0xE6
     * Zero Page,X     | 0xF6
     * Absolute        | 0xEE
     * Absolute,X      | 0xFE
     */
    void inc(uint8_t operand);

    /* Increment X Register
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0xE8
     */
    void inx();

    /* Increment Y Register
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0xC8
     */
    void iny();

    /* Jump
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Absolute        | 0x4C
     * Indirect        | 0x6C
     */
    void jmp(uint16_t address);

    /* Jump to Subroutine
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Absolute        | 0x20
     */
    void jsr(uint16_t operand);

    /* Load Accumulator
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Immediate       | 0xA9
     * Zero Page       | 0xA5
     * Zero Page,X     | 0xB5
     * Absolute        | 0xAD
     * Absolute,X      | 0xBD
     * Absolute,Y      | 0xB9
     * (Indirect,X)    | 0xA1
     * (Indirect),Y    | 0xB1
     */
    void lda(uint8_t operand);

    /* Load X Register
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Immediate       | 0xA2
     * Zero Page       | 0xA6
     * Zero Page,Y     | 0xB6
     * Absolute        | 0xAE
     * Absolute,Y      | 0xBE
     */
    void ldx(uint8_t operand);

    /* Load Y Register
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Immediate       | 0xA0
     * Zero Page       | 0xA4
     * Zero Page,X     | 0xB4
     * Absolute        | 0xAC
     * Absolute,X      | 0xBC
     */
    void ldy(uint8_t operand);

    /* Logical Shift Right
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Accumulator     | 0x4A
     * Zero Page       | 0x46
     * Zero Page,X     | 0x56
     * Absolute        | 0x4E
     * Absolute,X      | 0x5E
     */
    void lsr(uint16_t address);

    /* No Operation
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0xEA
     *
     * UNOFFICIAL NOPS
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0x1A
     * Implied         | 0x3A
     * Implied         | 0x5A
     * Implied         | 0x7A
     * Implied         | 0xDA
     * Implied         | 0xFA
     * Immediate       | 0x80
     * Immediate       | 0x82
     * Immediate       | 0x89
     * Immediate       | 0xC2
     * Immediate       | 0xE2
     * Zero Page       | 0x04
     * Zero Page       | 0x44
     * Zero Page       | 0x64
     * Zero Page,X     | 0x14
     * Zero Page,X     | 0x34
     * Zero Page,X     | 0x54
     * Zero Page,X     | 0x74
     * Zero Page,X     | 0xD4
     * Zero Page,X     | 0xF4
     * Absolute        | 0x0C
     * Absolute,X      | 0x1C
     * Absolute,X      | 0x3C
     * Absolute,X      | 0x5C
     * Absolute,X      | 0x7C
     * Absolute,X      | 0xDC
     * Absolute,X      | 0xFC
     */
    void nop();

    /* Logical Inclusive OR
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Immediate       | 0x09
     * Zero Page       | 0x05
     * Zero Page,X     | 0x15
     * Absolute        | 0x0D
     * Absolute,X      | 0x1D
     * Absolute,Y      | 0x19
     * (Indirect,X)    | 0x01
     * (Indirect),Y    | 0x11
     */
    void ora(uint8_t operand);

    /* Push Accumulator
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0x48
     */
    void pha();

    /* Push Processor Status
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0x08
     */
    void php();

    /* Pull Accumulator
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0x68
     */
    void pla();

    /* Pull Processor Status
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0x28
     */
    void plp();

    /* Rotate Left
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Accumulator     | 0x2A
     * Zero Page       | 0x26
     * Zero Page,X     | 0x36
     * Absolute        | 0x2E
     * Absolute,X      | 0x3E
     */
    void rol(uint16_t address);

    /* Rotate Right
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Accumulator     | 0x6A
     * Zero Page       | 0x66
     * Zero Page,X     | 0x76
     * Absolute        | 0x6E
     * Absolute,X      | 0x7E
     */
    void ror(uint16_t address);

    /* Return from Interrupt
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0x40
     */
    void rti();

    /* Return from Subroutine
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0x60
     */
    void rts();

    /* Subtract with Carry
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Immediate       | 0xE9
     * Zero Page       | 0xE5
     * Zero Page,X     | 0xF5
     * Absolute        | 0xED
     * Absolute,X      | 0xFD
     * Absolute,Y      | 0xF9
     * (Indirect,X)    | 0xE1
     * (Indirect),Y    | 0xF1
     */
    void sbc(uint8_t operand);

    /* Set Carry Flag
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0x38
     */
    void sec();

    /* Set Decimal Flag
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0xF8
     */
    void sed();

    /* Set Interrupt Disable
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0x78
     */
    void sei();

    /* Store Accumulator
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Immediate       | 0x85
     * Zero Page       | 0x95
     * Zero Page,X     | 0x8D
     * Absolute        | 0x9D
     * Absolute,X      | 0x9D
     * Absolute,Y      | 0x99
     * (Indirect,X)    | 0x81
     * (Indirect),Y    | 0x91
     */
    void sta(uint16_t address);

    /* Store X Register
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Zero Page       | 0x86
     * Zero Page,Y     | 0x96
     * Absolute        | 0x8E
     */
    void stx(uint16_t address);

    /* Store Y Register
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Zero Page       | 0x84
     * Zero Page,X     | 0x94
     * Absolute        | 0x8C
     */
    void sty(uint16_t address);

    /* Transfer Accumulator to X
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0xAA
     */
    void tax();

    /* Transfer Accumulator to Y
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0xA8
     */
    void tay();

    /* Transfer Stack Pointer to X
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0xBA
     */
    void tsx();

    /* Transfer X to Accumulator
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0x8A
     */
    void txa();

    /* Transfer X to Stack Pointer
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0x9A
     */
    void txs();

    /* Transfer Y to Accumulator
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Implied         | 0x98
     */
    void tya();


    // ----------------------------- UNOFFICIAL OPCODES -----------------------------


    /* AND operation + LSR
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Immediate       | 0x4B
     */
    void alr();

    /* AND operation + set C as ASL
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Immediate       | 0x0B
     */
    void anc();

    /* AND operation + set C as ROL
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Immediate       | 0x2B
     */
    void anc2();

    /* And X + AND operation
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Immediate       | 0x8B
     */
    void ane();

    /* AND operation + ROR
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Immediate       | 0x6B
     */
    void arr();

    /* Decrement operation + CMP operation
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Zero Page       | 0xC7
     * Zero Page,X     | 0xD7
     * Absolute        | 0xCF
     * Absolute,X      | 0xDF
     * Absolute,Y      | 0xDB
     * (Indirect,X)    | 0xC3
     * (Indirect),Y    | 0xD3
     */
    void dcp();

    /* Increment operation + SBC operation
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Zero Page       | 0xE7
     * Zero Page,X     | 0xF7
     * Absolute        | 0xEF
     * Absolute,X      | 0xFF
     * Absolute,Y      | 0xFB
     * (Indirect,X)    | 0xE3
     * (Indirect),Y    | 0xF3
     */
    void isc();

    /* LDA/TSX operation
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Absolute,Y      | 0xBB
     */
    void las();

    /* LDA operation + LDX operation
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Zero Page       | 0xA7
     * Zero Page,Y     | 0xB7
     * Absolute        | 0xAF
     * Absolute,Y      | 0xBF
     * (Indirect,X)    | 0xA3
     * (Indirect),Y    | 0xB3
     */
    void lax();

    /* Store * AND operation in A and X
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Immediate       | 0xAB
     */
    void lxa();

    /* ROL operation + AND operation
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Zero Page       | 0x27
     * Zero Page,X     | 0x37
     * Absolute        | 0x2F
     * Absolute,X      | 0x3F
     * Absolute,Y      | 0x3B
     * (Indirect,X)    | 0x23
     * (Indirect),Y    | 0x33
     */
    void rla();

    /* ROR operation + ADC operation
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Zero Page       | 0x67
     * Zero Page,X     | 0x77
     * Absolute        | 0x6F
     * Absolute,X      | 0x7F
     * Absolute,Y      | 0x7B
     * (Indirect,X)    | 0x63
     * (Indirect),Y    | 0x73
     */
    void rra();

    /* A and X on the bus at the same time and stored in M
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Zero Page       | 0x87
     * Zero Page,Y     | 0x97
     * Absolute        | 0x8F
     * (Indirect,X)    | 0x83
     */
    void sax();

    /* CMP and DEX at once. Sets CMP flags
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Immediate       | 0xCB
     */
    void sbx();

    /* Stores A AND X and high byte of addr + 1 at addr
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Absolute,Y      | 0x9F
     * (Indirect),Y    | 0x93
     */
    void sha();

    /* Stores X AND high byte of addr + 1 at addr
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Absolute,Y      | 0x9E
     */
    void shx();

    /* Stores Y AND high byte of addr + 1 at addr
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Absolute,X      | 0x9C
     */
    void shy();

    /* ASL operation + ORA operation
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Zero Page       | 0x07
     * Zero Page,X     | 0x17
     * Absolute        | 0x0F
     * Absolute,X      | 0x1F
     * Absolute,Y      | 0x1B
     * (Indirect,X)    | 0x03
     * (Indirect),Y    | 0x13
     */
    void slo();

    /* LSR operation + EOR operation
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Zero Page       | 0x47
     * Zero Page,X     | 0x57
     * Absolute        | 0x4F
     * Absolute,X      | 0x5F
     * Absolute,Y      | 0x5B
     * (Indirect,X)    | 0x43
     * (Indirect),Y    | 0x53
     */
    void sre();

    /* puts A AND X in SP and store A AND X AND high byte of addr + 1 at addr
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Absolute,Y      | 0x9B
     */
    void tas();

     /* SBC operation + NOP
     *
     * Addressing Mode | Opcode
     * ------------------------
     * Immediate       | 0xEB
     */
    void usbc();


    // Memory Addressing Modes
    void accumulator(void (CPU::*instruction)(uint8_t));

    void immediate(void (CPU::*instruction)(uint8_t));

    void zero_page(void (CPU::*instruction)(uint8_t));

    void zero_page_x(void (CPU::*instruction)(uint8_t));

    void zero_page_y(void (CPU::*instruction)(uint8_t));

    void absolute(void (CPU::*instruction)(uint16_t));

    void absolute(void (CPU::*instruction)(uint8_t));

    bool absolute_x(void (CPU::*instruction)(uint8_t));

    bool absolute_y(void (CPU::*instruction)(uint8_t));

    void indirect(void (CPU::*instruction)(uint16_t));

    void indirect_x(void (CPU::*instruction)(uint8_t));

    bool indirect_y(void (CPU::*instruction)(uint8_t));

    void accumulator(void (CPU::*instruction)(uint16_t));

    void immediate(void (CPU::*instruction)(uint16_t));

    void zero_page(void (CPU::*instruction)(uint16_t));

    void zero_page_x(void (CPU::*instruction)(uint16_t));

    void zero_page_y(void (CPU::*instruction)(uint16_t));

    void absolute(void (CPU::*instruction)(uint8_t &));

    void absolute_x(void (CPU::*instruction)(uint16_t));

    void absolute_y(void (CPU::*instruction)(uint16_t));

    void indirect_x(void (CPU::*instruction)(uint16_t));

    void indirect_y(void (CPU::*instruction)(uint16_t));
};

#endif //EMULATOR_CPU_H
