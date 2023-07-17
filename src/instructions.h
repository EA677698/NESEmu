//
// Created by eafc6 on 7/17/2023.
//

#ifndef EMULATOR_INSTRUCTIONS_H
#define EMULATOR_INSTRUCTIONS_H

#include <cstdint>

void clear_negative_flag();
void clear_zero_flag();
void set_zero_flag();
void set_negative_flag();
void set_overflow_flag();
bool is_bit_set(uint8_t operand, char bit);
void assign_zero_status(uint8_t operand); // Sets the zero flag based on whether the operand is zero.
void assign_negative_status(uint8_t operand); // Sets the negative flag based on operand's sign.
void stack_decrement(); // Increments the stack pointer and checks for underflows.
void stack_increment(); // Increments the stack pointer and checks for overflows.

// Instruction Definitions
void adc(uint8_t operand); // Add with Carry
void logical_and(uint8_t operand); // Logical AND
void asl(uint8_t &operand); // Arithmetic Shift Left
void bcc(int8_t operand); // Branch if Carry Clear
void bcs(int8_t operand); // Branch if Carry Set
void beq(int8_t operand); // Branch if Equal
void bit(uint8_t operand); // Bit Test
void bmi(int8_t operand); // Branch if Minus
void bne(int8_t operand); // Branch if Not Equal
void bpl(int8_t operand); // Branch if Positive
void brk(); // Force Interrupt
void bvc(int8_t operand); // Branch if Overflow Clear
void bvs(int8_t operand); // Branch if Overflow Set
void clc(); // Clear Carry Flag
void cld(); // Clear Decimal Mode
void cli(); // Clear Interrupt Disable
void clv(); // Clear Overflow Flag
void cmp(uint8_t operand); // Compare
void cpx(uint8_t operand); // Compare X Register
void cpy(uint8_t operand); // Compare Y Register
void dec(uint8_t operand); // Decrement Memory
void dex(); // Decrement X Register
void dey(); // Decrement Y Register
void eor(uint8_t operand); // Exclusive OR
void inc(uint8_t operand); // Increment Memory
void inx(); // Increment X Register
void iny(); // Increment Y Register
void jmp(uint16_t address); // Jump
void jsr(uint16_t operand); // Jump to Subroutine
void lda(uint8_t operand); // Load Accumulator
void ldx(uint8_t operand); // Load X Register
void ldy(uint8_t operand); // Load Y Register
void lsr(uint8_t &operand); // Logical Shift Right
void nop(); // No Operation
void ora(uint8_t operand); // Logical Inclusive OR
void pha(); // Push Accumulator
void php(); // Push Processor Status
void pla(); // Pull Accumulator
void plp(); // Pull Processor Status
void rol(uint8_t &operand); // Rotate Left
void ror(uint8_t &operand); // Rotate Right
void rti(); // Return from Interrupt
void rts(); // Return from Subroutine
void sbc(uint8_t operand); // Subtract with Carry
void sec(); // Set Carry Flag
void sed(); // Set Decimal Flag
void sei(); // Set Interrupt Disable
void sta(uint8_t &operand); // Store Accumulator
void stx(uint8_t &operand); // Store X Register
void sty(uint8_t &operand); // Store Y Register
void tax(); // Transfer Accumulator to X
void tay(); // Transfer Accumulator to Y
void tsx(); // Transfer Stack Pointer to X
void txa(); // Transfer X to Accumulator
void txs(); // Transfer X to Stack Pointer
void tya(); // Transfer Y to Accumulator
void accumulator(void (*instruction)(uint8_t));
void immediate(uint8_t operand, void (*instruction)(uint8_t));
void zero_page(uint8_t address, void (*instruction)(uint8_t));
void zero_page_x(uint8_t address, void (*instruction)(uint8_t));
void absolute(uint16_t address, void (*instruction)(uint16_t));
void absolute(uint16_t address, void (*instruction)(uint8_t));
void absolute_x(uint16_t address, void (*instruction)(uint8_t));
void absolute_y(uint16_t address, void (*instruction)(uint8_t));
void indirect(uint16_t address, void (*instruction)(uint16_t));
void indirect_x(uint8_t address, void (*instruction)(uint8_t));
void indirect_y(uint8_t address, void (*instruction)(uint8_t));

#endif //EMULATOR_INSTRUCTIONS_H
