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
            cpu_registers.cycles += 2;
            break;
        case 0x65:
            zero_page(adc);
            cpu_registers.cycles += 3;
            break;
        case 0x75:
            zero_page_x(adc);
            cpu_registers.cycles += 4;
            break;
        case 0x6D:
            absolute(adc);
            cpu_registers.cycles += 4;
            break;
        case 0x7D:
            cpu_registers.cycles += (4 + absolute_x(adc));
            //TODO special cycle case
            break;
        case 0x79:
            cpu_registers.cycles += (4 + absolute_y(adc));
            //TODO special cycle case
            break;
        case 0x61:
            indirect_x(adc);
            cpu_registers.cycles += 6;
            break;
        case 0x71:
            cpu_registers.cycles += (5 + indirect_y(adc));
            //TODO special cycle case
            break;

        //AND
        case 0x29:
            immediate(logical_and);
            cpu_registers.cycles += 2;
            break;
        case 0x25:
            zero_page(logical_and);
            cpu_registers.cycles += 3;
            break;
        case 0x35:
            zero_page_x(logical_and);
            cpu_registers.cycles += 4;
            break;
        case 0x2D:
            absolute(logical_and);
            cpu_registers.cycles += 4;
            break;
        case 0x3D:
            cpu_registers.cycles += (4 + absolute_x(logical_and));
            //TODO special cycle case
            break;
        case 0x39:
            cpu_registers.cycles += (4 + absolute_y(logical_and));
            //TODO special cycle case
            break;
        case 0x21:
            indirect_x(logical_and);
            cpu_registers.cycles += 6;
            break;
        case 0x31:
            cpu_registers.cycles += (5 + indirect_y(logical_and));
            //TODO special cycle case
            break;

        //ASL
        case 0x0A:
            accumulator(asl);
            cpu_registers.cycles += 2;
            break;
        case 0x06:
            zero_page(asl);
            cpu_registers.cycles += 5;
            break;
        case 0x16:
            zero_page_x(asl);
            cpu_registers.cycles += 6;
            break;
        case 0x0E:
            absolute(asl);
            cpu_registers.cycles += 6;
            break;
        case 0x1E:
            absolute_x(asl);
            cpu_registers.cycles += 7;
            break;

        //BCC
        case 0x90:
            bcc(cpu_mem[cpu_registers.pc++]);
            //TODO special cycle case
            cpu_registers.cycles += 2;
            break;

        //BCS
        case 0xB0:
            bcs(cpu_mem[cpu_registers.pc++]);
            //TODO special cycle case
            cpu_registers.cycles += 2;
            break;

        //BEQ
        case 0xF0:
            beq(cpu_mem[cpu_registers.pc++]);
            //TODO special cycle case
            cpu_registers.cycles += 2;
            break;

        //BIT
        case 0x24:
            zero_page(bit);
            cpu_registers.cycles += 3;
            break;
        case 0x2C:
            absolute(bit);
            cpu_registers.cycles += 4;
            break;

        //BMI
        case 0x30:
            bmi(cpu_mem[cpu_registers.pc++]);
            cpu_registers.cycles += 2;
            //TODO special cycle case
            break;

        //BNE
        case 0xD0:
            bne(cpu_mem[cpu_registers.pc++]);
            cpu_registers.cycles += 2;
            //TODO special cycle case
            break;

        //BPL
        case 0x10:
            bpl(cpu_mem[cpu_registers.pc++]);
            cpu_registers.cycles += 2;
            //TODO special cycle case
            break;

        //BRK
        case 0x00:
            brk();
            cpu_registers.cycles += 7;
            break;

        //BVC
        case 0x50:
            bvc(cpu_mem[cpu_registers.pc++]);
            cpu_registers.cycles += 2;
            //TODO special cycle case
            break;

        //BVS
        case 0x70:
            bvs(cpu_mem[cpu_registers.pc++]);
            cpu_registers.cycles += 2;
            //TODO special cycle case
            break;

        //CLC
        case 0x18:
            clc();
            cpu_registers.cycles += 2;
            break;

        //CLD
        case 0xD8:
            cld();
            cpu_registers.cycles += 2;
            break;

        //CLI
        case 0x58:
            cli();
            cpu_registers.cycles += 2;
            break;

        //CLV
        case 0xB8:
            clv();
            cpu_registers.cycles += 2;
            break;

        //CMP
        case 0xC9:
            immediate(cmp);
            cpu_registers.cycles += 2;
            break;
        case 0xC5:
            zero_page(cmp);
            cpu_registers.cycles += 3;
            break;
        case 0xD5:
            zero_page_x(cmp);
            cpu_registers.cycles += 4;
            break;
        case 0xCD:
            absolute(cmp);
            cpu_registers.cycles += 4;
            break;
        case 0xDD:
            cpu_registers.cycles += (4 + absolute_x(cmp));
            //TODO special cycle case
            break;
        case 0xD9:
            cpu_registers.cycles += (4 + absolute_y(cmp));
            //TODO special cycle case
            break;
        case 0xC1:
            indirect_x(cmp);
            cpu_registers.cycles += 6;
            break;
        case 0xD1:
            cpu_registers.cycles += (5 + indirect_y(cmp));
            //TODO special cycle case
            break;

        //CPX
        case 0xE0:
            immediate(cpx);
            cpu_registers.cycles += 2;
            break;
        case 0xE4:
            zero_page(cpx);
            cpu_registers.cycles += 3;
            break;
        case 0xEC:
            absolute(cpx);
            cpu_registers.cycles += 4;
            break;

        //CPY
        case 0xC0:
            immediate(cpy);
            cpu_registers.cycles += 2;
            break;
        case 0xC4:
            zero_page(cpy);
            cpu_registers.cycles += 3;
            break;
        case 0xCC:
            absolute(cpy);
            cpu_registers.cycles += 4;
            break;

        //DEC
        case 0xC6:
            zero_page(dec);
            cpu_registers.cycles += 5;
            break;
        case 0xD6:
            zero_page_x(dec);
            cpu_registers.cycles += 6;
            break;
        case 0xCE:
            absolute(dec);
            cpu_registers.cycles += 6;
            break;
        case 0xDE:
            absolute_x(dec);
            cpu_registers.cycles += 7;
            break;

        //DEX
        case 0xCA:
            dex();
            cpu_registers.cycles += 2;
            break;

        //DEY
        case 0x88:
            dey();
            cpu_registers.cycles += 2;
            break;

        //EOR
        case 0x49:
            immediate(eor);
            cpu_registers.cycles += 2;
            break;
        case 0x45:
            zero_page(eor);
            cpu_registers.cycles += 3;
            break;
        case 0x55:
            zero_page_x(eor);
            cpu_registers.cycles += 4;
            break;
        case 0x4D:
            absolute(eor);
            cpu_registers.cycles += 4;
            break;
        case 0x5D:
            cpu_registers.cycles += (4 + absolute_x(eor));
            //TODO special cycle case
            break;
        case 0x59:
            cpu_registers.cycles += (4 + absolute_y(eor));
            //TODO special cycle case
            break;
        case 0x41:
            indirect_x(eor);
            cpu_registers.cycles += 6;
            break;
        case 0x51:
            cpu_registers.cycles += (5 + indirect_y(eor));
            //TODO special cycle case
            break;

        //INC
        case 0xE6:
            zero_page(inc);
            cpu_registers.cycles += 5;
            break;
        case 0xF6:
            zero_page_x(inc);
            cpu_registers.cycles += 6;
            break;
        case 0xEE:
            absolute(inc);
            cpu_registers.cycles += 6;
            break;
        case 0xFE:
            absolute_x(inc);
            cpu_registers.cycles += 7;
            break;

        //INX
        case 0xE8:
            inx();
            cpu_registers.cycles += 2;
            break;

        //INY
        case 0xC8:
            iny();
            cpu_registers.cycles += 2;
            break;

        //JMP
        case 0x4C:
            absolute(jmp);
            cpu_registers.cycles += 3;
            break;
        case 0x6C:
            indirect(jmp);
            cpu_registers.cycles += 5;
            break;

        //JSR
        case 0x20:
            absolute(jsr);
            cpu_registers.cycles += 6;
            break;

        //LDA
        case 0xA9:
            immediate(lda);
            cpu_registers.cycles += 2;
            break;
        case 0xA5:
            zero_page(lda);
            cpu_registers.cycles += 3;
            break;
        case 0xB5:
            zero_page_x(lda);
            cpu_registers.cycles += 4;
            break;
        case 0xAD:
            absolute(lda);
            cpu_registers.cycles += 4;
            break;
        case 0xBD:
            cpu_registers.cycles += (4 + absolute_x(lda));
            //TODO special cycle case
            break;
        case 0xB9:
            cpu_registers.cycles += (4 + absolute_y(lda));
            //TODO special cycle case
            break;
        case 0xA1:
            indirect_x(lda);
            cpu_registers.cycles += 6;
            break;
        case 0xB1:
            cpu_registers.cycles += (5 + indirect_y(lda));
            //TODO special cycle case
            break;

        //LDX
        case 0xA2:
            immediate(ldx);
            cpu_registers.cycles += 2;
            break;
        case 0xA6:
            zero_page(ldx);
            cpu_registers.cycles += 3;
            break;
        case 0xB6:
            zero_page_y(ldx);
            cpu_registers.cycles += 4;
            break;
        case 0xAE:
            absolute(ldx);
            cpu_registers.cycles += 4;
            break;
        case 0xBE:
            cpu_registers.cycles += (4 + absolute_y(ldx));
            //TODO special cycle case
            break;

        //LDY
        case 0xA0:
            immediate(ldy);
            cpu_registers.cycles += 2;
            break;
        case 0xA4:
            zero_page(ldy);
            cpu_registers.cycles += 3;
            break;
        case 0xB4:
            zero_page_x(ldy);
            cpu_registers.cycles += 4;
            break;
        case 0xAC:
            absolute(ldy);
            cpu_registers.cycles += 4;
            break;
        case 0xBC:
            cpu_registers.cycles += (4 + absolute_x(ldy));
            //TODO special cycle case
            break;

        //LSR
        case 0x4A:
            accumulator(lsr);
            cpu_registers.cycles += 2;
            break;
        case 0x46:
            zero_page(lsr);
            cpu_registers.cycles += 5;
            break;
        case 0x56:
            zero_page_x(lsr);
            cpu_registers.cycles += 6;
            break;
        case 0x4E:
            absolute(lsr);
            cpu_registers.cycles += 6;
            break;
        case 0x5E:
            absolute_x(lsr);
            cpu_registers.cycles += 7;
            break;

        //NOP
        case 0xEA:
            nop();
            cpu_registers.cycles += 2;
            break;

        //ORA
        case 0x09:
            immediate(ora);
            cpu_registers.cycles += 2;
            break;
        case 0x05:
            zero_page(ora);
            cpu_registers.cycles += 3;
            break;
        case 0x15:
            zero_page_x(ora);
            cpu_registers.cycles += 4;
            break;
        case 0x0D:
            absolute(ora);
            cpu_registers.cycles += 4;
            break;
        case 0x1D:
            cpu_registers.cycles += (4 + absolute_x(ora));
            //TODO special cycle case
            break;
        case 0x19:
            cpu_registers.cycles += (4 + absolute_y(ora));
            //TODO special cycle case
            break;
        case 0x01:
            indirect_x(ora);
            cpu_registers.cycles += 6;
            break;
        case 0x11:
            cpu_registers.cycles += (5 + indirect_y(ora));
            //TODO special cycle case
            break;

        //PHA
        case 0x48:
            pha();
            cpu_registers.cycles += 3;
            break;

        //PHP
        case 0x08:
            php();
            cpu_registers.cycles += 3;
            break;

        //PLA
        case 0x68:
            pla();
            cpu_registers.cycles += 4;
            break;

        //PLP
        case 0x28:
            plp();
            cpu_registers.cycles += 4;
            break;

        //ROL
        case 0x2A:
            accumulator(rol);
            cpu_registers.cycles += 2;
            break;
        case 0x26:
            zero_page(rol);
            cpu_registers.cycles += 5;
            break;
        case 0x36:
            zero_page_x(rol);
            cpu_registers.cycles += 6;
            break;
        case 0x2E:
            absolute(rol);
            cpu_registers.cycles += 6;
            break;
        case 0x3E:
            absolute_x(rol);
            cpu_registers.cycles += 7;
            break;

        //ROR
        case 0x6A:
            accumulator(ror);
            cpu_registers.cycles += 2;
            break;
        case 0x66:
            zero_page(ror);
            cpu_registers.cycles += 5;
            break;
        case 0x76:
            zero_page_x(ror);
            cpu_registers.cycles += 6;
            break;
        case 0x6E:
            absolute(ror);
            cpu_registers.cycles += 6;
            break;
        case 0x7E:
            absolute_x(ror);
            cpu_registers.cycles += 7;
            break;

        //RTI
        case 0x40:
            rti();
            cpu_registers.cycles += 6;
            break;

        //RTS
        case 0x60:
            rts();
            cpu_registers.cycles += 6;
            break;

        //SBC
        case 0xE9:
            immediate(sbc);
            cpu_registers.cycles += 2;
            break;
        case 0xE5:
            zero_page(sbc);
            cpu_registers.cycles += 3;
            break;
        case 0xF5:
            zero_page_x(sbc);
            cpu_registers.cycles += 4;
            break;
        case 0xED:
            absolute(sbc);
            cpu_registers.cycles += 4;
            break;
        case 0xFD:
            cpu_registers.cycles += (4 + absolute_x(sbc));
            //TODO special cycle case
            break;
        case 0xF9:
            cpu_registers.cycles += (4 + absolute_y(sbc));
            //TODO special cycle case
            break;
        case 0xE1:
            indirect_x(sbc);
            cpu_registers.cycles += 6;
            break;
        case 0xF1:
            cpu_registers.cycles += (5 + indirect_y(sbc));
            //TODO special cycle case
            break;

        //SEC
        case 0x38:
            sec();
            cpu_registers.cycles += 2;
            break;

        //SED
        case 0xF8:
            sed();
            cpu_registers.cycles += 2;
            break;

        //SEI
        case 0x78:
            sei();
            cpu_registers.cycles += 2;
            break;

        //STA
        case 0x85:
            zero_page(sta);
            cpu_registers.cycles += 3;
            break;
        case 0x95:
            zero_page_x(sta);
            cpu_registers.cycles += 4;
            break;
        case 0x8D:
            absolute(sta);
            cpu_registers.cycles += 4;
            break;
        case 0x9D:
            absolute_x(sta);
            cpu_registers.cycles += 5;
            break;
        case 0x99:
            absolute_y(sta);
            cpu_registers.cycles += 5;
            break;
        case 0x81:
            indirect_x(sta);
            cpu_registers.cycles += 6;
            break;
        case 0x91:
            indirect_y(sta);
            cpu_registers.cycles += 6;
            break;

        //STX
        case 0x86:
            zero_page(stx);
            cpu_registers.cycles += 3;
            break;
        case 0x96:
            zero_page_y(stx);
            cpu_registers.cycles += 4;
            break;
        case 0x8E:
            absolute(stx);
            cpu_registers.cycles += 4;
            break;

        //STY
        case 0x84:
            zero_page(sty);
            cpu_registers.cycles += 3;
            break;
        case 0x94:
            zero_page_x(sty);
            cpu_registers.cycles += 4;
            break;
        case 0x8C:
            absolute(sty);
            cpu_registers.cycles += 4;
            break;

        //TAX
        case 0xAA:
            tax();
            cpu_registers.cycles += 2;
            break;

        //TAY
        case 0xA8:
            tay();
            cpu_registers.cycles += 2;
            break;

        //TSX
        case 0xBA:
            tsx();
            cpu_registers.cycles += 2;
            break;

        //TXA
        case 0x8A:
            txa();
            cpu_registers.cycles += 2;
            break;

        //TXS
        case 0x9A:
            txs();
            cpu_registers.cycles += 2;
            break;

        //TYA
        case 0x98:
            tya();
            cpu_registers.cycles += 2;
            break;

        default:
            spdlog::critical("INVALID INSTRUCTION. OPCODE: 0x{:X}",opcode);
            spdlog::critical("Halting CPU...");
            exit();
            exit(1);
    }

}

