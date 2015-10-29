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
    //initialize address space
    int ADDR_SPACE_SIZE = 65536;
    int8_t *a= malloc(sizeof(int8_t)*ADDR_SPACE_SIZE);
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

int8_t getRegByte(CPU *c, REG name){
    return c->regs[name];
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

int8_t getFlag(CPU *c, FLAG name){
    //bit 5 of the status register is not to be set
    //and should always be 1
    assert(name != NOT_USED_FLAG);
    int8_t flag =  c->regs[STATUS] & (1 << name) ? 1 : 0;
    return flag;
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

int8_t read(CPU *c, int16_t addr){
    //placeholder code 
    //TODO: replace this w/ actual implementation
    int8_t placeholder = c->addressSpace[addr];
    return placeholder;
}

void write(CPU *c, int16_t addr, int8_t val){
    c->addressSpace[addr] = val;
}

/* FLAG REG OPERATIONS */

void setCarry(CPU *c, int16_t val){
    //sets carry flag if
    //val > 0b11111111, largest
    //eight bit val
    int8_t carry = val > 0xFF ? 1 : 0;
    setFlag(c,V,carry);
}

void setCarryBCD(CPU *c, int16_t val){
    //sets carry flag if
    //val > 0x99, for addition in BCD mode
    int8_t carry = val > 0x99 ? 1 : 0;
    setFlag(c,V,carry);
}

void setOverflow(CPU *c, int8_t a, int8_t b, int8_t val){
    //sets overflow if overflow in twos complement 
    //occurred when adding a and b to get val
    //this bit twiddling from:
    //http://nesdev.com/6502.txt
    int8_t overflow = !((a ^ b) & 0x80) && ((a ^ val) & 0x80);
    //overflow = signs of operands are the same AND
    //          sign of result not equal to sign of operands
    setFlag(c,V,overflow);
}

void setSign(CPU *c, int8_t val){
    //sets sign flag equal to sign
    //of bit 7 of val
    int8_t sign = val & 0x80? 1 : 0;
    setFlag(c,S,sign);
}

void setZero(CPU *c, int8_t val){
    //sets zero flag to 0 if val == 0
    //and zero flag to 1 otherwise
    int8_t isZero = val? 1 : 0 ;
    setFlag(c,Z,isZero);
}

/* OP CODE IMPLEMENTATIONS HERE */

//Add with carry
void ADC(CPU *c, OP_CODE_INFO *o){
    int8_t carry = getFlag(c,C);
    int8_t accum = getRegByte(c,ACCUM);
    int8_t addrVal = read(c,o->address);
    int16_t sum = carry + accum + addrVal;
    setZero(c,sum);
    if(getFlag(c,D)){ //in decimal mode
        //if lower 4 bits of operands plus
        //the carry in are larger than 9,
        //then we need to apply conversions
        //to remain in binary coded decimal format.
        if((accum & 0xF) + (addrVal & 0xF)
            + carry > 9){
            sum += 6;
        }
        setSign(c,sum);
        setOverflow(c,accum,addrVal,sum);
        //if the higher bits aren't in
        //BCD format we need to add 96 to convert.
        //Black magic from http://nesdev.com/6502.txt
        sum += sum > 0x99 ? 96 : 0;
        setCarryBCD(c, sum);
    } else {
        setSign(c,sum);
        setOverflow(c,accum,addrVal,sum);
        setCarry(c,sum);
    }
    setRegByte(c,ACCUM,sum);
}

void AND(CPU *c, OP_CODE_INFO *o){
    int8_t accum = getRegByte(c,ACCUM);
    int8_t addrVal = read(c,o->address);
    int8_t res = accum & addrVal;
    setSign(c,res);
    setZero(c,res);
    setRegByte(c,ACCUM,res);
}

//Arithmetic shift left
void ASL(CPU *c, OP_CODE_INFO *o){
    int8_t addrVal = read(c,o->address);
    int16_t res = addrVal << 1;
    setCarry(c,res);
    setSign(c,res);
    setZero(c,res);
    if(o->mode == Accumulator){
        setRegByte(c,ACCUM,res);
    } else {
        write(c,o->address,res);
    }
}

//Branch if carry clear
void BCC(CPU *c, OP_CODE_INFO *o){
    if(!getFlag(c,C)){
        c->PC = o->address;
        //TODO: cpu add branch cycles here
    }
}

//Branch if carry set
void BCS(CPU *c, OP_CODE_INFO *o){
    if(getFlag(c,C)){
        c->PC = o->address;
        //TODO: cpu add branch cycles here
    }
}

//Branch if equals
void BEQ(CPU *c, OP_CODE_INFO *o){
    if(getFlag(c,Z)){
        c->PC = o->address;
        //TODO: cpu add branch cycles here
    }
}

//Branch on overflow set
void BVS(CPU *c, OP_CODE_INFO *o){
    if (getFlag(c, V)){
        c->PC = o->address;
        //TODO: add clk cycles here
    }
}

//Clear carry flag
void CLC(CPU *c, OP_CODE_INFO *o){
    setFlag(c, C, 0);
}

//Clear decimal mode
void CLD(CPU *c, OP_CODE_INFO *o){
    setFlag(c, D, 0);
}

//Clear interrupt disable bit
void CLI(CPU *c, OP_CODE_INFO *o){
    setFlag(c, I, 0);
}

//Clear overflow flag
void CLV(CPU *c, OP_CODE_INFO *o){
    setFlag(c, V, 0);
}

//Compare memory and accumulator
void CMP(CPU *c, OP_CODE_INFO *o){
    int8_t src = getRegByte(c, ACCUM) - o->address;
    setFlag(c, C, src < 0x100);
    setFlag(c, N, (src & 0x40));    // get 7th bit of src
    setFlag(c, Z, (src &= 0xff));
}

//Compare memory and index x
void CPX(CPU *c, OP_CODE_INFO *o){
    int8_t src = getRegByte(c, IND_X) - o->address;
    setFlag(c, C, src < 0x100);
    setFlag(c, N, (src & 0x40));    // get 7th bit of src
    setFlag(c, Z, (src &= 0xff));
}

//Compare memory and index y
void CPY(CPU *c, OP_CODE_INFO *o){
    int8_t src = getRegByte(c, IND_Y) - o->address;
    setFlag(c, C, src < 0x100);
    setFlag(c, N, (src & 0x40));    // get 7th bit of src
    setFlag(c, Z, (src &= 0xff));
}


int main ()
{
    CPU *c = getCPU();
    print(c);
    return 0;
}
