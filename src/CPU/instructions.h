//
// Created by eafc6 on 7/17/2023.
//

#ifndef EMULATOR_INSTRUCTIONS_H
#define EMULATOR_INSTRUCTIONS_H

#include "../global.h"
#include <cstdint>

void clear_negative_flag();
void clear_zero_flag();
void set_zero_flag();
void set_negative_flag();
void set_overflow_flag();
void assign_zero_status(uint8_t operand); // Sets the zero flag based on whether the operand is zero.
void assign_negative_status(uint8_t operand); // Sets the negative flag based on operand's sign.
void stack_decrement(); // Decrements the stack pointer.
void stack_increment(); // Increments the stack pointer.

// Instruction Definitions

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

// Memory Addressing Modes
void accumulator(void (*instruction)(uint8_t));
void immediate(void (*instruction)(uint8_t));
void zero_page(void (*instruction)(uint8_t));
void zero_page_x(void (*instruction)(uint8_t));
void zero_page_y(void (*instruction)(uint8_t));
void absolute(void (*instruction)(uint16_t));
void absolute(void (*instruction)(uint8_t));
bool absolute_x(void (*instruction)(uint8_t));
bool absolute_y(void (*instruction)(uint8_t));
void indirect(void (*instruction)(uint16_t));
void indirect_x(void (*instruction)(uint8_t));
bool indirect_y(void (*instruction)(uint8_t));

void accumulator(void (*instruction)(uint16_t));
void immediate(void (*instruction)(uint16_t));
void zero_page(void (*instruction)(uint16_t));
void zero_page_x(void (*instruction)(uint16_t));
void zero_page_y(void (*instruction)(uint16_t));
void absolute(void (*instruction)(uint8_t &));
void absolute_x(void (*instruction)(uint16_t));
void absolute_y(void (*instruction)(uint16_t));
void indirect_x(void (*instruction)(uint16_t));
void indirect_y(void (*instruction)(uint16_t));
#endif //EMULATOR_INSTRUCTIONS_H