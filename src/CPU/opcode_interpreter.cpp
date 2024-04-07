//
// Created by eafc6 on 8/12/2023.
//

#include <iostream>
#include "cpu.h"

// low byte then high byte

void CPU::execute_opcode(int opcode){
    switch (opcode) {
        //ADC
        case 0x69:
            immediate(&CPU::adc);
            cycles += 2;
            break;
        case 0x65:
            zero_page(&CPU::adc);
            cycles += 3;
            break;
        case 0x75:
            zero_page_x(&CPU::adc);
            cycles += 4;
            break;
        case 0x6D:
            absolute(&CPU::adc);
            cycles += 4;
            break;
        case 0x7D:
            cycles += (4 + absolute_x(&CPU::adc));
            //TODO special cycle case
            break;
        case 0x79:
            cycles += (4 + absolute_y(&CPU::adc));
            //TODO special cycle case
            break;
        case 0x61:
            indirect_x(&CPU::adc);
            cycles += 6;
            break;
        case 0x71:
            cycles += (5 + indirect_y(&CPU::adc));
            //TODO special cycle case
            break;

        //AND
        case 0x29:
            immediate(&CPU::logical_and);
            cycles += 2;
            break;
        case 0x25:
            zero_page(&CPU::logical_and);
            cycles += 3;
            break;
        case 0x35:
            zero_page_x(&CPU::logical_and);
            cycles += 4;
            break;
        case 0x2D:
            absolute(&CPU::logical_and);
            cycles += 4;
            break;
        case 0x3D:
            cycles += (4 + absolute_x(&CPU::logical_and));
            //TODO special cycle case
            break;
        case 0x39:
            cycles += (4 + absolute_y(&CPU::logical_and));
            //TODO special cycle case
            break;
        case 0x21:
            indirect_x(&CPU::logical_and);
            cycles += 6;
            break;
        case 0x31:
            cycles += (5 + indirect_y(&CPU::logical_and));
            //TODO special cycle case
            break;

        //ASL
        case 0x0A:
            accumulator(&CPU::asl);
            cycles += 2;
            break;
        case 0x06:
            zero_page(&CPU::asl);
            cycles += 5;
            break;
        case 0x16:
            zero_page_x(&CPU::asl);
            cycles += 6;
            break;
        case 0x0E:
            absolute(&CPU::asl);
            cycles += 6;
            break;
        case 0x1E:
            absolute_x(&CPU::asl);
            cycles += 7;
            break;

        //BCC
        case 0x90:
            current_operand = mem[registers.pc];
            bcc(mem[registers.pc++]);
            //TODO special cycle case
            cycles += 2;
            break;

        //BCS
        case 0xB0:
            current_operand = mem[registers.pc];
            bcs(mem[registers.pc++]);
            //TODO special cycle case
            cycles += 2;
            break;

        //BEQ
        case 0xF0:
            current_operand = mem[registers.pc];
            beq(mem[registers.pc++]);
            //TODO special cycle case
            cycles += 2;
            break;

        //BIT
        case 0x24:
            zero_page(&CPU::bit);
            cycles += 3;
            break;
        case 0x2C:
            absolute(&CPU::bit);
            cycles += 4;
            break;

        //BMI
        case 0x30:
            current_operand = mem[registers.pc];
            bmi(mem[registers.pc++]);
            cycles += 2;
            //TODO special cycle case
            break;

        //BNE
        case 0xD0:
            current_operand = mem[registers.pc];
            bne(mem[registers.pc++]);
            cycles += 2;
            //TODO special cycle case
            break;

        //BPL
        case 0x10:
            current_operand = mem[registers.pc];
            bpl(mem[registers.pc++]);
            cycles += 2;
            //TODO special cycle case
            break;

        //BRK
        case 0x00:
            brk();
            cycles += 7;
            break;

        //BVC
        case 0x50:
            current_operand = mem[registers.pc];
            bvc(mem[registers.pc++]);
            cycles += 2;
            //TODO special cycle case
            break;

        //BVS
        case 0x70:
            current_operand = mem[registers.pc];
            bvs(mem[registers.pc++]);
            cycles += 2;
            //TODO special cycle case
            break;

        //CLC
        case 0x18:
            clc();
            cycles += 2;
            break;

        //CLD
        case 0xD8:
            cld();
            cycles += 2;
            break;

        //CLI
        case 0x58:
            cli();
            cycles += 2;
            break;

        //CLV
        case 0xB8:
            clv();
            cycles += 2;
            break;

        //CMP
        case 0xC9:
            immediate(&CPU::cmp);
            cycles += 2;
            break;
        case 0xC5:
            zero_page(&CPU::cmp);
            cycles += 3;
            break;
        case 0xD5:
            zero_page_x(&CPU::cmp);
            cycles += 4;
            break;
        case 0xCD:
            absolute(&CPU::cmp);
            cycles += 4;
            break;
        case 0xDD:
            cycles += (4 + absolute_x(&CPU::cmp));
            //TODO special cycle case
            break;
        case 0xD9:
            cycles += (4 + absolute_y(&CPU::cmp));
            //TODO special cycle case
            break;
        case 0xC1:
            indirect_x(&CPU::cmp);
            cycles += 6;
            break;
        case 0xD1:
            cycles += (5 + indirect_y(&CPU::cmp));
            //TODO special cycle case
            break;

        //CPX
        case 0xE0:
            immediate(&CPU::cpx);
            cycles += 2;
            break;
        case 0xE4:
            zero_page(&CPU::cpx);
            cycles += 3;
            break;
        case 0xEC:
            absolute(&CPU::cpx);
            cycles += 4;
            break;

        //CPY
        case 0xC0:
            immediate(&CPU::cpy);
            cycles += 2;
            break;
        case 0xC4:
            zero_page(&CPU::cpy);
            cycles += 3;
            break;
        case 0xCC:
            absolute(&CPU::cpy);
            cycles += 4;
            break;

        //DEC
        case 0xC6:
            zero_page(&CPU::dec);
            cycles += 5;
            break;
        case 0xD6:
            zero_page_x(&CPU::dec);
            cycles += 6;
            break;
        case 0xCE:
            absolute(&CPU::dec);
            cycles += 6;
            break;
        case 0xDE:
            absolute_x(&CPU::dec);
            cycles += 7;
            break;

        //DEX
        case 0xCA:
            dex();
            cycles += 2;
            break;

        //DEY
        case 0x88:
            dey();
            cycles += 2;
            break;

        //EOR
        case 0x49:
            immediate(&CPU::eor);
            cycles += 2;
            break;
        case 0x45:
            zero_page(&CPU::eor);
            cycles += 3;
            break;
        case 0x55:
            zero_page_x(&CPU::eor);
            cycles += 4;
            break;
        case 0x4D:
            absolute(&CPU::eor);
            cycles += 4;
            break;
        case 0x5D:
            cycles += (4 + absolute_x(&CPU::eor));
            //TODO special cycle case
            break;
        case 0x59:
            cycles += (4 + absolute_y(&CPU::eor));
            //TODO special cycle case
            break;
        case 0x41:
            indirect_x(&CPU::eor);
            cycles += 6;
            break;
        case 0x51:
            cycles += (5 + indirect_y(&CPU::eor));
            //TODO special cycle case
            break;

        //INC
        case 0xE6:
            zero_page(&CPU::inc);
            cycles += 5;
            break;
        case 0xF6:
            zero_page_x(&CPU::inc);
            cycles += 6;
            break;
        case 0xEE:
            absolute(&CPU::inc);
            cycles += 6;
            break;
        case 0xFE:
            absolute_x(&CPU::inc);
            cycles += 7;
            break;

        //INX
        case 0xE8:
            inx();
            cycles += 2;
            break;

        //INY
        case 0xC8:
            iny();
            cycles += 2;
            break;

        //JMP
        case 0x4C:
            absolute(&CPU::jmp);
            cycles += 3;
            break;
        case 0x6C:
            indirect(&CPU::jmp);
            cycles += 5;
            break;

        //JSR
        case 0x20:
            absolute(&CPU::jsr);
            cycles += 6;
            break;

        //LDA
        case 0xA9:
            immediate(&CPU::lda);
            cycles += 2;
            break;
        case 0xA5:
            zero_page(&CPU::lda);
            cycles += 3;
            break;
        case 0xB5:
            zero_page_x(&CPU::lda);
            cycles += 4;
            break;
        case 0xAD:
            absolute(&CPU::lda);
            cycles += 4;
            break;
        case 0xBD:
            cycles += (4 + absolute_x(&CPU::lda));
            //TODO special cycle case
            break;
        case 0xB9:
            cycles += (4 + absolute_y(&CPU::lda));
            //TODO special cycle case
            break;
        case 0xA1:
            indirect_x(&CPU::lda);
            cycles += 6;
            break;
        case 0xB1:
            cycles += (5 + indirect_y(&CPU::lda));
            //TODO special cycle case
            break;

        //LDX
        case 0xA2:
            immediate(&CPU::ldx);
            cycles += 2;
            break;
        case 0xA6:
            zero_page(&CPU::ldx);
            cycles += 3;
            break;
        case 0xB6:
            zero_page_y(&CPU::ldx);
            cycles += 4;
            break;
        case 0xAE:
            absolute(&CPU::ldx);
            cycles += 4;
            break;
        case 0xBE:
            cycles += (4 + absolute_y(&CPU::ldx));
            //TODO special cycle case
            break;

        //LDY
        case 0xA0:
            immediate(&CPU::ldy);
            cycles += 2;
            break;
        case 0xA4:
            zero_page(&CPU::ldy);
            cycles += 3;
            break;
        case 0xB4:
            zero_page_x(&CPU::ldy);
            cycles += 4;
            break;
        case 0xAC:
            absolute(&CPU::ldy);
            cycles += 4;
            break;
        case 0xBC:
            cycles += (4 + absolute_x(&CPU::ldy));
            //TODO special cycle case
            break;

        //LSR
        case 0x4A:
            accumulator(&CPU::lsr);
            cycles += 2;
            break;
        case 0x46:
            zero_page(&CPU::lsr);
            cycles += 5;
            break;
        case 0x56:
            zero_page_x(&CPU::lsr);
            cycles += 6;
            break;
        case 0x4E:
            absolute(&CPU::lsr);
            cycles += 6;
            break;
        case 0x5E:
            absolute_x(&CPU::lsr);
            cycles += 7;
            break;

        //NOP
        case 0xEA:
            nop();
            cycles += 2;
            break;
        //UNOFFICIAL NOPS
        case 0x1A:
        case 0x3A:
        case 0x5A:
        case 0x7A:
        case 0xDA:
        case 0xFA:
            nop();
            cycles += 2;
            break;
        case 0x80:
        case 0x82:
        case 0x89:
        case 0xC2:
        case 0xE2:
            immediate(&CPU::nop);
            cycles += 2;
            break;
        case 0x04:
        case 0x44:
        case 0x64:
            zero_page(&CPU::nop);
            cycles += 3;
            break;
        case 0x14:
        case 0x34:
        case 0x54:
        case 0x74:
        case 0xD4:
        case 0xF4:
            zero_page_x(&CPU::nop);
            cycles += 4;
            break;
        case 0x0C:
            absolute(&CPU::nop);
            cycles += 4;
            break;
        case 0x1C:
        case 0x3C:
        case 0x5C:
        case 0x7C:
        case 0xDC:
        case 0xFC:
            cycles += (4 + absolute_x(&CPU::nop));
            //TODO special cycle case
            break;


        //ORA
        case 0x09:
            immediate(&CPU::ora);
            cycles += 2;
            break;
        case 0x05:
            zero_page(&CPU::ora);
            cycles += 3;
            break;
        case 0x15:
            zero_page_x(&CPU::ora);
            cycles += 4;
            break;
        case 0x0D:
            absolute(&CPU::ora);
            cycles += 4;
            break;
        case 0x1D:
            cycles += (4 + absolute_x(&CPU::ora));
            //TODO special cycle case
            break;
        case 0x19:
            cycles += (4 + absolute_y(&CPU::ora));
            //TODO special cycle case
            break;
        case 0x01:
            indirect_x(&CPU::ora);
            cycles += 6;
            break;
        case 0x11:
            cycles += (5 + indirect_y(&CPU::ora));
            //TODO special cycle case
            break;

        //PHA
        case 0x48:
            pha();
            cycles += 3;
            break;

        //PHP
        case 0x08:
            php();
            cycles += 3;
            break;

        //PLA
        case 0x68:
            pla();
            cycles += 4;
            break;

        //PLP
        case 0x28:
            plp();
            cycles += 4;
            break;

        //ROL
        case 0x2A:
            accumulator(&CPU::rol);
            cycles += 2;
            break;
        case 0x26:
            zero_page(&CPU::rol);
            cycles += 5;
            break;
        case 0x36:
            zero_page_x(&CPU::rol);
            cycles += 6;
            break;
        case 0x2E:
            absolute(&CPU::rol);
            cycles += 6;
            break;
        case 0x3E:
            absolute_x(&CPU::rol);
            cycles += 7;
            break;

        //ROR
        case 0x6A:
            accumulator(&CPU::ror);
            cycles += 2;
            break;
        case 0x66:
            zero_page(&CPU::ror);
            cycles += 5;
            break;
        case 0x76:
            zero_page_x(&CPU::ror);
            cycles += 6;
            break;
        case 0x6E:
            absolute(&CPU::ror);
            cycles += 6;
            break;
        case 0x7E:
            absolute_x(&CPU::ror);
            cycles += 7;
            break;

        //RTI
        case 0x40:
            rti();
            cycles += 6;
            break;

        //RTS
        case 0x60:
            rts();
            cycles += 6;
            break;

        //SBC
        case 0xE9:
            immediate(&CPU::sbc);
            cycles += 2;
            break;
        case 0xE5:
            zero_page(&CPU::sbc);
            cycles += 3;
            break;
        case 0xF5:
            zero_page_x(&CPU::sbc);
            cycles += 4;
            break;
        case 0xED:
            absolute(&CPU::sbc);
            cycles += 4;
            break;
        case 0xFD:
            cycles += (4 + absolute_x(&CPU::sbc));
            //TODO special cycle case
            break;
        case 0xF9:
            cycles += (4 + absolute_y(&CPU::sbc));
            //TODO special cycle case
            break;
        case 0xE1:
            indirect_x(&CPU::sbc);
            cycles += 6;
            break;
        case 0xF1:
            cycles += (5 + indirect_y(&CPU::sbc));
            //TODO special cycle case
            break;

        //SEC
        case 0x38:
            sec();
            cycles += 2;
            break;

        //SED
        case 0xF8:
            sed();
            cycles += 2;
            break;

        //SEI
        case 0x78:
            sei();
            cycles += 2;
            break;

        //STA
        case 0x85:
            zero_page(&CPU::sta);
            cycles += 3;
            break;
        case 0x95:
            zero_page_x(&CPU::sta);
            cycles += 4;
            break;
        case 0x8D:
            absolute(&CPU::sta);
            cycles += 4;
            break;
        case 0x9D:
            absolute_x(&CPU::sta);
            cycles += 5;
            break;
        case 0x99:
            absolute_y(&CPU::sta);
            cycles += 5;
            break;
        case 0x81:
            indirect_x(&CPU::sta);
            cycles += 6;
            break;
        case 0x91:
            indirect_y(&CPU::sta);
            cycles += 6;
            break;

        //STX
        case 0x86:
            zero_page(&CPU::stx);
            cycles += 3;
            break;
        case 0x96:
            zero_page_y(&CPU::stx);
            cycles += 4;
            break;
        case 0x8E:
            absolute(&CPU::stx);
            cycles += 4;
            break;

        //STY
        case 0x84:
            zero_page(&CPU::sty);
            cycles += 3;
            break;
        case 0x94:
            zero_page_x(&CPU::sty);
            cycles += 4;
            break;
        case 0x8C:
            absolute(&CPU::sty);
            cycles += 4;
            break;

        //TAX
        case 0xAA:
            tax();
            cycles += 2;
            break;

        //TAY
        case 0xA8:
            tay();
            cycles += 2;
            break;

        //TSX
        case 0xBA:
            tsx();
            cycles += 2;
            break;

        //TXA
        case 0x8A:
            txa();
            cycles += 2;
            break;

        //TXS
        case 0x9A:
            txs();
            cycles += 2;
            break;

        //TYA
        case 0x98:
            tya();
            cycles += 2;
            break;

        //ALL OTHER UNOFFICIAL OPS

        //ALR
        case 0x4B:
            immediate(&CPU::alr);
            cycles += 2;
            break;

        //ANC
        case 0x0B:
            immediate(&CPU::anc);
            cycles += 2;
            break;
        //ANC2
        case 0x2B:
            immediate(&CPU::anc2);
            cycles += 2;
            break;
        //ANE (XAA)
        case 0x8B:
            immediate(&CPU::ane);
            cycles += 2;
            break;
        //ARR
        case 0x6B:
            immediate(&CPU::arr);
            cycles += 2;
            break;
        //DCP
        case 0xC7:
            zero_page(&CPU::dcp);
            cycles += 5;
            break;
        case 0xD7:
            zero_page_x(&CPU::dcp);
            cycles += 6;
            break;
        case 0xCF:
            absolute(&CPU::dcp);
            cycles += 6;
            break;
        case 0xDF:
            absolute_x(&CPU::dcp);
            cycles += 7;
            break;
        case 0xDB:
            absolute_y(&CPU::dcp);
            cycles += 7;
            break;
        case 0xC3:
            indirect_x(&CPU::dcp);
            cycles += 8;
            break;
        case 0xD3:
            indirect_y(&CPU::dcp);
            cycles += 8;
            break;
        //ISC
        case 0xE7:
            zero_page(&CPU::isc);
            cycles += 5;
            break;
        case 0xF7:
            zero_page_x(&CPU::isc);
            cycles += 6;
            break;
        case 0xEF:
            absolute(&CPU::isc);
            cycles += 6;
            break;
        case 0xFF:
            absolute_x(&CPU::isc);
            cycles += 7;
            break;
        case 0xFB:
            absolute_y(&CPU::isc);
            cycles += 7;
            break;
        case 0xE3:
            indirect_x(&CPU::isc);
            cycles += 8;
            break;
        case 0xF3:
            indirect_y(&CPU::isc);
            cycles += 8;
            break;
        //LAS
        case 0xBB:
            absolute_y(&CPU::las);
            cycles += 4;
            break;
        //LAX
        case 0xA7:
            zero_page(&CPU::lax);
            cycles += 3;
            break;
        case 0xB7:
            zero_page_y(&CPU::lax);
            cycles += 4;
            break;
        case 0xAF:
            absolute(&CPU::lax);
            cycles += 4;
            break;
        case 0xBF:
            absolute_y(&CPU::lax);
            cycles += 4;
            break;
        case 0xA3:
            indirect_x(&CPU::lax);
            cycles += 6;
            break;
        case 0xB3:
            indirect_y(&CPU::lax);
            cycles += 5;
            break;
        //LXA
        case 0xAB:
            immediate(&CPU::lax);
            cycles += 2;
            break;
        //RLA
        case 0x27:
            zero_page(&CPU::rla);
            cycles += 5;
            break;
        case 0x37:
            zero_page_x(&CPU::rla);
            cycles += 6;
            break;
        case 0x2F:
            absolute(&CPU::rla);
            cycles += 6;
            break;
        case 0x3F:
            absolute_x(&CPU::rla);
            cycles += 7;
            break;
        case 0x3B:
            absolute_y(&CPU::rla);
            cycles += 7;
            break;
        case 0x23:
            indirect_x(&CPU::rla);
            cycles += 8;
            break;
        case 0x33:
            indirect_y(&CPU::rla);
            cycles += 8;
            break;
        //RRA
        case 0x67:
            zero_page(&CPU::rra);
            cycles += 5;
            break;
        case 0x77:
            zero_page_x(&CPU::rra);
            cycles += 6;
            break;
        case 0x6F:
            absolute(&CPU::rra);
            cycles += 6;
            break;
        case 0x7F:
            absolute_x(&CPU::rra);
            cycles += 7;
            break;
        case 0x7B:
            absolute_y(&CPU::rra);
            cycles += 7;
            break;
        case 0x63:
            indirect_x(&CPU::rra);
            cycles += 8;
            break;
        case 0x73:
            indirect_y(&CPU::rra);
            cycles += 8;
            break;
        //SAX
        case 0x87:
            zero_page(&CPU::sax);
            cycles += 3;
            break;
        case 0x97:
            zero_page_y(&CPU::sax);
            cycles += 4;
            break;
        case 0x8F:
            absolute(&CPU::sax);
            cycles += 4;
            break;
        case 0x83:
            indirect_x(&CPU::sax);
            cycles += 6;
            break;
        //SBX
        case 0xCB:
            immediate(&CPU::sbx);
            cycles += 2;
            break;
        //SHA
        case 0x9F:
            absolute_y(&CPU::sha);
            cycles += 5;
            break;
        case 0x93:
            indirect_y(&CPU::sha);
            cycles += 6;
            break;
        //SHX
        case 0x9E:
            absolute_y(&CPU::shx);
            cycles += 5;
            break;
        //SHY
        case 0x9C:
            absolute_x(&CPU::shy);
            cycles += 5;
            break;
        //SLO
        case 0x07:
            zero_page(&CPU::slo);
            cycles += 5;
            break;
        case 0x17:
            zero_page_x(&CPU::slo);
            cycles += 6;
            break;
        case 0x0F:
            absolute(&CPU::slo);
            cycles += 6;
            break;
        case 0x1F:
            absolute_x(&CPU::slo);
            cycles += 7;
            break;
        case 0x1B:
            absolute_y(&CPU::slo);
            cycles += 7;
            break;
        case 0x03:
            indirect_x(&CPU::slo);
            cycles += 8;
            break;
        case 0x13:
            indirect_y(&CPU::slo);
            cycles += 8;
            break;
        //SRE
        case 0x47:
            zero_page(&CPU::sre);
            cycles += 5;
            break;
        case 0x57:
            zero_page_x(&CPU::sre);
            cycles += 6;
            break;
        case 0x4F:
            absolute(&CPU::sre);
            cycles += 6;
            break;
        case 0x5F:
            absolute_x(&CPU::sre);
            cycles += 7;
            break;
        case 0x5B:
            absolute_y(&CPU::sre);
            cycles += 7;
            break;
        case 0x43:
            indirect_x(&CPU::sre);
            cycles += 8;
            break;
        case 0x53:
            indirect_y(&CPU::sre);
            cycles += 8;
            break;
        //TAS
        case 0x9B:
            absolute_y(&CPU::tas);
            cycles += 5;
            break;
        //USBC
        case 0xEB:
            immediate(&CPU::usbc);
            cycles += 2;
            break;




        default:
            spdlog::critical("INVALID INSTRUCTION. OPCODE: 0x{:X}",opcode);
            spdlog::critical("Halting CPU...");
            exit();
            exit(1);
    }

}

