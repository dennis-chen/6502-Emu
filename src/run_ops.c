#include <stdio.h>
#include "opcodes.h"
#include "load_prog.h"
#include "cpu.h"

void check_ops(CPU *c, int16_t end) {
    OP_CODE_INFO *op = malloc(sizeof(OP_CODE_INFO));
    int16_t start = c->PC;
    int16_t *pc = &(c->PC);
    print(c);
    while (c->PC < end){
        switch (c->addressSpace[*pc] & 0xFF) {
            case ADC_IMM:
                op->operand = c->addressSpace[++(*pc)];
                op->mode = Immediate;
                adc(c, op);
                (*pc)++;
                break;

            case ADC_ZP:
                op->operand = c->addressSpace[c->addressSpace[++(*pc)]];
                op->mode = ZeroPageAbsolute;
                adc(c, op);
                (*pc)++;
                break;

            case LDX_IMM:
                op->operand = c->addressSpace[++(*pc)];
                op->mode = Immediate;
                ldx(c, op);
                (*pc)++;
                break;

            case STX_ZP:
                op->address = 0x00FF & c->addressSpace[++(*pc)];
                op->mode = ZeroPageAbsolute;
                stx(c, op);
                (*pc)++;
                break;

            case LDY_IMM:
                op->operand = c->addressSpace[++(*pc)];
                op->mode = Immediate;
                ldy(c, op);
                (*pc)++;
                break;

            case SEC:
                op->mode = Implied;
                sec(c, op);
                (*pc)++;
                break;

            case TYA:
                op->mode = Implied;
                tya(c, op);
                (*pc)++;
                break;

            case SBC_IMM:
                op->mode = Immediate;
                op->operand = c->addressSpace[++(*pc)];
                sbc(c, op);
                (*pc)++;
                break;

            case TAY:
                op->mode = Implied;
                tay(c, op);
                (*pc)++;
                break;

            case CLC:
                op->mode = Implied;
                clc(c, op);
                (*pc)++;
                break;

            case LDA_IMM:
                op->mode = Immediate;
                op->operand = c->addressSpace[++(*pc)];
                lda(c, op);
                (*pc)++;
                break;

            case STA_ZP:
                op->mode = ZeroPageAbsolute;
                op->address = 0x00FF & c->addressSpace[++(*pc)];
                sta(c, op);
                (*pc)++;
                break;

            case LDX_ZP:
                op->mode = ZeroPageAbsolute;
                op->operand = c->addressSpace[0x00FF & c->addressSpace[++(*pc)]];
                ldx(c, op);
                (*pc)++;
                break;

            case DEY:
                op->mode = Implied;
                dey(c, op);
                (*pc)++;
                break;

            case BNE_REL:
                op->mode = Relative;
                (*pc)++;
                op->address = (int) *pc + (int) c->addressSpace[*pc];
                bne(c, op);
                (*pc)++;
                break;

            case SBC_ZPX:
                op->mode = ZeroPageIndexed;
                op->operand = c->addressSpace[getRegByte(c, IND_X)+c->addressSpace[++(*pc)]];
                sbc(c, op);
                (*pc)++;
                break;

            default:
                printf("op code probably not declared yet\n");
                (*pc)++;
                break;
        }
    }
    printf("\n");
    print(c);
}


int main(int argc, char **argv) {
    CPU *c = getCPU();
    c->PC = 0x600;
    int16_t end = load_program(argc, argv, c->addressSpace, c->PC);
    if (c->PC == end)
        return 1;

    check_ops(c, end);
    printf("\n");

    return 0;
}
