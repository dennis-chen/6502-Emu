#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cpu CPU;
typedef enum reg REG;
typedef enum flag FLAG;
typedef struct op_code_info OP_CODE_INFO;

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
void setZero(CPU *c,int8_t val);
void setSign(CPU *c,int8_t val);
void setOverflow(CPU *c,int8_t a,int8_t b,int8_t val);
void setCarryBCD(CPU *c,int16_t val);
void setCarry(CPU *c,int16_t val);
void write(CPU *c,int16_t addr,int8_t val);
int8_t read(CPU *c,int16_t addr);
int8_t getFlag(CPU *c,FLAG name);
void setFlag(CPU *c,FLAG name,int val);
int8_t getRegByte(CPU *c,REG name);
void setRegByte(CPU *c,REG name,int8_t val);
void print(CPU *c);
CPU *getCPU();
void setRegBit(CPU *c,REG name,int8_t bit,int8_t val);
