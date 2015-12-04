#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef enum reg {
    //enums for 8 bit registers
    STATUS, STACK, ACCUM, IND_X, IND_Y
} REG;

typedef struct cpu {
    //CPU registers
    uint16_t PC;
    int8_t *regs;
    int8_t *addressSpace;
} CPU; 

typedef enum mode {
    UNUSED,
    modeAbsolute,
    modeAbsoluteX,
    modeAbsoluteY,
    modeAccumulator,
    modeImmediate,
    modeImplied,
    modeIndexedIndirect,
    modeIndirect,
    modeIndirectIndexed,
    modeRelative,
    modeZeroPage,
    modeZeroPageX,
    modeZeroPageY
} MODE;

typedef struct op_code_info {
    //holds information that op codes
    //need to execute
    int8_t operand;
    //address that operand is fetched
    //from is needed for operations
    //like LSR
    uint16_t address;
    MODE mode;
} OP_CODE_INFO;

typedef enum flag {
    //enums specifying flag bits of the status register
    // C = carry
    // Z = zero
    // I = interrupt enable
    // D = decimal mode
    // B = enabled on BRK
    // V = overflow
    // S = sign
    C, Z, I, D, B, NOT_USED_FLAG, V, S
} FLAG;

int main();
void lsr(CPU *c,OP_CODE_INFO *o);
void ldy(CPU *c,OP_CODE_INFO *o);
void ldx(CPU *c,OP_CODE_INFO *o);
void lda(CPU *c,OP_CODE_INFO *o);
void jsr(CPU *c,OP_CODE_INFO *o);
void jmp(CPU *c,OP_CODE_INFO *o);
void cpy(CPU *c,OP_CODE_INFO *o);
void cpx(CPU *c,OP_CODE_INFO *o);
void cmp(CPU *c,OP_CODE_INFO *o);
void clv(CPU *c,__attribute__((unused))OP_CODE_INFO *o);
void cli(CPU *c,__attribute__((unused))OP_CODE_INFO *o);
void cld(CPU *c,__attribute__((unused))OP_CODE_INFO *o);
void clc(CPU *c,__attribute__((unused))OP_CODE_INFO *o);
void dec(CPU *c, OP_CODE_INFO *o);
void dex(CPU *c, OP_CODE_INFO *o);
void dey(CPU *c, OP_CODE_INFO *o);
void fut(CPU *c, OP_CODE_INFO *o);
void inc(CPU *c, OP_CODE_INFO *o);
void inx(CPU *c, OP_CODE_INFO *o);
void bvs(CPU *c,OP_CODE_INFO *o);
void bvc(CPU *c,OP_CODE_INFO *o);
void brk(CPU *c,OP_CODE_INFO *o);
void bpl(CPU *c,OP_CODE_INFO *o);
void bne(CPU *c,OP_CODE_INFO *o);
void bmi(CPU *c,OP_CODE_INFO *o);
void bit(CPU *c,OP_CODE_INFO *o);
void beq(CPU *c,OP_CODE_INFO *o);
void bcs(CPU *c,OP_CODE_INFO *o);
void bcc(CPU *c,OP_CODE_INFO *o);
void asl(CPU *c,OP_CODE_INFO *o);
void and(CPU *c,OP_CODE_INFO *o);
void adc(CPU *c,OP_CODE_INFO *o);
void nop(CPU *c, OP_CODE_INFO *o);
void ora(CPU *c, OP_CODE_INFO *o);
void pha(CPU *c, OP_CODE_INFO *o);
void php(CPU *c, OP_CODE_INFO *o);
void pla(CPU *c, OP_CODE_INFO *o);
void plp(CPU *c, OP_CODE_INFO *o);
void sbc(CPU *c, OP_CODE_INFO *o);
void sec(CPU *c, OP_CODE_INFO *o);
void sta(CPU *c, OP_CODE_INFO *o);
void stx(CPU *c, OP_CODE_INFO *o);
void tay(CPU *c, OP_CODE_INFO *o);
void tya(CPU *c, OP_CODE_INFO *o);
void txa(CPU *c, OP_CODE_INFO *o);
void PUSH(CPU *c,int8_t operand);
int8_t PULL(CPU *c);
void setZero(CPU *c,int8_t val);
void setSign(CPU *c,int8_t val);
void setOverflow(CPU *c,int8_t a,int8_t b,int8_t val);
void setCarryBCD(CPU *c,int16_t val);
void setCarry(CPU *c,int16_t val);
void write(CPU *c,uint16_t addr,int8_t val);
int8_t read(CPU *c,uint16_t addr);
int8_t getFlag(CPU *c,FLAG name);
void setFlag(CPU *c,FLAG name,int8_t val);
int8_t getRegByte(CPU *c,REG name);
void setRegByte(CPU *c,REG name,int8_t val);
void print(CPU *c);
void printAddressLine(CPU *c, uint16_t beg);
void printAddressSpace(CPU *c, uint16_t beg, uint16_t end);
char * getStatus(CPU *c);
CPU *getCPU();
void resetCPU(CPU *c);
void freeCPU(CPU *c);
OP_CODE_INFO *getOP_CODE_INFO(int8_t operand, uint16_t address, MODE mode);
void freeOP_CODE_INFO(OP_CODE_INFO *o);
void setRegBit(CPU *c,REG name,int8_t bit,int8_t val);
int8_t getRegBit(CPU *c,REG name,int8_t bit);
char *byte_to_binary(int x);
void run_ops(CPU *c, int16_t end);
void run_op(CPU *c);

void eor(CPU *c, OP_CODE_INFO *o);
void rol(CPU *c, OP_CODE_INFO *o);
void ror(CPU *c, OP_CODE_INFO *o);
void rts(CPU *c, OP_CODE_INFO *o);
void rti(CPU *c, OP_CODE_INFO *o);
void sei(CPU *c, OP_CODE_INFO *o);
void sty(CPU *c, OP_CODE_INFO *o);
void txs(CPU *c, OP_CODE_INFO *o);
void tax(CPU *c, OP_CODE_INFO *o);
void tsx(CPU *c, OP_CODE_INFO *o);
void iny(CPU *c, OP_CODE_INFO *o);
void sed(CPU *c, OP_CODE_INFO *o);

extern uint8_t instructionSizes[256];
extern uint8_t instructionModes[256];
extern char instructionNames[256][4];
extern void (*opcodeToFunction[256])(CPU *c, OP_CODE_INFO *o);

#endif
