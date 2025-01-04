//
// Created by eafc6 on 8/12/2023.
//

#include <iostream>
#include "cpu.h"
#include "../global.h"

// low byte then high byte

void CPU::execute_opcode(int opcode){
    uint8_t prev_mode;
    uint8_t operand;
    switch (opcode) {
        //ADC
        case 0x69:
            immediate(&CPU::adc);
            break;
        case 0x65:
            zero_page(&CPU::adc);
            break;
        case 0x75:
            zero_page_x(&CPU::adc);
            break;
        case 0x6D:
            absolute(&CPU::adc);
            break;
        case 0x7D:
            absolute_x(&CPU::adc);
            break;
        case 0x79:
            absolute_y(&CPU::adc);
            break;
        case 0x61:
            indirect_x(&CPU::adc);
            break;
        case 0x71:
            indirect_y(&CPU::adc);
            break;

        //AND
        case 0x29:
            immediate(&CPU::logical_and);
            break;
        case 0x25:
            zero_page(&CPU::logical_and);
            break;
        case 0x35:
            zero_page_x(&CPU::logical_and);
            break;
        case 0x2D:
            absolute(&CPU::logical_and);
            break;
        case 0x3D:
            absolute_x(&CPU::logical_and);
            break;
        case 0x39:
            absolute_y(&CPU::logical_and);
            break;
        case 0x21:
            indirect_x(&CPU::logical_and);
            break;
        case 0x31:
            indirect_y(&CPU::logical_and);
            break;

        //ASL
        case 0x0A:
            accumulator(&CPU::asl);
            break;
        case 0x06:
            zero_page(&CPU::asl);
            break;
        case 0x16:
            zero_page_x(&CPU::asl);
            break;
        case 0x0E:
            absolute(&CPU::asl);
            break;
        case 0x1E:
            absolute_x(&CPU::asl);
            break;

        //BCC
        case 0x90:
            prev_mode = rw_register_mode;
            rw_register_mode = 0x0;
            current_operand = read(registers.pc);
            operand = read(registers.pc++);
            rw_register_mode = prev_mode;
            bcc(operand);
            break;

        //BCS
        case 0xB0:
            prev_mode = rw_register_mode;
            rw_register_mode = 0x0;
            current_operand = read(registers.pc);
            operand = read(registers.pc++);
            rw_register_mode = prev_mode;
            bcs(operand);
            break;

        //BEQ
        case 0xF0:
            prev_mode = rw_register_mode;
            rw_register_mode = 0x0;
            current_operand = read(registers.pc);
            operand = read(registers.pc++);
            rw_register_mode = prev_mode;
            beq(operand);
            break;

        //BIT
        case 0x24:
            zero_page(&CPU::bit);
            break;
        case 0x2C:
            absolute(&CPU::bit);
            break;

        //BMI
        case 0x30:
            prev_mode = rw_register_mode;
            rw_register_mode = 0x0;
            current_operand = read(registers.pc);
            operand = read(registers.pc++);
            rw_register_mode = prev_mode;
            bmi(operand);
            break;

        //BNE
        case 0xD0:
            prev_mode = rw_register_mode;
            rw_register_mode = 0x0;
            current_operand = read(registers.pc);
            operand = read(registers.pc++);
            rw_register_mode = prev_mode;
            bne(operand);
            break;

        //BPL
        case 0x10:
            prev_mode = rw_register_mode;
            rw_register_mode = 0x0;
            current_operand = read(registers.pc);
            operand = read(registers.pc++);
            rw_register_mode = prev_mode;
            bpl(operand);
            break;

        //BRK
        case 0x00:
            brk();
            break;

        //BVC
        case 0x50:
            prev_mode = rw_register_mode;
            rw_register_mode = 0x0;
            current_operand = read(registers.pc);
            operand = read(registers.pc++);
            rw_register_mode = prev_mode;
            bvc(operand);
            break;

        //BVS
        case 0x70:
            prev_mode = rw_register_mode;
            rw_register_mode = 0x0;
            current_operand = read(registers.pc);
            operand = read(registers.pc++);
            rw_register_mode = prev_mode;
            bvs(operand);
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
            immediate(&CPU::cmp);
            break;
        case 0xC5:
            zero_page(&CPU::cmp);
            break;
        case 0xD5:
            zero_page_x(&CPU::cmp);
            break;
        case 0xCD:
            absolute(&CPU::cmp);
            break;
        case 0xDD:
            absolute_x(&CPU::cmp);
            break;
        case 0xD9:
            absolute_y(&CPU::cmp);
            break;
        case 0xC1:
            indirect_x(&CPU::cmp);
            break;
        case 0xD1:
            indirect_y(&CPU::cmp);
            break;

        //CPX
        case 0xE0:
            immediate(&CPU::cpx);
            break;
        case 0xE4:
            zero_page(&CPU::cpx);
            break;
        case 0xEC:
            absolute(&CPU::cpx);
            break;

        //CPY
        case 0xC0:
            immediate(&CPU::cpy);
            break;
        case 0xC4:
            zero_page(&CPU::cpy);
            break;
        case 0xCC:
            absolute(&CPU::cpy);
            break;

        //DEC
        case 0xC6:
            zero_page(&CPU::dec);
            break;
        case 0xD6:
            zero_page_x(&CPU::dec);
            break;
        case 0xCE:
            absolute(&CPU::dec);
            break;
        case 0xDE:
            absolute_x(&CPU::dec);
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
            immediate(&CPU::eor);
            break;
        case 0x45:
            zero_page(&CPU::eor);
            break;
        case 0x55:
            zero_page_x(&CPU::eor);
            break;
        case 0x4D:
            absolute(&CPU::eor);
            break;
        case 0x5D:
            absolute_x(&CPU::eor);
            break;
        case 0x59:
            absolute_y(&CPU::eor);
            break;
        case 0x41:
            indirect_x(&CPU::eor);
            break;
        case 0x51:
            indirect_y(&CPU::eor);
            break;

        //INC
        case 0xE6:
            zero_page(&CPU::inc);
            break;
        case 0xF6:
            zero_page_x(&CPU::inc);
            break;
        case 0xEE:
            absolute(&CPU::inc);
            break;
        case 0xFE:
            absolute_x(&CPU::inc);
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
            absolute(&CPU::jmp);
            break;
        case 0x6C:
            indirect(&CPU::jmp);
            break;

        //JSR
        case 0x20:
            absolute(&CPU::jsr);
            break;

        //LDA
        case 0xA9:
            immediate(&CPU::lda);
            break;
        case 0xA5:
            zero_page(&CPU::lda);
            break;
        case 0xB5:
            zero_page_x(&CPU::lda);
            break;
        case 0xAD:
            absolute(&CPU::lda);
            break;
        case 0xBD:
            absolute_x(&CPU::lda);
            break;
        case 0xB9:
            absolute_y(&CPU::lda);
            break;
        case 0xA1:
            indirect_x(&CPU::lda);
            break;
        case 0xB1:
            indirect_y(&CPU::lda);
            break;

        //LDX
        case 0xA2:
            immediate(&CPU::ldx);
            break;
        case 0xA6:
            zero_page(&CPU::ldx);
            break;
        case 0xB6:
            zero_page_y(&CPU::ldx);
            break;
        case 0xAE:
            absolute(&CPU::ldx);
            break;
        case 0xBE:
            absolute_y(&CPU::ldx);
            break;

        //LDY
        case 0xA0:
            immediate(&CPU::ldy);
            break;
        case 0xA4:
            zero_page(&CPU::ldy);
            break;
        case 0xB4:
            zero_page_x(&CPU::ldy);
            break;
        case 0xAC:
            absolute(&CPU::ldy);
            break;
        case 0xBC:
            absolute_x(&CPU::ldy);
            break;

        //LSR
        case 0x4A:
            accumulator(&CPU::lsr);
            break;
        case 0x46:
            zero_page(&CPU::lsr);
            break;
        case 0x56:
            zero_page_x(&CPU::lsr);
            break;
        case 0x4E:
            absolute(&CPU::lsr);
            break;
        case 0x5E:
            absolute_x(&CPU::lsr);
            break;

        //NOP
        case 0xEA:
        //UNOFFICIAL NOPS
        case 0x1A:
        case 0x3A:
        case 0x5A:
        case 0x7A:
        case 0xDA:
        case 0xFA:
            nop();
            break;
        case 0x80:
        case 0x82:
        case 0x89:
        case 0xC2:
        case 0xE2:
            immediate(&CPU::nop);
            break;
        case 0x04:
        case 0x44:
        case 0x64:
            zero_page(&CPU::nop);
            break;
        case 0x14:
        case 0x34:
        case 0x54:
        case 0x74:
        case 0xD4:
        case 0xF4:
            zero_page_x(&CPU::nop);
            break;
        case 0x0C:
            absolute(&CPU::nop);
            break;
        case 0x1C:
        case 0x3C:
        case 0x5C:
        case 0x7C:
        case 0xDC:
        case 0xFC:
            absolute_x(&CPU::nop);
            break;


        //ORA
        case 0x09:
            immediate(&CPU::ora);
            break;
        case 0x05:
            zero_page(&CPU::ora);
            break;
        case 0x15:
            zero_page_x(&CPU::ora);
            break;
        case 0x0D:
            absolute(&CPU::ora);
            break;
        case 0x1D:
            absolute_x(&CPU::ora);
            break;
        case 0x19:
            absolute_y(&CPU::ora);
            break;
        case 0x01:
            indirect_x(&CPU::ora);
            break;
        case 0x11:
            indirect_y(&CPU::ora);
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
            accumulator(&CPU::rol);
            break;
        case 0x26:
            zero_page(&CPU::rol);
            break;
        case 0x36:
            zero_page_x(&CPU::rol);
            break;
        case 0x2E:
            absolute(&CPU::rol);
            break;
        case 0x3E:
            absolute_x(&CPU::rol);
            break;

        //ROR
        case 0x6A:
            accumulator(&CPU::ror);
            break;
        case 0x66:
            zero_page(&CPU::ror);
            break;
        case 0x76:
            zero_page_x(&CPU::ror);
            break;
        case 0x6E:
            absolute(&CPU::ror);
            break;
        case 0x7E:
            absolute_x(&CPU::ror);
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
            immediate(&CPU::sbc);
            break;
        case 0xE5:
            zero_page(&CPU::sbc);
            break;
        case 0xF5:
            zero_page_x(&CPU::sbc);
            break;
        case 0xED:
            absolute(&CPU::sbc);
            break;
        case 0xFD:
            absolute_x(&CPU::sbc);
            break;
        case 0xF9:
            absolute_y(&CPU::sbc);
            break;
        case 0xE1:
            indirect_x(&CPU::sbc);
            break;
        case 0xF1:
            indirect_y(&CPU::sbc);
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
            zero_page(&CPU::sta);
            break;
        case 0x95:
            zero_page_x(&CPU::sta);
            break;
        case 0x8D:
            absolute(&CPU::sta);
            break;
        case 0x9D:
            absolute_x(&CPU::sta);
            break;
        case 0x99:
            absolute_y(&CPU::sta);
            break;
        case 0x81:
            indirect_x(&CPU::sta);
            break;
        case 0x91:
            indirect_y(&CPU::sta);
            break;

        //STX
        case 0x86:
            zero_page(&CPU::stx);
            break;
        case 0x96:
            zero_page_y(&CPU::stx);
            break;
        case 0x8E:
            absolute(&CPU::stx);
            break;

        //STY
        case 0x84:
            zero_page(&CPU::sty);
            break;
        case 0x94:
            zero_page_x(&CPU::sty);
            break;
        case 0x8C:
            absolute(&CPU::sty);
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

        //ALL OTHER UNOFFICIAL OPS

        //ALR
        case 0x4B:
            immediate(&CPU::alr);
            break;

        //ANC
        case 0x0B:
            immediate(&CPU::anc);
            break;
        //ANC2
        case 0x2B:
            immediate(&CPU::anc2);
            break;
        //ANE (XAA)
        case 0x8B:
            immediate(&CPU::ane);
            break;
        //ARR
        case 0x6B:
            immediate(&CPU::arr);
            break;
        //DCP
        case 0xC7:
            zero_page(&CPU::dcp);
            break;
        case 0xD7:
            zero_page_x(&CPU::dcp);
            break;
        case 0xCF:
            absolute(&CPU::dcp);
            break;
        case 0xDF:
            absolute_x(&CPU::dcp);
            break;
        case 0xDB:
            absolute_y(&CPU::dcp);
            break;
        case 0xC3:
            indirect_x(&CPU::dcp);
            break;
        case 0xD3:
            indirect_y(&CPU::dcp);
            break;
        //ISC
        case 0xE7:
            zero_page(&CPU::isc);
            break;
        case 0xF7:
            zero_page_x(&CPU::isc);
            break;
        case 0xEF:
            absolute(&CPU::isc);
            break;
        case 0xFF:
            absolute_x(&CPU::isc);
            break;
        case 0xFB:
            absolute_y(&CPU::isc);
            break;
        case 0xE3:
            indirect_x(&CPU::isc);
            break;
        case 0xF3:
            indirect_y(&CPU::isc);
            break;
        //LAS
        case 0xBB:
            absolute_y(&CPU::las);
            break;
        //LAX
        case 0xA7:
            zero_page(&CPU::lax);
            break;
        case 0xB7:
            zero_page_y(&CPU::lax);
            break;
        case 0xAF:
            absolute(&CPU::lax);
            break;
        case 0xBF:
            absolute_y(&CPU::lax);
            break;
        case 0xA3:
            indirect_x(&CPU::lax);
            break;
        case 0xB3:
            indirect_y(&CPU::lax);
            break;
        //LXA
        case 0xAB:
            immediate(&CPU::lax);
            break;
        //RLA
        case 0x27:
            zero_page(&CPU::rla);
            break;
        case 0x37:
            zero_page_x(&CPU::rla);
            break;
        case 0x2F:
            absolute(&CPU::rla);
            break;
        case 0x3F:
            absolute_x(&CPU::rla);
            break;
        case 0x3B:
            absolute_y(&CPU::rla);
            break;
        case 0x23:
            indirect_x(&CPU::rla);
            break;
        case 0x33:
            indirect_y(&CPU::rla);
            break;
        //RRA
        case 0x67:
            zero_page(&CPU::rra);
            break;
        case 0x77:
            zero_page_x(&CPU::rra);
            break;
        case 0x6F:
            absolute(&CPU::rra);
            break;
        case 0x7F:
            absolute_x(&CPU::rra);
            break;
        case 0x7B:
            absolute_y(&CPU::rra);
            break;
        case 0x63:
            indirect_x(&CPU::rra);
            break;
        case 0x73:
            indirect_y(&CPU::rra);
            break;
        //SAX
        case 0x87:
            zero_page(&CPU::sax);
            break;
        case 0x97:
            zero_page_y(&CPU::sax);
            break;
        case 0x8F:
            absolute(&CPU::sax);
            break;
        case 0x83:
            indirect_x(&CPU::sax);
            break;
        //SBX
        case 0xCB:
            immediate(&CPU::sbx);
            break;
        //SHA
        case 0x9F:
            absolute_y(&CPU::sha);
            break;
        case 0x93:
            indirect_y(&CPU::sha);
            break;
        //SHX
        case 0x9E:
            absolute_y(&CPU::shx);
            break;
        //SHY
        case 0x9C:
            absolute_x(&CPU::shy);
            break;
        //SLO
        case 0x07:
            zero_page(&CPU::slo);
            break;
        case 0x17:
            zero_page_x(&CPU::slo);
            break;
        case 0x0F:
            absolute(&CPU::slo);
            break;
        case 0x1F:
            absolute_x(&CPU::slo);
            break;
        case 0x1B:
            absolute_y(&CPU::slo);
            break;
        case 0x03:
            indirect_x(&CPU::slo);
            break;
        case 0x13:
            indirect_y(&CPU::slo);
            break;
        //SRE
        case 0x47:
            zero_page(&CPU::sre);
            break;
        case 0x57:
            zero_page_x(&CPU::sre);
            break;
        case 0x4F:
            absolute(&CPU::sre);
            break;
        case 0x5F:
            absolute_x(&CPU::sre);
            break;
        case 0x5B:
            absolute_y(&CPU::sre);
            break;
        case 0x43:
            indirect_x(&CPU::sre);
            break;
        case 0x53:
            indirect_y(&CPU::sre);
            break;
        //TAS
        case 0x9B:
            absolute_y(&CPU::tas);
            break;
        //USBC
        case 0xEB:
            immediate(&CPU::usbc);
            break;

        default:
            if (!CYCLE_TESTING) {
                spdlog::critical("INVALID INSTRUCTION. OPCODE: 0x{:X}",opcode);
                spdlog::critical("0x{:X}  0x{:X}  0x{:X}           A:0x{:X} X:0x{:X} Y:0x{:X} SR:0x{:X} SP:0x{:X}",
                  registers.pc, opcode, current_operand, registers.ac, registers.x, registers.y, registers.sr, registers.sp);
                spdlog::critical("Halting CPU...");
                exit();
                exit(1);
            }
    }


}

