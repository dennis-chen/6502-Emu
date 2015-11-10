#include <stdio.h>
#include "opcodes.h"
#include "exec.h"

void check_op(uint8_t *mem, int16_t start, int16_t end) {
    int16_t pc = start;
    while (pc <= end) {
        switch (mem[pc]) {
            case ADC_IMM:
                printf("adc imm %x\n", mem[++pc]);
                break;
            case ADC_ZP:
                printf("adc zp\n");
                break;
            case LDX_IMM:
                printf("ldx imm %x\n", mem[++pc]);
                break;
            case STX_ZP:
                printf("stx zp %x\n", mem[++pc]);
                break;
            case LDY_IMM:
                printf("ldy imm %x\n", mem[++pc]);
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
        pc++;
    }
}


int main(int argc, char **argv) {
    uint8_t mem[0xffff];
    int16_t start = 0x60;
    int16_t end = load_program(argc, argv, mem, start);
    if (start == end)
        return 1;

    //for (int i = start; i < end; i++)
        //check_op(mem[i]);
    check_op(mem, start, end);
    printf("\n");

    return 0;
}
