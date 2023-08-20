//
// Created by eafc6 on 8/12/2023.
//

#include <iostream>
#include "cpu.h"
#include "instructions.h"
#include "spdlog/spdlog.h"

// low byte then high byte

void execute_opcode(int opcode){
    switch (opcode) {
        //ADC
        case 0x69:
            immediate(adc);
            break;
        case 0x65:
            zero_page(adc);
            break;
        case 0x75:
            zero_page_x(adc);
            break;
        case 0x6D:
            absolute(adc);
            break;
        case 0x7D:
            absolute_x(adc);
            break;
        case 0x79:
            absolute_y(adc);
            break;
        case 0x61:
            indirect_x(adc);
            break;
        case 0x71:
            indirect_y(adc);
            break;

        //AND
        case 0x29:
            immediate(logical_and);
            break;
        case 0x25:
            zero_page(logical_and);
            break;
        case 0x35:
            zero_page_x(logical_and);
            break;
        case 0x2D:
            absolute(logical_and);
            break;
        case 0x3D:
            absolute_x(logical_and);
            break;
        case 0x39:
            absolute_y(logical_and);
            break;
        case 0x21:
            indirect_x(logical_and);
            break;
        case 0x31:
            indirect_y(logical_and);
            break;

        //ASL
        case 0x0A:
            accumulator(asl);
            break;
        case 0x06:
            zero_page(asl);
            break;
        case 0x16:
            zero_page_x(asl);
            break;
        case 0x0E:
            absolute(asl);
            break;
        case 0x1E:
            absolute_x(asl);
            break;

        //BCC
        case 0x90:
            bcc(internal_mem[registers.pc++]);
            break;

        //BCS
        case 0xB0:
            bcs(internal_mem[registers.pc++]);

        //BEQ
        case 0xF0:
            beq(internal_mem[registers.pc++]);
            break;

        //BIT
        case 0x24:
            zero_page(bit);
            break;
        case 0x2C:
            absolute(bit);
            break;

        //BMI
        case 0x30:
            bmi(internal_mem[registers.pc++]);
            break;

        //BNE
        case 0xD0:
            bne(internal_mem[registers.pc++]);
            break;

        //BPL
        case 0x10:
            bpl(internal_mem[registers.pc++]);
            break;

        //BRK
        case 0x00:
            brk();
            break;

        //BVC
        case 0x50:
            bvc(internal_mem[registers.pc++]);
            break;

        //BVS
        case 0x70:
            bvs(internal_mem[registers.pc++]);
            break;

        //CLC
        case 0x18:
            clc();
            break;

        //CLD
        case 0xD8:
            cld();
            break;

        //CLI
        case 0x58:
            cli();
            break;

        //CLV
        case 0xB8:
            clv();
            break;

        //CMP
        case 0xC9:
            immediate(cmp);
            break;
        case 0xC5:
            zero_page(cmp);
            break;
        case 0xD5:
            zero_page_x(cmp);
            break;
        case 0xCD:
            absolute(cmp);
            break;
        case 0xDD:
            absolute_x(cmp);
            break;
        case 0xD9:
            absolute_y(cmp);
            break;
        case 0xC1:
            indirect_x(cmp);
            break;
        case 0xD1:
            indirect_y(cmp);
            break;

        //CPX
        case 0xE0:
            immediate(cpx);
            break;
        case 0xE4:
            zero_page(cpx);
            break;
        case 0xEC:
            absolute(cpx);
            break;

        //CPY
        case 0xC0:
            immediate(cpy);
            break;
        case 0xC4:
            zero_page(cpy);
            break;
        case 0xCC:
            absolute(cpy);
            break;

        //DEC
        case 0xC6:
            zero_page(dec);
            break;
        case 0xD6:
            zero_page_x(dec);
            break;
        case 0xCE:
            absolute(dec);
            break;
        case 0xDE:
            absolute_x(dec);
            break;

        //DEX
        case 0xCA:
            dex();
            break;

        //DEY
        case 0x88:
            dey();
            break;

        //EOR
        case 0x49:
            immediate(eor);
            break;
        case 0x45:
            zero_page(eor);
            break;
        case 0x55:
            zero_page_x(eor);
            break;
        case 0x4D:
            absolute(eor);
            break;
        case 0x5D:
            absolute_x(eor);
            break;
        case 0x59:
            absolute_y(eor);
            break;
        case 0x41:
            indirect_x(eor);
            break;
        case 0x51:
            indirect_y(eor);
            break;

        //INC
        case 0xE6:
            zero_page(inc);
            break;
        case 0xF6:
            zero_page_x(inc);
            break;
        case 0xEE:
            absolute(inc);
            break;
        case 0xFE:
            absolute_x(inc);
            break;

        //INX
        case 0xE8:
            inx();
            break;

        //INY
        case 0xC8:
            iny();
            break;

        //JMP
        case 0x4C:
            absolute(jmp);
            break;
        case 0x6C:
            indirect(jmp);
            break;

        //JSR
        case 0x20:
            absolute(jsr);
            break;

        //LDA
        case 0xA9:
            immediate(lda);
            break;
        case 0xA5:
            zero_page(lda);
            break;
        case 0xB5:
            zero_page_x(lda);
            break;
        case 0xAD:
            absolute(lda);
        case 0xBD:
            absolute_x(lda);
            break;
        case 0xB9:
            absolute_y(lda);
            break;
        case 0xA1:
            indirect_x(lda);
            break;
        case 0xB1:
            indirect_y(lda);
            break;

        //LDX
        case 0xA2:
            immediate(ldx);
            break;
        case 0xA6:
            zero_page(ldx);
            break;
        case 0xB6:
            zero_page_y(ldx);
            break;
        case 0xAE:
            absolute(ldx);
            break;
        case 0xBE:
            absolute_y(ldx);
            break;

        //LDY
        case 0xA0:
            immediate(ldy);
            break;
        case 0xA4:
            zero_page(ldy);
            break;
        case 0xB4:
            zero_page_x(ldy);
            break;
        case 0xAC:
            absolute(ldy);
            break;
        case 0xBC:
            absolute_x(ldy);
            break;

        //LSR
        case 0x4A:
            accumulator(lsr);
            break;
        case 0x46:
            zero_page(lsr);
            break;
        case 0x56:
            zero_page_x(lsr);
            break;
        case 0x4E:
            absolute(lsr);
            break;
        case 0x5E:
            absolute_x(lsr);
            break;

        //NOP
        case 0xEA:
            nop();
            break;

        //ORA
        case 0x09:
            immediate(ora);
            break;
        case 0x05:
            zero_page(ora);
            break;
        case 0x15:
            zero_page_x(ora);
            break;
        case 0x0D:
            absolute(ora);
            break;
        case 0x1D:
            absolute_x(ora);
            break;
        case 0x19:
            absolute_y(ora);
            break;
        case 0x01:
            indirect_x(ora);
            break;
        case 0x11:
            indirect_y(ora);
            break;

        //PHA
        case 0x48:
            pha();
            break;

        //PHP
        case 0x08:
            php();
            break;

        //PLA
        case 0x68:
            pla();
            break;

        //PLP
        case 0x28:
            plp();
            break;

        //ROL
        case 0x2A:
            accumulator(rol);
            break;
        case 0x26:
            zero_page(rol);
            break;
        case 0x36:
            zero_page_x(rol);
            break;
        case 0x2E:
            absolute(rol);
            break;
        case 0x3E:
            absolute_x(rol);
            break;

        //ROR
        case 0x6A:
            accumulator(ror);
            break;
        case 0x66:
            zero_page(ror);
            break;
        case 0x76:
            zero_page_x(ror);
            break;
        case 0x6E:
            absolute(ror);
            break;
        case 0x7E:
            absolute_x(ror);
            break;

        //RTI
        case 0x40:
            rti();
            break;

        //RTS
        case 0x60:
            rts();
            break;

        //SBC
        case 0xE9:
            immediate(sbc);
            break;
        case 0xE5:
            zero_page(sbc);
            break;
        case 0xF5:
            zero_page_x(sbc);
            break;
        case 0xED:
            absolute(sbc);
            break;
        case 0xFD:
            absolute_x(sbc);
            break;
        case 0xF9:
            absolute_y(sbc);
            break;
        case 0xE1:
            indirect_x(sbc);
            break;
        case 0xF1:
            indirect_y(sbc);
            break;

        //SEC
        case 0x38:
            sec();
            break;

        //SED
        case 0xF8:
            sed();
            break;

        //SEI
        case 0x78:
            sei();
            break;

        //STA
        case 0x85:
            zero_page(sta);
            break;
        case 0x95:
            zero_page_x(sta);
            break;
        case 0x8D:
            absolute(sta);
            break;
        case 0x9D:
            absolute_x(sta);
            break;
        case 0x99:
            absolute_y(sta);
            break;
        case 0x81:
            indirect_x(sta);
            break;
        case 0x91:
            indirect_y(sta);
            break;

        //STX
        case 0x86:
            zero_page(stx);
            break;
        case 0x96:
            zero_page_y(stx);
            break;
        case 0x8E:
            absolute(stx);
            break;

        //STY
        case 0x84:
            zero_page(sty);
            break;
        case 0x94:
            zero_page_x(sty);
            break;
        case 0x8C:
            absolute(sty);
            break;

        //TAX
        case 0xAA:
            tax();
            break;

        //TAY
        case 0xA8:
            tay();
            break;

        //TSX
        case 0xBA:
            tsx();
            break;

        //TXA
        case 0x8A:
            txa();
            break;

        //TXS
        case 0x9A:
            txs();
            break;

        //TYA
        case 0x98:
            tya();
            break;

        default:
            //TODO THROW ERROR
            spdlog::critical("INVALID INSTRUCTION. OPCODE: 0x{:X}",opcode);
            spdlog::critical("Halting CPU...");
            exit(1);
    }

}

