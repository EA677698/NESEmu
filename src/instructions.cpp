//
// Created by eafc6 on 7/1/2023.
//

#include "instructions.h"
#include "cpu.cpp"

//helper functions -------------------- NOT INSTRUCTIONS --------------------
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

bool is_bit_set(uint8_t operand, char bit){
    return (operand & (0x1 << bit)) >> bit;
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
    if(registers.sp == 0x0){
        registers.sp = 0xFF;
    } else{
        registers.sp--;
    }
}

void stack_increment(){
    if(registers.sp == 0xFF){
        registers.sp = 0x0;
    } else{
        registers.sp++;
    }
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

void asl(uint8_t &operand){
    if (is_bit_set(operand,7)) {
        sec();
    } else {
        clc();
    }
    operand = operand << 1;
    assign_zero_status(operand);
    assign_negative_status(operand);
}


void bcc(int8_t operand){
    if(!is_bit_set(registers.sr,0)){
        registers.pc += operand;
    }
}

void bcs(int8_t operand){
    if(is_bit_set(registers.sr,0)){
        registers.pc += operand;
    }
}

void beq(int8_t operand){
    if(is_bit_set(registers.sr,1)){
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
        registers.pc += operand;
    }
}

void bne(int8_t operand){
    if(!is_bit_set(registers.sr,1)){
        registers.pc += operand;
    }
}

void bpl(int8_t operand){
    if(!is_bit_set(registers.sr,7)){
        registers.pc += operand;
    }
}

void brk(){
    registers.pc += 2;
    php();
    uint8_t front = registers.pc >> 8;
    uint8_t back = registers.pc & 0xFF;
    stack_increment();
    internal_mem[0x100 + registers.sp] = back;
    stack_increment();
    internal_mem[0x100 + registers.sp] = front;
    registers.sr |= 0x10;
    uint16_t interrupt_vector = internal_mem[0xFFFE] | (internal_mem[0xFFFF] << 8);
    registers.pc = interrupt_vector;
}

void bvc(int8_t operand){
    if(!is_bit_set(registers.sr,6)){
        registers.pc += operand;
    }
}

void bvs(int8_t operand){
    if(is_bit_set(registers.sr,6)){
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
    uint16_t address = registers.pc + 3 - 1;
    uint8_t front = address>>8;
    uint8_t back = address & 0xFF;
    stack_decrement();
    internal_mem[0x100 + registers.sp] = back;
    stack_decrement();
    internal_mem[0x100 + registers.sp] = front;
    registers.pc = operand;
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

void lsr(uint8_t &operand){
    if(is_bit_set(operand,0)){
        sec();
    } else{
        clc();
    }
    operand = operand >> 1;
    assign_zero_status(operand);
    clear_negative_flag();
}

void nop(){}

void ora(uint8_t operand){
    registers.ac |= operand;
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}

void pha(){
    stack_decrement();
    internal_mem[0x100 + registers.sp] = registers.ac;
}

void php(){
    stack_decrement();
    internal_mem[0x100 + registers.sp] = registers.sr;
}

void pla(){
    stack_increment();
    registers.ac = internal_mem[0x100 + registers.sp];
    assign_zero_status(registers.ac);
    assign_negative_status(registers.ac);
}

void plp(){
    stack_increment();
    registers.sr &= 0x0;
    registers.sr |= internal_mem[0x100 + registers.sp];
}

void rol(uint8_t &operand){
    uint8_t old_carry = is_bit_set(registers.sr,7);
    registers.sr &= 0xFE | is_bit_set(operand,7);
    operand <<= 1;
    operand &= 0xFE | old_carry;
    assign_zero_status(operand);
    assign_negative_status(operand);
}

void ror(uint8_t &operand){
    uint8_t old_carry = is_bit_set(registers.sr,7);
    registers.sr &= 0xFE | is_bit_set(operand,0);
    operand >>= 1;
    operand &= 0x7F | (old_carry << 7);
    assign_zero_status(operand);
    assign_negative_status(operand);
}

void rti(){
    plp();
    stack_increment();
    uint16_t address = internal_mem[0x100 + registers.sp];
    address <<= 8;
    stack_increment();
    address |= internal_mem[0x100 + registers.sp];
    registers.pc = address;
}

void rts(){
    stack_increment();
    uint16_t address = internal_mem[0x100 + registers.sp];
    address <<= 8;
    stack_increment();
    address |= internal_mem[0x100 + registers.sp];
    registers.pc = address;
    registers.pc++;
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

void sta(uint8_t &operand){
    operand = registers.ac;
}

void stx(uint8_t &operand){
    operand = registers.x;
}

void sty(uint8_t &operand){
    operand = registers.y;
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


void accumulator(void (*instruction)(uint8_t)){instruction(registers.ac);}
void immediate(uint8_t operand, void (*instruction)(uint8_t) ){instruction(operand);}
void zero_page(uint8_t address, void (*instruction)(uint8_t) ){instruction(internal_mem[address]);}
void zero_page_x(uint8_t address, void (*instruction)(uint8_t) ){instruction(internal_mem[address + registers.x]);}
void absolute(uint16_t address, void (*instruction)(uint16_t) ){instruction(address);}
void absolute(uint16_t address, void (*instruction)(uint8_t) ){instruction(internal_mem[address]);}
void absolute_x(uint16_t address, void (*instruction)(uint8_t) ){instruction(internal_mem[address + registers.x]);}
void absolute_y(uint16_t address, void (*instruction)(uint8_t) ){instruction(internal_mem[address + registers.y]);}
void indirect(uint16_t address, void (*instruction)(uint16_t)){
    if(instruction != jmp){
        //TODO Throw invalid instruction error here!
        return;
    }
    instruction(internal_mem[address] | (internal_mem[(address & 0xFF00) | ((address + 1) & 0x00FF)] << 8));
}
void indirect_x(uint8_t address, void (*instruction)(uint8_t)) {
    uint8_t zero_page_address = (address + registers.x) & 0xFF;
    uint16_t effective_address = internal_mem[zero_page_address] | (internal_mem[(zero_page_address + 1) & 0xFF] << 8);
    instruction(internal_mem[effective_address]);
}
void indirect_y(uint8_t address, void (*instruction)(uint8_t)) {
    uint16_t effective_address = (internal_mem[address] | (internal_mem[(address + 1) & 0xFF] << 8)) + registers.y;
    instruction(internal_mem[effective_address]);
}
