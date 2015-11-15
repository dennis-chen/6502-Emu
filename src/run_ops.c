#include <stdio.h>
#include "opcodes.h"
#include "load_prog.h"
#include "cpu.h"

void check_op(CPU *c, int16_t end) {
    OP_CODE_INFO *op = malloc(sizeof(OP_CODE_INFO));
    print(c);
    while (c->PC <= end) {
        //printf("%x\n", c->addressSpace[PC]);
        switch (c->addressSpace[c->PC] & 0xFF) {
            case ADC_IMM:
                op->operand = c->addressSpace[++(c->PC)];
                op->mode = Immediate;
                adc(c, op);
                break;
            case ADC_ZP:
                op->operand = c->addressSpace[c->addressSpace[++(c->PC)]];
                op->mode = ZeroPageAbsolute;
                adc(c, op);
                printf("adc zp\n");
                break;
            case LDX_IMM:
                op->operand = c->addressSpace[++(c->PC)];
                op->mode = Immediate;
                ldx(c, op);
                break;
            case STX_ZP:
                op->operand = c->addressSpace[++(c->PC)];
                op->mode = ZeroPageAbsolute;
                stx(c, op);
                printf("stx zp\n");
                break;
            case LDY_IMM:
                op->operand = c->addressSpace[++(c->PC)];
                op->mode = Immediate;
                ldy(c, op);
                printf("ldy imm %x\n", op->operand);
                break;
            case SEC:
                op->mode = Implied;
                sec(c, op);
                printf("SEC\n");
                break;
            case TYA:
                op->mode = Implied;
                tya(c, op);
                printf("TYA\n");
                break;
            case SBC_IMM:
                op->mode = Immediate;
                op->operand = c->addressSpace[++(c->PC)];
                sbc(c, op);
                printf("SBC imm\n");
                break;
            case TAY:
                op->mode = Implied;
                tay(c, op);
                printf("TAY\n");
                break;
            case CLC:
                op->mode = Implied;
                clc(c, op);
                printf("clc\n");
                break;
            case LDA_IMM:
                op->mode = Immediate;
                op->operand = c->addressSpace[++(c->PC)];
                lda(c, op);
                printf("LDA imm\n");
                break;
            case STA_ZP:
                op->mode = ZeroPageAbsolute;
                op->operand = c->addressSpace[++(c->PC)];
                sta(c, op);
                printf("sta zp\n");
                break;
            case LDX_ZP:
                op->mode = ZeroPageAbsolute;
                op->operand = c->addressSpace[c->addressSpace[++(c->PC)]];
                ldx(c, op);
                printf("ldx zp\n");
                break;
            case DEY:
                op->mode = Implied;
                dey(c, op);
                printf("dey\n");
                break;
            case BNE_REL:
                op->mode = Relative;
                bne(c, op);
                printf("bne rel\n");
                break;
            case SBC_ZPX:
                op->mode = ZeroPageIndexed;
                op->operand = c->addressSpace[getRegByte(c, IND_X)+c->addressSpace[++(c->PC)]];
                sbc(c, op);
                printf("sbc zpx\n");
                break;
            default:
                printf("op code probably not declared yet\n");
                break;
        }
        print(c);
        (c->PC)++;
    }
}


int main(int argc, char **argv) {
    CPU *c = getCPU();
    c->PC = 0x600;
    int16_t end = load_program(argc, argv, c->addressSpace, c->PC);
    if (c->PC == end)
        return 1;

    //for (int i = start; i < end; i++)
        //check_op(c->addressSpace[i]);
    check_op(c, end);
    printf("\n");

    return 0;
}
