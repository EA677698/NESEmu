//
// Created by eafc6 on 7/1/2023.
//

#include <iostream>
#include "cpu.h"

//helper functions -------------------- NOT INSTRUCTIONS --------------------
bool is_page_crossed(uint16_t initial_address, uint16_t change){
    return (initial_address & 0xFF00) != ((initial_address + change) & 0xFF00);
}

void CPU::clear_negative_flag(){
    registers.sr &= 0x7F;
}

void CPU::clear_zero_flag(){
    registers.sr &= 0xFD;
}

void CPU::set_zero_flag(){
    registers.sr |= 0x2;
}

void CPU::set_negative_flag(){
    registers.sr |= 0x80;
}

void CPU::set_overflow_flag(){
    registers.sr |= (1 << 6);
}

void CPU::assign_zero_status(uint8_t operand){
    if(!operand){
        set_zero_flag();
    } else{
        clear_zero_flag();
    }
}

void CPU::assign_negative_status(uint8_t operand){
    if(is_bit_set(operand,7)){
        set_negative_flag();
    } else{
        clear_negative_flag();
    }
}

void CPU::stack_decrement(){
    registers.sp--;
}

void CPU::stack_increment(){
    registers.sp++;
}

void CPU::push(uint8_t operand) {
    write(0x100 + registers.sp, operand);
    stack_decrement();
}

uint8_t CPU::pop() {
    stack_increment();
    uint8_t value = read(0x100 + registers.sp);
    return value;

}


// ----------------------------------------- DEFINED INSTRUCTIONS -----------------------------------------

void CPU::sec(){
    registers.sr |= 0x1;
}

void CPU::clc(){
    registers.sr &= 0xFE;
}

void CPU::sed(){
    registers.sr |= 0x8;
}

void CPU::cld(){
    registers.sr &= 0xF7;
}

void CPU::sei(){
    registers.sr |= 0x4;
}

void CPU::cli(){
    registers.sr &= 0xFB;
}

void CPU::clv(){
    registers.sr &= 0xBF;
}

void CPU::adc(uint8_t operand){
    uint16_t temp = registers.ac + operand + is_bit_set(registers.sr, 0);
    if(temp > 0xFF){
        sec();
    } else {
        clc();
    }
    uint8_t overflow = ((registers.ac ^ operand) & 0x80) == 0 && ((registers.ac ^ temp) & 0x80) != 0;
    if (overflow) {
        set_overflow_flag();
    } else {
        clv();
    }
    registers.ac = temp & 0xFF;
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}


void CPU::logical_and(uint8_t operand){
    registers.ac &= operand;
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}

void CPU::asl(uint16_t address){
    uint8_t operand = read(address);
    if (is_bit_set(operand,7)) {
        sec();
    } else {
        clc();
    }
    write(address, operand <<= 1);
    assign_zero_status(operand);
    assign_negative_status(operand);
    rw_register_mode = 0x0;
}


void CPU::bcc(int8_t operand){
    if(!is_bit_set(registers.sr, 0)){
        cycles += (1 + is_page_crossed(registers.pc, operand));
        registers.pc += operand;
    }
}

void CPU::bcs(int8_t operand){
    if(is_bit_set(registers.sr, 0)){
        uint16_t old_pc = registers.pc;
        registers.pc += operand;
        cycles += (1 + is_page_crossed(old_pc, operand));

    }
}

void CPU::beq(int8_t operand){
    if(is_bit_set(registers.sr, 1)){
        cycles += (1 + is_page_crossed(registers.pc, operand));
        registers.pc += operand;
    }
}

void CPU::bit(uint8_t operand){
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

void CPU::bmi(int8_t operand){
    if(is_bit_set(registers.sr, 7)){
        cycles += (1 + is_page_crossed(registers.pc, operand));
        registers.pc += operand;
    }
}

void CPU::bne(int8_t operand){
    if(!is_bit_set(registers.sr, 1)){
        cycles += (1 + is_page_crossed(registers.pc, operand));
        registers.pc += operand;
    }
}

void CPU::bpl(int8_t operand){
    if(!is_bit_set(registers.sr, 7)){
        cycles += (1 + is_page_crossed(registers.pc, operand));
        registers.pc += operand;
    }
}

void CPU::brk(){
    registers.pc++;
    uint8_t front = registers.pc >> 8;
    uint8_t back = registers.pc & 0xFF;
    push(front);
    push(back);
    uint8_t status = registers.sr | 0x30;
    push(status);
    uint16_t interrupt_vector = read(0xFFFE) | ((uint16_t) (read(0xFFFF) << 8));
    registers.pc = interrupt_vector;
    spdlog::debug("SETTING PC COUNTER TO INTERRUPT VECTOR: 0x{:X}",interrupt_vector);
}

void CPU::bvc(int8_t operand){
    if(!is_bit_set(registers.sr, 6)){
        cycles += (1 + is_page_crossed(registers.pc, operand));
        registers.pc += operand;
    }
}

void CPU::bvs(int8_t operand){
    if(is_bit_set(registers.sr, 6)){
        cycles += (1 + is_page_crossed(registers.pc, operand));
        registers.pc += operand;
    }
}

void CPU::cmp(uint8_t operand){
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

void CPU::cpx(uint8_t operand){
    uint16_t result = registers.x - operand;
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
    assign_negative_status(result & 0xFF);
}

void CPU::cpy(uint8_t operand){
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

void CPU::dec(uint16_t operand){
    uint8_t res = read(operand);
    res--;
    write(operand,res);
    assign_zero_status(res);
    assign_negative_status(res);
}

void CPU::dex(){
    registers.x--;
    assign_zero_status(registers.x);
    assign_negative_status(registers.x);
}

void CPU::dey(){
    registers.y--;
    assign_zero_status(registers.y);
    assign_negative_status(registers.y);
}

void CPU::eor(uint8_t operand){
    registers.ac ^= operand;
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}

void CPU::inc(uint16_t operand){
    uint8_t res = read(operand);
    res++;
    write(operand,res);
    assign_zero_status(res);
    assign_negative_status(res);
}

void CPU::inx(){
    registers.x++;
    assign_zero_status(registers.x);
    assign_negative_status(registers.x);
}

void CPU::iny(){
    registers.y++;
    assign_zero_status(registers.y);
    assign_negative_status(registers.y);
}

void CPU::jmp(uint16_t address){
    registers.pc = address;
}

void CPU::jsr(uint16_t operand){
    registers.pc--;
    uint8_t front = registers.pc >> 8;
    uint8_t back = registers.pc & 0xFF;
    //spdlog::info("Storing address 0x{:X} into stack", registers.pc);
    push(front);
    push(back);
    registers.pc = operand;
    rw_register_mode = 0x0;
}

void CPU::lda(uint8_t operand){
    registers.ac = operand;
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}

void CPU::ldx(uint8_t operand){
    registers.x = operand;
    assign_zero_status(registers.x);
    assign_negative_status(registers.x);
}

void CPU::ldy(uint8_t operand){
    registers.y = operand;
    assign_zero_status(registers.y);
    assign_negative_status(registers.y);
}

void CPU::lsr(uint16_t address){
    uint8_t operand = read(address);
    if(is_bit_set(operand,0)){
        sec();
    } else{
        clc();
    }
    write(address, operand >>= 1);
    assign_zero_status(operand);
    clear_negative_flag();
}

void CPU::nop(){}

void CPU::nop(uint8_t operand){}

void CPU::ora(uint8_t operand){
    registers.ac |= operand;
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}

void CPU::pha(){
    push(registers.ac);
}

void CPU::php(){
    push(registers.sr | 0x10);
}

void CPU::pla(){
    registers.ac = pop();
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}

void CPU::plp(){
    registers.sr &= 0x0;
    registers.sr |= pop();
    registers.sr &= 0xEF;
    registers.sr |= 0x20;
}

void CPU::rol(uint16_t address){
    uint8_t operand = read(address);
    uint8_t old_carry = is_bit_set(registers.sr, 0);
    clc();
    if(is_bit_set(operand, 7)) {
        sec();
    }
    operand <<= 1;
    operand |= old_carry;
    write(address, operand);
    assign_zero_status(operand);
    assign_negative_status(operand);
    rw_register_mode = 0x0;
}

void CPU::ror(uint16_t address){
    uint8_t operand = read(address);
    uint8_t old_carry = is_bit_set(registers.sr, 0);
    clc();
    if(is_bit_set(operand, 0)) {
        sec();
    }
    operand >>= 1;
    operand |= (old_carry << 7);
    write(address, operand);
    assign_zero_status(operand);
    assign_negative_status(operand);
    rw_register_mode = 0x0;
}

void CPU::rti(){
    registers.pc++;
    uint8_t status = pop();
    registers.sr = status;
    uint16_t address = pop();
    address |= pop() << 8;
    registers.pc = address;
}

void CPU::rts(){
    registers.pc++;
    uint16_t address = pop();
    address |= pop() << 8;
    //spdlog::info("Retrieving address from stack: 0x{:X}",address);
    registers.pc = address + 1;
}

void CPU::sbc(uint8_t operand){
    uint16_t temp = registers.ac - operand - (1 - is_bit_set(registers.sr, 0));
    if(temp <= 0xFF){
        sec();

    } else{
        clc();
    }

    uint8_t overflow = ((registers.ac ^ temp) & 0x80) && ((registers.ac ^ operand) & 0x80);
    if(overflow) {
        set_overflow_flag();
    } else {
        clv();
    }

    registers.ac = temp & 0xFF;
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}

void CPU::sta(uint16_t address){
    write(address, registers.ac);
    rw_register_mode = 0x0;
}

void CPU::stx(uint16_t address){
    write(address, registers.x);
}

void CPU::sty(uint16_t address){
    write(address, registers.y);
}

void CPU::tax(){
    registers.x = registers.ac;
    assign_zero_status(registers.x);
    assign_negative_status(registers.x);
}

void CPU::tay(){
    registers.y = registers.ac;
    assign_zero_status(registers.y);
    assign_negative_status(registers.y);
}

void CPU::tsx(){
    registers.x = registers.sp;
    assign_zero_status(registers.x);
    assign_negative_status(registers.x);
}

void CPU::txa(){
    registers.ac = registers.x;
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}

void CPU::txs(){
    registers.sp = registers.x;
}

void CPU::tya(){
    registers.ac = registers.y;
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}

// ----------------------- UNOFFICIAL OPCODES -----------------------

void CPU::alr(uint16_t operand) {
    uint8_t value = read(operand);
    registers.ac &= value;
    rw_register_mode = 0x1;
    lsr(AC_ADDRESS);
    rw_register_mode = 0x0;
}

void CPU::anc(uint8_t operand) {
    registers.ac &= operand;
    if(is_bit_set(registers.ac, 7)){
        sec();
    } else{
        clc();
    }
    assign_negative_status(registers.ac);
    assign_zero_status(registers.ac);
}

void CPU::anc2(uint8_t operand) {
    anc(operand);
}

void CPU::ane(uint8_t operand) {
    uint8_t value = registers.ac | 0x0;
    value &= registers.x;
    value &= operand;
    registers.ac = value;
    assign_negative_status(registers.ac);
    assign_zero_status(registers.ac);
}

void CPU::arr(uint8_t operand) {
    registers.ac &= operand;
    uint8_t newCarry = is_bit_set(registers.ac, 0);
    registers.ac = (registers.ac >> 1) | (is_bit_set(registers.sr, 0) << 7);
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
    registers.sr = (registers.sr & 0xFE) | (newCarry ? 1 : 0);
    uint8_t bit5 = is_bit_set(registers.ac, 5);
    uint8_t bit6 = is_bit_set(registers.ac, 6);
    if (bit5 ^ bit6) {
        registers.sr |= (1 << 6);
    } else {
        registers.sr &= ~(1 << 6);
    }
}


void CPU::dcp(uint16_t operand) {
    uint8_t value = read(operand);
    value--;
    write(operand, value);
    cmp(value);
}

void CPU::isc(uint16_t operand) {
    uint8_t value = read(operand);
    value++;
    write(operand, value);
    sbc(value);
}

void CPU::las(uint16_t operand) {
    registers.ac = read(operand) & registers.sp;
    registers.x = registers.ac;
    registers.sp = registers.ac;
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}

void CPU::lax(uint8_t operand) {
    registers.ac = operand;
    registers.x = operand;
    assign_zero_status(operand);
    assign_negative_status(operand);
}

void CPU::lxa(uint8_t operand) {
    uint8_t value = (registers.ac | 0x0) & operand;
    registers.ac = value;
    registers.x = value;
    assign_negative_status(registers.ac);
    assign_zero_status(registers.ac);

}

void CPU::rla(uint16_t operand) {
    rol(operand);
    uint8_t value = read(operand);
    registers.ac &= value;
    assign_negative_status(registers.ac);
    assign_zero_status(registers.ac);
}

void CPU::rra(uint16_t operand) {
    ror(operand);
    uint8_t value = read(operand);
    adc(value);
}

void CPU::sax(uint16_t operand) {
    write(operand, registers.ac & registers.x);
}

void CPU::sbx(uint8_t operand) {
    uint16_t value = registers.x & registers.ac;
    value -= operand;
    registers.x = value & 0xFF;
    if (value >= operand) {
        sec();
    } else {
        clc();
    }
    if (registers.x == operand) {
        set_zero_flag();
    } else {
        clear_zero_flag();
    }
    assign_negative_status(registers.x);
}

void CPU::sha(uint16_t operand) {
    uint8_t highBytePlusOne = ((operand >> 8) & 0xFF) + 1;
    uint8_t value = registers.ac & registers.x & highBytePlusOne;
    write(operand, value);
}

void CPU::shx(uint16_t operand) {
    uint8_t highBytePlusOne = ((operand >> 8) & 0xFF) + 1;
    uint8_t value = registers.x & highBytePlusOne;
    write(operand, value);
}

void CPU::shy(uint16_t operand) {
    uint8_t highBytePlusOne = ((operand >> 8) & 0xFF) + 1;
    uint8_t value = registers.y & highBytePlusOne;
    write(operand, value);
}

void CPU::slo(uint16_t operand) {
    uint8_t value = read(operand);
    uint8_t original = value;
    value <<= 1;
    write(operand, value);
    registers.ac |= value;
    if(is_bit_set(original, 7)){
        sec();
    } else{
        clc();
    }
    assign_zero_status(registers.ac); // Update zero flag based on A.
    assign_negative_status(registers.ac); // Update negative flag based on A.
}

void CPU::sre(uint16_t operand) {
    uint8_t value = read(operand);
    uint8_t original = value;
    value >>= 1;
    write(operand, value);
    registers.ac ^= value;
    if(is_bit_set(original, 0)){
        sec();
    } else{
        clc();
    }
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}

void CPU::tas(uint16_t operand) {
    uint8_t highBytePlusOne = ((operand >> 8) & 0xFF) + 1;
    registers.sp = registers.ac & registers.x;
    write(operand, registers.sp & highBytePlusOne);
}

void CPU::usbc(uint8_t operand) {
    sbc(operand);
    nop();
}


// Memory Addressing Modes

void CPU::accumulator(void (CPU::*instruction)(uint8_t)) {
    (this->*instruction)(registers.ac);
}
void CPU::immediate(void (CPU::*instruction)(uint8_t)) {
    uint8_t operand = read(registers.pc++);
    (this->*instruction)(this->current_operand = operand);
}
void CPU::zero_page(void (CPU::*instruction)(uint8_t)) {
    uint8_t operand = read(registers.pc++);
    this->current_operand = operand;
    (this->*instruction)(read(operand));
}
void CPU::zero_page_x(void (CPU::*instruction)(uint8_t)) {
    uint8_t operand = read(registers.pc++);
    this->current_operand = operand;
    uint8_t address = operand + registers.x;
    (this->*instruction)(read(address));
}
void CPU::zero_page_y(void (CPU::*instruction)(uint8_t)) {
    uint8_t operand = read(registers.pc++);
    this->current_operand = operand;
    uint8_t address = operand + registers.y;
    (this->*instruction)(read(address));
}
void CPU::absolute(void (CPU::*instruction)(uint16_t)) {
    uint16_t lowByte = read(registers.pc++);
    uint16_t highByte = read(registers.pc++);
    uint16_t address = (highByte << 8) | lowByte;
    (this->*instruction)(this->current_operand = address);
}
void CPU::absolute(void (CPU::*instruction)(uint8_t)) {
    uint16_t lowByte = read(registers.pc++);
    uint16_t highByte = read(registers.pc++);
    uint16_t address = (highByte << 8) | lowByte;
    this->current_operand = address;
    (this->*instruction)(read(address));
}
bool CPU::absolute_x(void (CPU::*instruction)(uint8_t)) {
    uint16_t lowByte = read(registers.pc++);
    uint16_t highByte = read(registers.pc++);
    uint16_t address = (highByte << 8) | lowByte;
    this->current_operand = address;
    (this->*instruction)(read(address + registers.x));
    return is_page_crossed(address, registers.x);
}
bool CPU::absolute_y(void (CPU::*instruction)(uint8_t)){
    uint16_t lowByte = read(registers.pc++);
    uint16_t highByte = read(registers.pc++);
    uint16_t address = (highByte << 8) | lowByte;
    this->current_operand = address;
    (this->*instruction)(read(address + registers.y));
    return is_page_crossed(address, registers.y);
}
void CPU::indirect(void (CPU::*instruction)(uint16_t)){
    uint16_t lowByte = read(registers.pc++);
    uint16_t highByte = read(registers.pc++);
    uint16_t address = (highByte << 8) | lowByte;
    if(instruction != &CPU::jmp){
        //TODO throw invalid instruction error
        spdlog::critical("INVALID ADDRESSING MODE FOR JMP INSTRUCTION (INDIRECT ADDRESSING MODE)");
        exit(1);
    }
    this->current_operand = address;
    (this->*instruction)(read(address) | (read((address & 0xFF00) | ((address + 1) & 0x00FF)) << 8));
}
void CPU::indirect_x(void (CPU::*instruction)(uint8_t)) {
    uint8_t address = read(registers.pc++);
    uint8_t zero_page_address = (address + registers.x) & 0xFF;
    uint16_t effective_address = read(zero_page_address) | (read((zero_page_address + 1) & 0xFF) << 8);
    this->current_operand = address;
    (this->*instruction)(read(effective_address));
}
bool CPU::indirect_y(void (CPU::*instruction)(uint8_t)) {
    uint8_t address = read(registers.pc++);
    uint16_t effective_address = (read(address) | (read((address + 1) & 0xFF) << 8)) + registers.y;
    this->current_operand = address;
    (this->*instruction)(read(effective_address));
    return is_page_crossed(registers.pc - 1, effective_address);
}

void CPU::accumulator(void (CPU::*instruction)(uint16_t)) {
    rw_register_mode = 0x1;
    (this->*instruction)(AC_ADDRESS);
    rw_register_mode = 0x0;
}
void CPU::immediate(void (CPU::*instruction)(uint16_t)) {
    this->current_operand = read(registers.pc);
    (this->*instruction)(registers.pc++);
}
void CPU::zero_page(void (CPU::*instruction)(uint16_t)) {
    uint8_t operand = read(registers.pc++);
    this->current_operand = operand;
    (this->*instruction)(operand);
}
void CPU::zero_page_x(void (CPU::*instruction)(uint16_t)) {
    uint8_t operand = read(registers.pc++);
    this->current_operand = operand;
    uint8_t address = operand + registers.x;
    (this->*instruction)(address);
}
void CPU::zero_page_y(void (CPU::*instruction)(uint16_t)) {
    uint8_t operand = read(registers.pc++);
    this->current_operand = operand;
    uint8_t address = operand + registers.y;
    (this->*instruction)(address);
}

/*
 * Unused supposedly...
 *
void absolute(void (CPU::*instruction)(uint8_t &)) {
    uint16_t lowByte = read(registers.pc++);
    uint16_t highByte = read(registers.pc++);
    uint16_t address = (highByte << 8) | lowByte;
    instruction(read(address));
}
*/

void CPU::absolute_x(void (CPU::*instruction)(uint16_t)) {
    uint16_t lowByte = read(registers.pc++);
    uint16_t highByte = read(registers.pc++);
    uint16_t address = (highByte << 8) | lowByte;
    this->current_operand = address;
    (this->*instruction)(address + registers.x);
}
void CPU::absolute_y(void (CPU::*instruction)(uint16_t)){
    uint16_t lowByte = read(registers.pc++);
    uint16_t highByte = read(registers.pc++);
    uint16_t address = (highByte << 8) | lowByte;
    this->current_operand = address;
    (this->*instruction)(address + registers.y);
}
void CPU::indirect_x(void (CPU::*instruction)(uint16_t)) {
    uint8_t address = read(registers.pc++);
    uint8_t zero_page_address = (address + registers.x) & 0xFF;
    uint16_t effective_address = read(zero_page_address) | (read((zero_page_address + 1) & 0xFF) << 8);
    this->current_operand = address;
    (this->*instruction)(effective_address);
}
void CPU::indirect_y(void (CPU::*instruction)(uint16_t)) {
    uint8_t address = read(registers.pc++);
    uint16_t effective_address = (read(address) | (read((address + 1) & 0xFF) << 8)) + registers.y;
    this->current_operand = address;
    (this->*instruction)(effective_address);
}