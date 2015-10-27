#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    //enums for 8 bit registers
    STATUS, STACK, ACCUM, IND_X, IND_Y
} REG;

typedef struct {
    //CPU registers
    int16_t PC;
    int8_t *regs;
    int8_t *addressSpace;
} CPU;

/* sets a single bit of one of the eight bit 
 * registers on CPU to val */
void setRegBit(CPU *c, REG name, int8_t bit, int8_t val){
    //bit must be between 0 and 7 since registers
    //are 8 bits
    assert(bit > -1 && bit < 8);
    //only setting one bit so val must be 0 or 1
    assert (val == 0 || val == 1);
    c->regs[name] = (c->regs[name] & ~(1 << bit)) | 
                    (val << bit);
}

/* CPU initializer */
CPU * getCPU(){
    CPU *c = malloc(sizeof(CPU));
    //initialize 8 bit registers
    int NUM_REG = 5;
    int8_t *r = malloc(sizeof(int8_t)*NUM_REG);
    c-> regs = r;
    int ADDR_SPACE_SIZE = 1; //TODO: change to actual size lol
    int8_t *a= malloc(sizeof(int8_t)*NUM_REG);
    c-> addressSpace = a;
    //set bit 5 of status register to 1
    //to match specifications
    setRegBit(c, STATUS, 5, 1);
    return c;
}

/* prints state of CPU registers */
void print(CPU *c){
    printf("PC: ");
    printf("%d\n", c->PC);
    printf("STATUS REG: ");
    printf("%d\n", c->regs[STATUS]);
    printf("STACK REG: ");
    printf("%d\n", c->regs[STACK]);
    printf("ACCUM REG: ");
    printf("%d\n", c->regs[ACCUM]);
    printf("IND_X REG: ");
    printf("%d\n", c->regs[IND_X]);
    printf("IND_Y REG: ");
    printf("%d\n", c->regs[IND_Y]);
}

/* set byte value in one of the eight bit 
 * registers to val */
void setRegByte(CPU *c, REG name, int8_t val){
    c->regs[name] = val;
}

typedef enum {
    //enums specifying flag bits of the status register
    C, Z, I, D, B, NOT_USED_FLAG, V, S
} FLAG;

void setFlag(CPU *c, FLAG name, int val){
    //bit 5 of the status register is not to be set
    //and should always be 1
    assert(name != NOT_USED_FLAG);
    setRegBit(c, STATUS, name, val);
}

typedef enum {
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

typedef struct {
    //holds information that op codes
    //need to execute
    int16_t address;
    int16_t PC;
    MODE mode;
} OP_CODE_INFO;

/* OP CODE IMPLEMENTATIONS HERE */

/*
void ADC(CPU *c, OP_CODE_INFO *o){
    //implementation for ADC here. Note that function name 
    //should match Op code name
}
*/

int main ()
{
    CPU *c = getCPU();
    print(c);
    return 0;
}
