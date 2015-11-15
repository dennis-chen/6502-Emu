#include <stdio.h>
#include "opcodes.h"
#include "load_prog.h"
#include "cpu.h"

void check_op(CPU *c, int16_t end) {
    while (c->PC <= end) {
        //printf("%x\n", c->addressSpace[PC]);
        switch (c->addressSpace[c->PC] & 0xFF) {
            case ADC_IMM:
                printf("adc imm %x\n", c->addressSpace[++(c->PC)]);
                break;
            case ADC_ZP:
                printf("adc zp\n");
                break;
            case LDX_IMM:
                printf("ldx imm %x\n", c->addressSpace[++(c->PC)]);
                break;
            case STX_ZP:
                printf("stx zp %x\n", c->addressSpace[++(c->PC)]);
                break;
            case LDY_IMM:
                printf("ldy imm %x\n", c->addressSpace[++(c->PC)]);
                break;
            case SEC:
                printf("SEC\n");
                break;
            case TYA:
                printf("TYA\n");
                break;
            case TAY:
                printf("TAY\n");
                break;
            case LDA_IMM:
                printf("LDA imm\n");
                break;
            case STA_ZP:
                printf("sta zp\n");
                break;
            case LDX_ZP:
                printf("ldx zp\n");
                break;
            case DEY:
                printf("dey\n");
                break;
            case BNE_REL:
                printf("bne rel\n");
                break;
            case SBC_ZPX:
                printf("sbc zpx\n");
                break;
            default:
                printf("op code probably not declared yet\n");
                break;
        }
        (c->PC)++;
    }
}


int main(int argc, char **argv) {
    CPU *c = getCPU();
    int16_t end = load_program(argc, argv, c->addressSpace, c->PC);
    if (c->PC == end)
        return 1;

    //for (int i = start; i < end; i++)
        //check_op(c->addressSpace[i]);
    check_op(c, end);
    printf("\n");

    return 0;
}
