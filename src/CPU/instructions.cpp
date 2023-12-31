//
// Created by eafc6 on 7/1/2023.
//

#include <iostream>
#include "instructions.h"
#include "spdlog/spdlog.h"

//helper functions -------------------- NOT INSTRUCTIONS --------------------
bool is_page_crossed(uint16_t initial_address, uint16_t change){
    return (initial_address & 0xFF00) != ((initial_address + change) & 0xFF00);
}

void clear_negative_flag(){
    registers.sr &= 0x7F;
}

void clear_zero_flag(){
    registers.sr &= 0xFD;
}

void set_zero_flag(){
    registers.sr |= 0x2;
}

void set_negative_flag(){
    registers.sr |= 0x80;
}

void set_overflow_flag(){
    registers.sr |= (1 << 6);
}

void assign_zero_status(uint8_t operand){
    if(!operand){
        set_zero_flag();
    } else{
        clear_zero_flag();
    }
}

void assign_negative_status(uint8_t operand){
    if(is_bit_set(operand,7)){
        set_negative_flag();
    } else{
        clear_negative_flag();
    }
}

void stack_decrement(){
    registers.sp--;
}

void stack_increment(){
    registers.sp++;
}

// ----------------------------------------- DEFINED INSTRUCTIONS -----------------------------------------

void sec(){
    registers.sr |= 0x1;
}

void clc(){
    registers.sr &= 0xFE;
}

void sed(){
    registers.sr |= 0x8;
}

void cld(){
    registers.sr &= 0xF7;
}

void sei(){
    registers.sr |= 0x4;
}

void cli(){
    registers.sr &= 0xFB;
}

void clv(){
    registers.sr &= 0xBF;
}

void adc(uint8_t operand){
    uint16_t temp = registers.ac + operand + is_bit_set(registers.sr,0);
    if(temp > 0xFF){
        sec();
    } else {
        clc();
    }
    registers.ac = temp & 0xFF;
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}


void logical_and(uint8_t operand){
    registers.ac &= operand;
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}

void asl(uint16_t address){
    uint8_t operand = cpu_read(address);
    if (is_bit_set(operand,7)) {
        sec();
    } else {
        clc();
    }
    cpu_write(address, operand << 1);
    assign_zero_status(operand);
    assign_negative_status(operand);
    registers.rw_register_mode = 0x0;
}


void bcc(int8_t operand){
    if(!is_bit_set(registers.sr,0)){
        registers.cycles += (1 + is_page_crossed(registers.pc, operand));
        registers.pc += operand;
    }
}

void bcs(int8_t operand){
    if(is_bit_set(registers.sr,0)){
        registers.cycles += (1 + is_page_crossed(registers.pc, operand));
        registers.pc += operand;
    }
}

void beq(int8_t operand){
    if(is_bit_set(registers.sr,1)){
        registers.cycles += (1 + is_page_crossed(registers.pc, operand));
        registers.pc += operand;
    }
}

void bit(uint8_t operand){
    if(!(registers.ac & operand)){
        set_zero_flag();
    } else{
        clear_zero_flag();
    }
    if(is_bit_set(operand,6)){
        set_overflow_flag();
    } else{
        clv();
    }
    assign_negative_status(operand);

}

void bmi(int8_t operand){
    if(is_bit_set(registers.sr,7)){
        registers.cycles += (1 + is_page_crossed(registers.pc, operand));
        registers.pc += operand;
    }
}

void bne(int8_t operand){
    if(!is_bit_set(registers.sr,1)){
        registers.cycles += (1 + is_page_crossed(registers.pc, operand));
        registers.pc += operand;
    }
}

void bpl(int8_t operand){
    if(!is_bit_set(registers.sr,7)){
        registers.cycles += (1 + is_page_crossed(registers.pc, operand));
        registers.pc += operand;
    }
}

void brk(){
    registers.pc += 1;
    registers.sr |= 0x10;
    php();
    uint8_t front = registers.pc >> 8;
    uint8_t back = registers.pc & 0xFF;
    stack_decrement();
    cpu_write(0x100 + registers.sp, back);
    stack_decrement();
    cpu_write(0x100 + registers.sp, front);
    uint16_t interrupt_vector = cpu_read(0xFFFE) | ((uint16_t) (cpu_read(0xFFFF) << 8));
    registers.pc = interrupt_vector;
    spdlog::debug("SETTING PC COUNTER TO INTERRUPT VECTOR: 0x{:X}",interrupt_vector);
}

void bvc(int8_t operand){
    if(!is_bit_set(registers.sr,6)){
        registers.cycles += (1 + is_page_crossed(registers.pc, operand));
        registers.pc += operand;
    }
}

void bvs(int8_t operand){
    if(is_bit_set(registers.sr,6)){
        registers.cycles += (1 + is_page_crossed(registers.pc, operand));
        registers.pc += operand;
    }
}

void cmp(uint8_t operand){
    uint8_t result = registers.ac - operand;
    if (registers.ac >= operand) {
        sec();
    } else {
        clc();
    }
    if (registers.ac == operand) {
        set_zero_flag();
    } else {
        clear_zero_flag();
    }
    assign_negative_status(result);
}

void cpx(uint8_t operand){
    uint8_t result = registers.x - operand;
    if (registers.x >= operand) {
        sec();
    } else {
        clc();
    }
    if (registers.x == operand) {
        set_zero_flag();
    } else {
        clear_zero_flag();
    }
    assign_negative_status(result);
}

void cpy(uint8_t operand){
    uint8_t result = registers.y - operand;
    if (registers.y >= operand) {
        sec();
    } else {
        clc();
    }
    if (registers.y == operand) {
        set_zero_flag();
    } else {
        clear_zero_flag();
    }
    assign_negative_status(result);
}

void dec(uint8_t operand){
    operand--;
    assign_zero_status(operand);
    assign_negative_status(operand);
}

void dex(){
    registers.x--;
    assign_zero_status(registers.x);
    assign_negative_status(registers.x);
}

void dey(){
    registers.y--;
    assign_zero_status(registers.y);
    assign_negative_status(registers.y);
}

void eor(uint8_t operand){
    registers.ac ^= operand;
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}

void inc(uint8_t operand){
    operand++;
    assign_zero_status(operand);
    assign_negative_status(operand);
}

void inx(){
    registers.x++;
    assign_zero_status(registers.x);
    assign_negative_status(registers.x);
}

void iny(){
    registers.y++;
    assign_zero_status(registers.y);
    assign_negative_status(registers.y);
}

void jmp(uint16_t address){
    registers.pc = address;
}

void jsr(uint16_t operand){
    uint8_t front = registers.pc >> 8;
    uint8_t back = registers.pc & 0xFF;
    spdlog::info("Storing address 0x{:X} into stack",registers.pc);
    stack_decrement();
    cpu_write(0x100 + registers.sp, back);
    stack_decrement();
    cpu_write(0x100 + registers.sp, front);
    registers.pc = operand;
    registers.rw_register_mode = 0x0;
}

void lda(uint8_t operand){
    registers.ac = operand;
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}

void ldx(uint8_t operand){
    registers.x = operand;
    assign_zero_status(registers.x);
    assign_negative_status(registers.x);
}

void ldy(uint8_t operand){
    registers.y = operand;
    assign_zero_status(registers.y);
    assign_negative_status(registers.y);
}

void lsr(uint16_t address){
    uint8_t operand = cpu_read(address);
    if(is_bit_set(operand,0)){
        sec();
    } else{
        clc();
    }
    cpu_write(address, operand >> 1);
    assign_zero_status(operand);
    clear_negative_flag();
    registers.rw_register_mode = 0x0;
}

void nop(){}

void ora(uint8_t operand){
    registers.ac |= operand;
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}

void pha(){
    stack_decrement();
    cpu_write(0x100 + registers.sp, registers.ac);
}

void php(){
    stack_decrement();
    cpu_write(0x100 + registers.sp, registers.sr);
}

void pla(){
    stack_increment();
    registers.ac = cpu_read(0x100 + registers.sp);
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}

void plp(){
    stack_increment();
    registers.sr &= 0x0;
    registers.sr |= cpu_read(0x100 + registers.sp);
}

void rol(uint16_t address){
    uint8_t operand = cpu_read(address);
    uint8_t old_carry = is_bit_set(registers.sr,7);
    registers.sr &= 0xFE | ((uint8_t)is_bit_set(operand,7));
    cpu_write(address, operand <<= 1);
    cpu_write(address, operand &= 0xFE | old_carry);
    assign_zero_status(operand);
    assign_negative_status(operand);
    registers.rw_register_mode = 0x0;
}

void ror(uint16_t address){
    uint8_t operand = cpu_read(address);
    uint8_t old_carry = is_bit_set(registers.sr,7);
    registers.sr &= 0xFE | ((uint8_t)is_bit_set(operand,0));
    cpu_write(address, operand >>= 1);
    cpu_write(address, operand &= 0x7F | (old_carry << 7));
    assign_zero_status(operand);
    assign_negative_status(operand);
    registers.rw_register_mode = 0x0;
}

void rti(){
    plp();
    stack_increment();
    uint16_t address = cpu_read(0x100 + registers.sp);
    address <<= 8;
    stack_increment();
    address |= cpu_read(0x100 + registers.sp);
    registers.pc = address;
}

void rts(){
    uint16_t address = cpu_read(0x100 + registers.sp);
    stack_increment();
    address <<= 8;
    address |= cpu_read(0x100 + registers.sp);
    stack_increment();
    spdlog::info("Retrieving address from stack: 0x{:X}",address);
    registers.pc = address;
}

void sbc(uint8_t operand){
    int16_t res = registers.ac - operand - !is_bit_set(registers.sr,0);
    if(res < 0){
        clc();
        set_overflow_flag();
    } else{
        sec();
    }
    registers.ac -= operand - !is_bit_set(registers.sr,0);
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}

void sta(uint16_t address){
    cpu_write(address, registers.ac);
    registers.rw_register_mode = 0x0;
}

void stx(uint16_t address){
    cpu_write(address, registers.x);
    registers.rw_register_mode = 0x0;
}

void sty(uint16_t address){
    cpu_write(address, registers.y);
    registers.rw_register_mode = 0x0;
}

void tax(){
    registers.x = registers.ac;
    assign_zero_status(registers.x);
    assign_negative_status(registers.x);
}

void tay(){
    registers.y = registers.ac;
    assign_zero_status(registers.y);
    assign_negative_status(registers.y);
}

void tsx(){
    registers.x = registers.sp;
    assign_zero_status(registers.x);
    assign_negative_status(registers.x);
}

void txa(){
    registers.ac = registers.x;
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}

void txs(){
    registers.sp = registers.x;
    assign_zero_status(registers.sp);
    assign_negative_status(registers.sp);
}

void tya(){
    registers.ac = registers.y;
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}


// Memory Addressing Modes
void accumulator(void (*instruction)(uint8_t)) {instruction(registers.ac);}
void immediate(void (*instruction)(uint8_t)){uint8_t operand = cpu_read(registers.pc++); instruction(operand);}
void zero_page(void (*instruction)(uint8_t)){uint8_t address = cpu_read(registers.pc++); instruction(cpu_read(address));}
void zero_page_x(void (*instruction)(uint8_t)){uint8_t address = cpu_read(registers.pc++); instruction(cpu_read(address + registers.x));}
void zero_page_y(void (*instruction)(uint8_t)){uint8_t address = cpu_read(registers.pc++); instruction(cpu_read(address + registers.y));}
void absolute(void (*instruction)(uint16_t)) {
    uint16_t lowByte = cpu_read(registers.pc++);
    uint16_t highByte = cpu_read(registers.pc++);
    uint16_t address = (highByte << 8) | lowByte;
    instruction(address);
}
void absolute(void (*instruction)(uint8_t)) {
    uint16_t lowByte = cpu_read(registers.pc++);
    uint16_t highByte = cpu_read(registers.pc++);
    uint16_t address = (highByte << 8) | lowByte;
    instruction(cpu_read(address));
}
bool absolute_x(void (*instruction)(uint8_t)) {
    uint16_t lowByte = cpu_read(registers.pc++);
    uint16_t highByte = cpu_read(registers.pc++);
    uint16_t address = (highByte << 8) | lowByte;
    instruction(cpu_read(address + registers.x));
    return is_page_crossed(address,registers.x);
}
bool absolute_y(void (*instruction)(uint8_t)){
    uint16_t lowByte = cpu_read(registers.pc++);
    uint16_t highByte = cpu_read(registers.pc++);
    uint16_t address = (highByte << 8) | lowByte;
    instruction(cpu_read(address + registers.y));
    return is_page_crossed(address,registers.y);
}
void indirect(void (*instruction)(uint16_t)){
    uint16_t lowByte = cpu_read(registers.pc++);
    uint16_t highByte = cpu_read(registers.pc++);
    uint16_t address = (highByte << 8) | lowByte;
    if(instruction != jmp){
        //TODO throw invalid instruction error
        spdlog::critical("INVALID ADDRESSING MODE FOR JMP INSTRUCTION (INDIRECT ADDRESSING MODE)");
        exit(1);
    }
    instruction(cpu_read(address) | (cpu_read((address & 0xFF00) | ((address + 1) & 0x00FF)) << 8));
}
void indirect_x(void (*instruction)(uint8_t)) {
    uint8_t address = cpu_read(registers.pc++);
    uint8_t zero_page_address = (address + registers.x) & 0xFF;
    uint16_t effective_address = cpu_read(zero_page_address) | (cpu_read((zero_page_address + 1) & 0xFF) << 8);
    instruction(cpu_read(effective_address));
}
bool indirect_y(void (*instruction)(uint8_t)) {
    uint8_t address = cpu_read(registers.pc++);
    uint16_t effective_address = (cpu_read(address) | (cpu_read((address + 1) & 0xFF) << 8)) + registers.y;
    instruction(cpu_read(effective_address));
    return is_page_crossed(registers.pc - 1, effective_address);
}

void accumulator(void (*instruction)(uint16_t)) {registers.rw_register_mode = 0x1 ;instruction(0x0);}
void immediate(void (*instruction)(uint16_t)){instruction(&internal_mem[registers.pc++] - internal_mem);}
void zero_page(void (*instruction)(uint16_t)){instruction(cpu_read(&internal_mem[registers.pc++] - internal_mem));}
void zero_page_x(void (*instruction)(uint16_t)){instruction(cpu_read((&internal_mem[registers.pc++] - internal_mem) + registers.x));}
void zero_page_y(void (*instruction)(uint16_t)){instruction(cpu_read((&internal_mem[registers.pc++] - internal_mem) + registers.y));}

/*
 * Unused supposedly...
 *
void absolute(void (*instruction)(uint8_t &)) {
    uint16_t lowByte = cpu_read(registers.pc++);
    uint16_t highByte = cpu_read(registers.pc++);
    uint16_t address = (highByte << 8) | lowByte;
    instruction(cpu_read(address));
}
*/

void absolute_x(void (*instruction)(uint16_t)) {
    uint16_t lowByte = cpu_read(registers.pc++);
    uint16_t highByte = cpu_read(registers.pc++);
    uint16_t address = (highByte << 8) | lowByte;
    instruction(&internal_mem[address + registers.x] - internal_mem);
}
void absolute_y(void (*instruction)(uint16_t)){
    uint16_t lowByte = cpu_read(registers.pc++);
    uint16_t highByte = cpu_read(registers.pc++);
    uint16_t address = (highByte << 8) | lowByte;
    instruction(&internal_mem[address + registers.y] - internal_mem);
}
void indirect_x(void (*instruction)(uint16_t)) {
    uint8_t address = cpu_read(registers.pc++);
    uint8_t zero_page_address = (address + registers.x) & 0xFF;
    uint16_t effective_address = cpu_read(zero_page_address) | (cpu_read((zero_page_address + 1) & 0xFF) << 8);
    instruction(&internal_mem[effective_address] - internal_mem);
}
void indirect_y(void (*instruction)(uint16_t)) {
    uint8_t address = cpu_read(registers.pc++);
    uint16_t effective_address = (cpu_read(address) | (cpu_read((address + 1) & 0xFF) << 8)) + registers.y;
    instruction(&internal_mem[effective_address] - internal_mem);
}