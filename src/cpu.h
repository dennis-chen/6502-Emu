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
    Immediate,
    Absolute,
    ZeroPageAbsolute,
    Implied,
    Accumulator,
    Indexed,
    ZeroPageIndexed,
    Indirect,
    PreIndexedIndirect,
    PostIndexedIndirect,
    Relative
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
void LSR(CPU *c,OP_CODE_INFO *o);
void LDY(CPU *c,OP_CODE_INFO *o);
void LDX(CPU *c,OP_CODE_INFO *o);
void LDA(CPU *c,OP_CODE_INFO *o);
void JSR(CPU *c,OP_CODE_INFO *o);
void JMP(CPU *c,OP_CODE_INFO *o);
void CPY(CPU *c,OP_CODE_INFO *o);
void CPX(CPU *c,OP_CODE_INFO *o);
void CMP(CPU *c,OP_CODE_INFO *o);
void CLV(CPU *c,__attribute__((unused))OP_CODE_INFO *o);
void CLI(CPU *c,__attribute__((unused))OP_CODE_INFO *o);
void CLD(CPU *c,__attribute__((unused))OP_CODE_INFO *o);
void CLC(CPU *c,__attribute__((unused))OP_CODE_INFO *o);
void DEY(CPU *c, OP_CODE_INFO *o);
void BVS(CPU *c,OP_CODE_INFO *o);
void BVC(CPU *c,OP_CODE_INFO *o);
void BPL(CPU *c,OP_CODE_INFO *o);
void BNE(CPU *c,OP_CODE_INFO *o);
void BMI(CPU *c,OP_CODE_INFO *o);
void BIT(CPU *c,OP_CODE_INFO *o);
void BEQ(CPU *c,OP_CODE_INFO *o);
void BCS(CPU *c,OP_CODE_INFO *o);
void BCC(CPU *c,OP_CODE_INFO *o);
void ASL(CPU *c,OP_CODE_INFO *o);
void AND(CPU *c,OP_CODE_INFO *o);
void ADC(CPU *c,OP_CODE_INFO *o);
void PUSH(__attribute__((unused))CPU *c,__attribute__((unused))OP_CODE_INFO *o,__attribute__((unused))int8_t operand);
void NOP(CPU *c, OP_CODE_INFO *o);
void ORA(CPU *c, OP_CODE_INFO *o);
void PHA(CPU *c, OP_CODE_INFO *o);
void PHP(CPU *c, OP_CODE_INFO *o);
void PLA(CPU *c, OP_CODE_INFO *o);
void PLP(CPU *c, OP_CODE_INFO *o);
void SBC(CPU *c, OP_CODE_INFO *o);
void SEC(CPU *c, OP_CODE_INFO *o);
void STA(CPU *c, OP_CODE_INFO *o);
void STX(CPU *c, OP_CODE_INFO *o);
void TAY(CPU *c, OP_CODE_INFO *o);
void TYA(CPU *c, OP_CODE_INFO *o);
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
char * getStatus(CPU *c);
CPU *getCPU();
void resetCPU(CPU *c);
void freeCPU(CPU *c);
OP_CODE_INFO *getOP_CODE_INFO(int8_t operand, uint16_t address, MODE mode);
void freeOP_CODE_INFO(OP_CODE_INFO *o);
void setRegBit(CPU *c,REG name,int8_t bit,int8_t val);
int8_t getRegBit(CPU *c,REG name,int8_t bit);
char *byte_to_binary(int x);
