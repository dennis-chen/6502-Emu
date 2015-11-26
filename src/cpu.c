#include <time.h>
#include "cpu.h"
#include "opcodes.h"
#include "load_prog.h"
#include "gfx.h"
#include "keyboard.h"

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

int8_t getRegBit(CPU *c, REG name, int8_t bit){
    //bit must be between 0 and 7 since registers
    //are 8 bits
    assert(bit > -1 && bit < 8);
    return (c->regs[name] & (1 << bit)) > 0 ? 1 : 0;
}

/* CPU initializer */
CPU * getCPU(){
    CPU *c = malloc(sizeof(CPU));
    c-> PC = 0;
    //initialize 8 bit registers
    int NUM_REG = 5;
    int8_t *r = calloc(NUM_REG,sizeof(int8_t));
    c-> regs = r;
    //initialize address space
    int ADDR_SPACE_SIZE = 65536;
    int8_t *a= calloc(ADDR_SPACE_SIZE,sizeof(int8_t));
    c-> addressSpace = a;
    //set bit 5 of status register to 1
    //to match specifications
    setRegBit(c, STATUS, 5, 1);
    //initialize stack pointer to 256 (it grows downwards
    //towards zero)
    setRegByte(c, STACK, 0xFF);
    return c;
}

void resetCPU(CPU *c){
    //resets CPU to initial state as if it has just been initialized
    int NUM_REG = 5;
    memset(c->regs, 0, NUM_REG*sizeof(int8_t));
    int ADDR_SPACE_SIZE = 65536;
    memset(c->addressSpace, 0, ADDR_SPACE_SIZE*sizeof(int8_t));
    c->PC = 0;
    setRegBit(c, STATUS, 5, 1);
}

void freeCPU(CPU *c){
    free(c->regs);
    free(c->addressSpace);
    free(c);
}

/* prints state of CPU registers */
void print(CPU *c){
    printf("PC: ");
    printf("%x\n", c->PC);
    printf("SVUBDIZC\n");
    printf("%s\n",getStatus(c));
    printf("STACK REG: ");
    uint8_t stackVal = 0xFF & c->regs[STACK];
    printf("%x\n", stackVal);
    printf("ACCUM REG: ");
    uint8_t accumVal = 0xFF & c->regs[ACCUM];
    printf("%x\n", accumVal);
    printf("IND_X REG: ");
    uint8_t xVal = 0xFF & c->regs[IND_X];
    printf("%x\n", xVal);
    printf("IND_Y REG: ");
    uint8_t yVal = 0xFF & c->regs[IND_Y];
    printf("%x\n", yVal);
}

void printAddressLine(CPU *c, uint16_t beg){
    //prints 16 hex vals starting from beg
    uint16_t counter = beg;
    while(counter < (beg + 16) && counter < 65536){
        printf(" %02x", c->addressSpace[counter]&0xFF);
        counter++;
    }
    printf("\n");
}

void printAddressSpace(CPU *c, uint16_t beg, uint16_t end){
    //prints address space from beg to end
    //prints 16 8-bit vals at a time as hex
    uint16_t counter = beg;
    while(counter < end){
        printf("%03x: ",counter);
        printAddressLine(c, counter);
        counter += 16;
    }
}

char *getStatus(CPU *c){
    return byte_to_binary(c->regs[STATUS]);
}

char *byte_to_binary(int x){
    char *b = malloc(sizeof(char)*9);
    b[0] = '\0';
    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }
    return b;
}

/* set byte value in one of the eight bit 
 * registers to val */
void setRegByte(CPU *c, REG name, int8_t val){
    c->regs[name] = val;
}

int8_t getRegByte(CPU *c, REG name){
    return c->regs[name];
}

void setFlag(CPU *c, FLAG name, int8_t val){
    //bit 5 of the status register is not to be set
    //and should always be 1
    assert(name != NOT_USED_FLAG);
    setRegBit(c, STATUS, name, val);
}

int8_t getFlag(CPU *c, FLAG name){
    //bit 5 of the status register is not to be set
    //and should always be 1
    assert(name != NOT_USED_FLAG);
    return getRegBit(c, STATUS, name);
}

int8_t read(CPU *c, uint16_t addr){
    return c->addressSpace[addr];
}

void write(CPU *c, uint16_t addr, int8_t val){
    c->addressSpace[addr] = val;
}

/* FLAG REG OPERATIONS */

void setCarry(CPU *c, int16_t val){
    //sets carry flag if
    //val > 0b11111111, largest
    //eight bit val
    int8_t carry = val > 0xFF ? 1 : 0;
    setFlag(c,C,carry);
}

void setCarryBCD(CPU *c, int16_t val){
    //sets carry flag if
    //val > 0x99, for addition in BCD mode
    int8_t carry = val > 0x99 ? 1 : 0;
    setFlag(c,C,carry);
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

void setOverflowSubtract(CPU *c, int8_t a, int8_t b, int8_t val){
    //sets overflow if overflow in twos complement 
    //occurred when subtracting b from a to get val
    //this bit twiddling from:
    //http://nesdev.com/6502.txt
    int8_t overflow = ((a ^ b) & 0x80) && ((a ^ val) & 0x80);
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
    int8_t isZero = val? 0 : 1 ;
    setFlag(c,Z,isZero);
}

OP_CODE_INFO * getOP_CODE_INFO(int8_t operand, uint16_t address, MODE mode){
    OP_CODE_INFO *o = malloc(sizeof(OP_CODE_INFO));
    o->operand = operand;
    o->address = address;
    o->mode = mode;
    return o;
}

void freeOP_CODE_INFO(OP_CODE_INFO *o){
    free(o);
}

void wait() {
    char enter;
    printf("Press enter to proceed...\n");
    while ((enter = getchar()) != '\n') { /* wait until enter is pressed */ }
}

void drawPoint(int x, int y){
    //draws point onto 'pixel' (actually a 8 by 8 square)
    int xMin = x * 8;
    int xMax = x * 8 + 8;
    int yMin = y * 8;
    int yMax = y * 8 + 8;
    int i;
    int j;
    for(i = xMin; i < xMax; i++){
        for(j = yMin; j < yMax; j++){
            gfx_point(i,j);
        }
    }
}

void initializegfx(){
    int ysize = 256;
    int xsize = 256;
    gfx_open(xsize,ysize,"Snake");
    gfx_color(255,255,255);
    return;
}

void visualizeMemory(CPU *c){
    //visualizes memory addresses $0200 to $05ff
    //(32 by 32 bytes)
    //as a 256 by 256 pixel square
    int x = 0;
    int y = 0;
    int i;
    for(i = 0x0200; i < 0x0600; i++){
        uint8_t memVal = c->addressSpace[i];
        if(memVal == 1){
            //in the snake game a memory val
            //of 1 represents a white color.
            //this special if statement is
            //because 1 needs to be mapped
            //to (255,255,255) in RGB
            gfx_color(255,255,255);
        } else if (memVal > 0){
            //this is to convert between
            //a memVal between 0-255 and
            //three 0-255 vals representing R,G,B
            //red is the first 3 bits
            uint8_t red = memVal & 0xE0;
            //blue is the next 3 bits
            uint8_t blue = (memVal << 3) & 0xE0;
            //green is the last 2
            uint8_t green = (memVal << 6) & 0xE0;
            gfx_color(red,blue,green);
        } else {
            gfx_color(0,0,0);
        }
        drawPoint(x,y);
        x++;
        if(x == 32){
            x = 0;
            y++;
        }
    }
    gfx_flush();
}

void getKeyboardInput(CPU *c){
    //reads keyboard input and
    //sets memory location $ff
    //to match keyboard input
    int dir = getDirection();
    switch(dir){
        case 0:
            //printf("left");
            c->addressSpace[0xff] = 0x61;
            break;
        case 1:
            //printf("right");
            c->addressSpace[0xff] = 0x64;
            break;
        case 2:
            //printf("up");
            c->addressSpace[0xff] = 0x77;
            break;
        case 3:
            //printf("down");
            c->addressSpace[0xff] = 0x73;
            break;
    }
}

void getRandomVal(CPU *c){
    //loads a random byte into $FE of
    //the cpu's memory (this is specifically
    //to get the snake game implementation
    //working and is NOT something the 6502
    //actually ever does
    uint8_t randNum = rand();
    c->addressSpace[0xfe] = randNum;
}

void initializerng(){
    //initialize random number generator
    srand(time(NULL));
}

/* RUN PROGRAM IN MEMORY */
void run_ops(CPU *c, int16_t end) {
    initializegfx();
    initializerng();
    char k;
    //TODO: line below for debugging only, remove when done
    c->addressSpace[0xFF] = 0x64; //set direction key to be right
    while (c->PC < end){
        getKeyboardInput(c);
        getRandomVal(c);
        run_op(c);
        visualizeMemory(c);
        //print(c);
        //printAddressSpace(c,0x400,0x420);
    }
}

void run_op(CPU *c){
    //runs a single operation based on whatever
    //is loaded into CPU memory, changing
    //hardware state appropriately.
    uint8_t opCode = c->addressSpace[c->PC];
    if(strcmp(instructionNames[opCode],"FUT") == 0){
        printf("Future opcode not implemented yet!");
        assert(0);
    }
    OP_CODE_INFO *o;
    //printf("%s\n",instructionNames[opCode]);
    uint8_t mode = instructionModes[opCode];
    uint16_t address;
    int8_t operand;
    switch(mode){
        case UNUSED:
            printf("Error! unused instruction mode!");
            assert(0);
            break;
        case modeAbsolute:
            //piece together 16 bit address from 2 bytes
            //following op code
            {
                //curly braces to allow for variable declaration
                //inside of case statement
                uint8_t lowerByte = c->addressSpace[c->PC+1];
                uint8_t upperByte = c->addressSpace[c->PC+2];
                uint16_t address = (upperByte << 8) | lowerByte;
                o = getOP_CODE_INFO(0, address, mode);
            }
            break;
        case modeAbsoluteX:
            {
                uint8_t lowerByte = c->addressSpace[c->PC+1];
                uint8_t upperByte = c->addressSpace[c->PC+2];
                uint16_t address = (upperByte << 8) | lowerByte;
                uint16_t xVal = getRegByte(c,IND_X);
                o = getOP_CODE_INFO(0, address+xVal, mode);
            }
            break;
        case modeAbsoluteY:
            {
                uint8_t lowerByte = c->addressSpace[c->PC+1];
                uint8_t upperByte = c->addressSpace[c->PC+2];
                uint16_t address = (upperByte << 8) | lowerByte;
                uint16_t yVal = getRegByte(c,IND_Y);
                o = getOP_CODE_INFO(0, address+yVal, mode);
            }
            break;
        case modeAccumulator:
            address = 0; //ADDRESS IS NOT APPLICABLE IN THIS MODE
            operand = getRegByte(c,ACCUM);
            o = getOP_CODE_INFO(operand, address, mode);
            break;
        case modeImmediate:
            address = c->PC + 1;
            operand = c->addressSpace[address];
            o = getOP_CODE_INFO(operand, address, mode);
            break;
        case modeImplied:
            address = c->PC;
            o = getOP_CODE_INFO(0, address, mode);
            break;
        case modeIndexedIndirect:
            {
                uint16_t immediateVal = c->addressSpace[c->PC + 1];
                uint16_t xVal = getRegByte(c,IND_X);
                uint8_t lowerByte = c->addressSpace[immediateVal+xVal];
                uint8_t upperByte = c->addressSpace[immediateVal+xVal+1];
                address = (upperByte << 8) | lowerByte;
                operand = c->addressSpace[address];
                o = getOP_CODE_INFO(operand, address, mode);
            }
            break;
        case modeIndirect:
            {
                uint8_t lowerByte = c->addressSpace[c->PC+1];
                uint8_t upperByte = c->addressSpace[c->PC+2];
                uint16_t address = (upperByte << 8) | lowerByte;
                uint8_t l = c->addressSpace[address];
                uint8_t u = c->addressSpace[address+1];
                uint16_t finalAddress = (u << 8) | l;
                o = getOP_CODE_INFO(0, finalAddress, mode);
            }
            break;
        case modeIndirectIndexed:
            {
                uint16_t immediateVal = c->addressSpace[c->PC + 1];
                uint8_t lowerByte = c->addressSpace[immediateVal];
                uint8_t upperByte = c->addressSpace[immediateVal+1];
                uint16_t yVal = getRegByte(c,IND_Y);
                address = ((upperByte << 8) | lowerByte) + yVal;
                operand = c->addressSpace[address];
                o = getOP_CODE_INFO(operand, address, mode);
            }
            break;
        case modeRelative:
            //for branching within +-128 
            {
                int16_t offset = c->addressSpace[c->PC + 1];
                address = c->PC + 2 + offset;
                o = getOP_CODE_INFO(0, address, mode);
            }
            break;
        case modeZeroPage:
            address = 0x00FF & c->addressSpace[c->PC + 1];
            operand = c->addressSpace[address];
            o = getOP_CODE_INFO(operand, address, mode);
            break;
        case modeZeroPageX:
            {
                address = 0x00FF & c->addressSpace[c->PC + 1];
                int8_t xVal = getRegByte(c,IND_X);
                address += xVal;
                operand = c->addressSpace[address];
                o = getOP_CODE_INFO(operand, address, mode);
            }
            break;
        case modeZeroPageY:
            assert(0);
            break;
    }
    c->PC += instructionSizes[opCode];
    opcodeToFunction[opCode](c,o);
    freeOP_CODE_INFO(o);
}

/* STACK OPERATIONS HERE */
void PUSH(CPU *c, int8_t operand){
    uint8_t stackVal = getRegByte(c, STACK);
    //0x0100 hardcoded in because stack
    //lives in 0x0100 to 0x01FF area of 
    //memory
    uint16_t address = 0x0100 | stackVal;
    write(c, address, operand);
    uint8_t newStackVal = stackVal - 1;
    setRegByte(c, STACK, newStackVal);
}

int8_t PULL(CPU *c){
    uint8_t stackVal = getRegByte(c, STACK);
    uint8_t newStackVal = stackVal + 1;
    setRegByte(c, STACK, newStackVal);
    //0x0100 hardcoded in because stack
    //lives in 0x0100 to 0x01FF area of 
    //memory
    uint16_t address = 0x0100 | newStackVal;
    return read(c, address);
}

/* OP CODE IMPLEMENTATIONS HERE */

//Add with carry
void adc(CPU *c, OP_CODE_INFO *o){
    int8_t carry = getFlag(c,C);
    int8_t accum = getRegByte(c,ACCUM);
    int8_t operand = o->operand;
    int16_t sum = (0x00FF&carry) + (0x00FF&accum) + (0x00FF&operand);
    int8_t sumByte = sum & 0x00FF;
    setZero(c,sumByte);
    if(getFlag(c,D)){ //in decimal mode
        //if lower 4 bits of operands plus
        //the carry in are larger than 9,
        //then we need to apply conversions
        //to remain in binary coded decimal format.
        if((accum & 0xF) + (operand & 0xF)
            + carry > 9){
            sum += 6;
        }
        setSign(c,sum&0xFF);
        setOverflow(c,accum,operand,sum&0xFF);
        //if the higher bits aren't in
        //BCD format we need to add 96 to convert.
        //Black magic from http://nesdev.com/6502.txt
        sum += sum > 0x99 ? 96 : 0;
        setCarryBCD(c, sum);
    } else {
        setSign(c,sumByte);
        setOverflow(c,accum,operand,sumByte);
        setCarry(c,sum);
    }
    setRegByte(c,ACCUM,sum&0xFF);
}

void and(CPU *c, OP_CODE_INFO *o){
    int8_t accum = getRegByte(c,ACCUM);
    int8_t operand = o->operand;
    int8_t res = accum & operand;
    setSign(c,res);
    setZero(c,res);
    setRegByte(c,ACCUM,res);
}

//Arithmetic shift left
void asl(CPU *c, OP_CODE_INFO *o){
    int8_t operand = o->operand;
    int16_t res = (0x00FF&operand) << 1;
    int8_t resByte = res & 0x00FF;
    setCarry(c,res);
    setSign(c,resByte);
    setZero(c,resByte);
    if(o->mode == modeAccumulator){
        setRegByte(c,ACCUM,res);
    } else {
        write(c,o->address,res);
    }
}

//Branch if carry clear
void bcc(CPU *c, OP_CODE_INFO *o){
    if(!getFlag(c,C)){
        c->PC = o->address;
        //TODO: cpu add branch cycles here
    }
}

//Branch if carry set
void bcs(CPU *c, OP_CODE_INFO *o){
    if(getFlag(c,C)){
        c->PC = o->address;
        //TODO: cpu add branch cycles here
    }
}

//Branch if equals
void beq(CPU *c, OP_CODE_INFO *o){
    if(getFlag(c,Z)){
        c->PC = o->address;
        //TODO: cpu add branch cycles here
    }
}

// Test bits in memory with accumulator
void bit(CPU *c, OP_CODE_INFO *o){
    int8_t src = o->operand;
    int8_t accum = getRegByte(c, ACCUM);
    setFlag(c, V, (src & 0x40) ? 1 : 0); // get 6th bit of src
    setFlag(c, S, (src & 0x80) ? 1 : 0); // get 7th bit of src
    setFlag(c, Z, (src & accum) ? 0 : 1);
}

// Branch if result minus
void bmi(CPU *c, OP_CODE_INFO *o){
    if (getFlag(c, S)) {
        c->PC = o->address;
        //TODO: cpu add branch cycles here
    }
}

// Branch if not equals
void bne(CPU *c, OP_CODE_INFO *o){
    if(!(getFlag(c,Z))){
        c->PC = o->address;
        //TODO: cpu add branch cycles here
    }
}

// Branch if result plus
void bpl(CPU *c, OP_CODE_INFO *o){
    if(!(getFlag(c,S))){
        c->PC = o->address;
        //TODO: cpu add branch cycles here
    }
}

// TODO BRK
// Force Break
void brk(CPU *c, OP_CODE_INFO *o){
    (c->PC)++; // this could be between 0 and 3.
    PUSH(c, (c->PC >> 8) & 0xff); // Push return address onto the stack.
    PUSH(c, c->PC & 0xff); // (push Program Counter AND 0xFF)
    setFlag(c, B, 1);
    PUSH(c, getRegByte(c, STATUS)); // (push status register onto top of stack)
    setFlag(c, I, 1);
    uint8_t lowerByte = c->addressSpace[0xFFFE];
    uint8_t upperByte = c->addressSpace[0xFFFF];
    uint16_t returnAddress = (upperByte << 8) | lowerByte;
    //c->PC = returnAddress;
}

// Branch if overflow clear
void bvc(CPU *c, OP_CODE_INFO *o){
    if (!(getFlag(c, V))){
        c->PC = o->address;
        //TODO: add clk cycles here
    }
}

//Branch on overflow set
void bvs(CPU *c, OP_CODE_INFO *o){
    if (getFlag(c, V)){
        c->PC = o->address;
        //TODO: add clk cycles here
    }
}

//Clear carry flag
void clc(CPU *c, __attribute__ ((unused)) OP_CODE_INFO *o){
    setFlag(c, C, 0);
}

//Clear decimal mode
void cld(CPU *c, __attribute__ ((unused)) OP_CODE_INFO *o){
    setFlag(c, D, 0);
}

//Clear interrupt disable bit
void cli(CPU *c, __attribute__ ((unused)) OP_CODE_INFO *o){
    setFlag(c, I, 0);
}

//Clear overflow flag
void clv(CPU *c, __attribute__ ((unused)) OP_CODE_INFO *o){
    setFlag(c, V, 0);
}

//Compare memory and accumulator
void cmp(CPU *c, OP_CODE_INFO *o){
    int8_t accum = getRegByte(c,ACCUM);
    int8_t operand = o->operand;
    int8_t diff = accum-operand;
    //longdiff is used to calculate whether a carry occurred
    uint16_t longDiff = (0x00FF&accum) - (0x00FF&operand);
    setFlag(c, C, longDiff < 0x100);
    setFlag(c, S, (diff & 0x80) ? 1 : 0);    // get 7th bit of src
    setFlag(c, Z, diff ? 0 : 1);
}

//Compare memory and index x
void cpx(CPU *c, OP_CODE_INFO *o){
    int8_t xVal = getRegByte(c,IND_X);
    int8_t operand = o->operand;
    int8_t diff = xVal-operand;
    //longdiff is used to calculate whether a carry occurred
    uint16_t longDiff = (0x00FF&xVal) - (0x00FF&operand);
    setFlag(c, C, longDiff < 0x100);
    setFlag(c, S, (diff & 0x80) ? 1 : 0);    // get 7th bit of src
    setFlag(c, Z, diff ? 0 : 1);
}

//Compare memory and index y
void cpy(CPU *c, OP_CODE_INFO *o){
    int8_t yVal = getRegByte(c,IND_Y);
    int8_t operand = o->operand;
    int8_t diff = yVal-operand;
    //longdiff is used to calculate whether a carry occurred
    uint16_t longDiff = (0x00FF&yVal) - (0x0FF&operand);
    setFlag(c, C, longDiff < 0x100);
    setFlag(c, S, (diff & 0x80) ? 1 : 0);    // get 7th bit of src
    setFlag(c, Z, diff ? 0 : 1);
}

//Decrement byte at specified address by 1
void dec(CPU *c, OP_CODE_INFO *o){
    uint8_t operand = o->operand;
    uint8_t res = operand - 1;
    setSign(c,res);
    setZero(c,res);
    c->addressSpace[o->address] = res;
}

//Decrement X index by 1
void dex(CPU *c, OP_CODE_INFO *o){
    uint8_t xVal = getRegByte(c, IND_X);
    uint8_t res = xVal - 1;
    setSign(c,res);
    setZero(c,res);
    setRegByte(c, IND_X, res);
}

//Decrement Y index by 1
void dey(CPU *c, OP_CODE_INFO *o){
    uint8_t yVal = getRegByte(c, IND_Y);
    uint8_t res = yVal - 1;
    setSign(c,res);
    setZero(c,res);
    setRegByte(c, IND_Y, res);
}

//Unimplemented future function that will error
//when called
void fut(CPU *c, OP_CODE_INFO *o){
    printf("Error! Called unimplemented future function!");
    assert(0);
}

//Increments specified address by 1
void inc(CPU *c, OP_CODE_INFO *o){
    uint8_t operand = o->operand;
    uint8_t res = operand + 1;
    setSign(c,res);
    setZero(c,res);
    c->addressSpace[o->address] = res;
}

//Increment X index by 1
void inx(CPU *c, OP_CODE_INFO *o){
    uint8_t xVal = getRegByte(c, IND_X);
    uint8_t res = xVal + 1;
    setSign(c,res);
    setZero(c,res);
    setRegByte(c, IND_X, res);
}

//Increment Y index by 1
void iny(CPU *c, OP_CODE_INFO *o){
    uint8_t yVal = getRegByte(c, IND_Y);
    uint8_t res = yVal + 1;
    setSign(c,res);
    setZero(c,res);
    setRegByte(c, IND_Y, res);
}

//Jump PC to 16 bit operand
void jmp(CPU *c, OP_CODE_INFO *o){
    c->PC = 0xFFFF&(o->address);
}

//Jump to subroutine
void jsr(CPU *c, OP_CODE_INFO *o){
    //decrement PC because PC will be
    //incremented by one when it jumps back
    c->PC--;
    //STACK holds eight bit values
    //so we push the 16 bit address
    //onto the stack in two parts
    uint8_t upperByte = ((c->PC)>>8) & 0xFF;
    uint8_t lowerByte = c->PC;
    PUSH(c, upperByte);
    PUSH(c, lowerByte);
    c->PC = o->address;
}

//Load value into accumulator
void lda(CPU *c, OP_CODE_INFO *o){
    setSign(c, o->operand);
    setZero(c, o->operand);
    setRegByte(c, ACCUM, o->operand);
}

//Load value into x reg
void ldx(CPU *c, OP_CODE_INFO *o){
    setSign(c, o->operand);
    setZero(c, o->operand);
    setRegByte(c, IND_X, o->operand);
}

//Load value into y reg
void ldy(CPU *c, OP_CODE_INFO *o){
    setSign(c, o->operand);
    setZero(c, o->operand);
    setRegByte(c, IND_Y, o->operand);
}

//Logical shift right with zero fill
void lsr(CPU *c, OP_CODE_INFO *o){
    //shift rightmost bit into carry
    setFlag(c, C, o->operand & 0x01);
    //cast to uint8_t to force zero fill
    int8_t shifted = ((uint8_t)o->operand) >> 1;
    setSign(c, shifted);
    setZero(c, shifted);
    if(o->mode == modeAccumulator){
        setRegByte(c, ACCUM, shifted);
    } else {
        write(c, o->address, shifted);
    }
}

// No operation
void nop(CPU *c, OP_CODE_INFO *o){}

// OR memory with accumulator
void ora(CPU *c, OP_CODE_INFO *o){
    int8_t src = o->operand | getRegByte(c, ACCUM);
    printf("FUNCTION NOT IMPLEMENTED CORRECTLY!");
    assert(0);
    //setFlag(c, Z, src);
    //setFlag(c, S, src);
    setRegByte(c, ACCUM, src);
}

// Push accumulator onto stack
void pha(CPU *c, OP_CODE_INFO *o){
    PUSH(c, getRegByte(c, ACCUM));
}

// Push status register onto stack
void php(CPU *c, OP_CODE_INFO *o){
    PUSH(c, getRegByte(c, STATUS));
}

// Pull accumulator from stack
void pla(CPU *c, OP_CODE_INFO *o){
    int8_t accumVal = PULL(c);
    setRegByte(c, ACCUM, accumVal);
    setSign(c,accumVal);
    setZero(c,accumVal);
}

// Pull status register from stack
void plp(CPU *c, OP_CODE_INFO *o){
    assert(0);
    // setRegByte(c, STATUS, PULL(c));
}

void rts(CPU *c, OP_CODE_INFO *o){
    uint8_t lowerByte = PULL(c);
    uint8_t upperByte = PULL(c);
    //add 1 to address before we jump PC to it
    //in order to resume program at correct place
    uint16_t address = ((upperByte << 8) | lowerByte) + 1;
    c->PC = address;
}

// Subtract operand from accumulator with borrow
void sbc(CPU *c, OP_CODE_INFO *o){
    //we want to subtract the opposite of the carry bit
    int8_t carry = getFlag(c,C) ? 0 : 1; 
    int8_t accum = getRegByte(c,ACCUM);
    int8_t operand = o->operand;
    uint16_t diff = (0x00FF&accum) - (0x00FF&operand) - (0x00FF&carry);
    setSign(c,diff&0xFF);
    setZero(c,diff&0xFF);
    setOverflowSubtract(c,accum,operand,diff&0xFF);
    if(getFlag(c,D)){ //in decimal mode
        if(((accum & 0xF) - carry) < (operand & 0xF)){
            diff -= 6;
        }
        if(diff > 0x99){
            diff -= 0x60;
        }
    }
    setFlag(c,C, diff < 0x100);
    setRegByte(c,ACCUM,diff&0xFF);
}

// Set carry flag to 1
void sec(CPU *c, OP_CODE_INFO *o){
    setFlag(c, C, 1);
}

// Store accumulator reg into memory
void sta(CPU *c, OP_CODE_INFO *o){
    write(c, o->address, getRegByte(c,ACCUM));
}

// Store X reg into memory
void stx(CPU *c, OP_CODE_INFO *o){
    write(c, o->address, getRegByte(c,IND_X));
}

// Store Y reg into memory
void sty(CPU *c, OP_CODE_INFO *o){
    write(c, o->address, getRegByte(c,IND_Y));
}

// Transfer accumulator to Y reg
void tay(CPU *c, OP_CODE_INFO *o){
    int8_t accumVal = getRegByte(c,ACCUM);
    setSign(c,accumVal);
    setZero(c,accumVal);
    setRegByte(c,IND_Y,accumVal);
}

// Transfer Y to accumulator
void tya(CPU *c, OP_CODE_INFO *o){
    int8_t yVal = getRegByte(c,IND_Y);
    setSign(c,yVal);
    setZero(c,yVal);
    setRegByte(c,ACCUM,yVal);
}

// Transfer x to accumulator
void txa(CPU *c, OP_CODE_INFO *o){
    int8_t xVal = getRegByte(c,IND_X);
    setSign(c,xVal);
    setZero(c,xVal);
    setRegByte(c,ACCUM,xVal);
}

// Transfer accumulator to x
void tax(CPU *c, OP_CODE_INFO *o){
    int8_t accumVal = getRegByte(c,ACCUM);
    setSign(c,accumVal);
    setZero(c,accumVal);
    setRegByte(c,IND_X,accumVal);
}

void eor(CPU *c, OP_CODE_INFO *o){
    printf("Called unimplemented function!");
    assert(0);
}

void rti(CPU *c, OP_CODE_INFO *o){
    printf("Called unimplemented function!");
    assert(0);
}

void rol(CPU *c, OP_CODE_INFO *o){
    printf("Called unimplemented function!");
    assert(0);
}

void ror(CPU *c, OP_CODE_INFO *o){
    printf("Called unimplemented function!");
    assert(0);
}

void sei(CPU *c, OP_CODE_INFO *o){
    printf("Called unimplemented function!");
    assert(0);
}

void txs(CPU *c, OP_CODE_INFO *o){
    printf("Called unimplemented function!");
    assert(0);
}

void tsx(CPU *c, OP_CODE_INFO *o){
    printf("Called unimplemented function!");
    assert(0);
}

void sed(CPU *c, OP_CODE_INFO *o){
    printf("Called unimplemented function!");
    assert(0);
}

//from https://github.com/fogleman/nes/blob/master/nes/cpu.go
uint8_t instructionSizes[256] = {
    1, 2, 0, 0, 2, 2, 2, 0, 
    1, 2, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 2, 2, 2, 0,
    1, 3, 1, 0, 3, 3, 3, 0,
    3, 2, 0, 0, 2, 2, 2, 0,
    1, 2, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 2, 2, 2, 0, 
    1, 3, 1, 0, 3, 3, 3, 0,
    1, 2, 0, 0, 2, 2, 2, 0, 
    1, 2, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 2, 2, 2, 0, 
    1, 3, 1, 0, 3, 3, 3, 0,
    1, 2, 0, 0, 2, 2, 2, 0, 
    1, 2, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 2, 2, 2, 0, 
    1, 3, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 2, 2, 2, 0, 
    1, 0, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 2, 2, 2, 0, 
    1, 3, 1, 0, 0, 3, 0, 0,
    2, 2, 2, 0, 2, 2, 2, 0, 
    1, 2, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 2, 2, 2, 0, 
    1, 3, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 2, 2, 2, 0, 
    1, 2, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 2, 2, 2, 0, 
    1, 3, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 2, 2, 2, 0, 
    1, 2, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 2, 2, 2, 0, 
    1, 3, 1, 0, 3, 3, 3, 0
};

//from https://github.com/fogleman/nes/blob/master/nes/cpu.go
uint8_t instructionModes[256] = {
    6, 7, 6, 7, 11, 11, 11, 11, 6, 5, 4, 5, 1, 1, 1, 1,
    10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
    1, 7, 6, 7, 11, 11, 11, 11, 6, 5, 4, 5, 1, 1, 1, 1,
    10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
    6, 7, 6, 7, 11, 11, 11, 11, 6, 5, 4, 5, 1, 1, 1, 1,
    10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
    6, 7, 6, 7, 11, 11, 11, 11, 6, 5, 4, 5, 8, 1, 1, 1,
    10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
    5, 7, 5, 7, 11, 11, 11, 11, 6, 5, 6, 5, 1, 1, 1, 1,
    10, 9, 6, 9, 12, 12, 13, 13, 6, 3, 6, 3, 2, 2, 3, 3,
    5, 7, 5, 7, 11, 11, 11, 11, 6, 5, 6, 5, 1, 1, 1, 1,
    10, 9, 6, 9, 12, 12, 13, 13, 6, 3, 6, 3, 2, 2, 3, 3,
    5, 7, 5, 7, 11, 11, 11, 11, 6, 5, 6, 5, 1, 1, 1, 1,
    10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
    5, 7, 5, 7, 11, 11, 11, 11, 6, 5, 6, 5, 1, 1, 1, 1,
    10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2
};

//from https://github.com/fogleman/nes/blob/master/nes/cpu.go
char instructionNames[256][4] = {
    //FUT represents unimplemented op codes
    "BRK", "ORA", "FUT", "FUT", "FUT", "ORA", "ASL", "FUT",
    "PHP", "ORA", "ASL", "FUT", "FUT", "ORA", "ASL", "FUT",
    "BPL", "ORA", "FUT", "FUT", "FUT", "ORA", "ASL", "FUT",
    "CLC", "ORA", "FUT", "FUT", "FUT", "ORA", "ASL", "FUT",
    "JSR", "AND", "FUT", "FUT", "BIT", "AND", "ROL", "FUT",
    "PLP", "AND", "ROL", "FUT", "BIT", "AND", "ROL", "FUT",
    "BMI", "AND", "FUT", "FUT", "FUT", "AND", "ROL", "FUT",
    "SEC", "AND", "FUT", "FUT", "FUT", "AND", "ROL", "FUT",
    "RTI", "EOR", "FUT", "FUT", "FUT", "EOR", "LSR", "FUT",
    "PHA", "EOR", "LSR", "FUT", "JMP", "EOR", "LSR", "FUT",
    "BVC", "EOR", "FUT", "FUT", "FUT", "EOR", "LSR", "FUT",
    "CLI", "EOR", "FUT", "FUT", "FUT", "EOR", "LSR", "FUT",
    "RTS", "ADC", "FUT", "FUT", "FUT", "ADC", "ROR", "FUT",
    "PLA", "ADC", "ROR", "FUT", "JMP", "ADC", "ROR", "FUT",
    "BVS", "ADC", "FUT", "FUT", "FUT", "ADC", "ROR", "FUT",
    "SEI", "ADC", "FUT", "FUT", "FUT", "ADC", "ROR", "FUT",
    "FUT", "STA", "FUT", "FUT", "STY", "STA", "STX", "FUT",
    "DEY", "FUT", "TXA", "FUT", "STY", "STA", "STX", "FUT",
    "BCC", "STA", "FUT", "FUT", "STY", "STA", "STX", "FUT",
    "TYA", "STA", "TXS", "FUT", "FUT", "STA", "FUT", "FUT",
    "LDY", "LDA", "LDX", "FUT", "LDY", "LDA", "LDX", "FUT",
    "TAY", "LDA", "TAX", "FUT", "LDY", "LDA", "LDX", "FUT",
    "BCS", "LDA", "FUT", "FUT", "LDY", "LDA", "LDX", "FUT",
    "CLV", "LDA", "TSX", "FUT", "LDY", "LDA", "LDX", "FUT",
    "CPY", "CMP", "FUT", "FUT", "CPY", "CMP", "DEC", "FUT",
    "INY", "CMP", "DEX", "FUT", "CPY", "CMP", "DEC", "FUT",
    "BNE", "CMP", "FUT", "FUT", "FUT", "CMP", "DEC", "FUT",
    "CLD", "CMP", "FUT", "FUT", "FUT", "CMP", "DEC", "FUT",
    "CPX", "SBC", "FUT", "FUT", "CPX", "SBC", "INC", "FUT",
    "INX", "SBC", "NOP", "FUT", "CPX", "SBC", "INC", "FUT",
    "BEQ", "SBC", "FUT", "FUT", "FUT", "SBC", "INC", "FUT",
    "SED", "SBC", "FUT", "FUT", "FUT", "SBC", "INC", "FUT"
};

//from https://github.com/fogleman/nes/blob/master/nes/cpu.go
void (*opcodeToFunction[256])(CPU *c, OP_CODE_INFO *o) = {
    brk, ora, fut, fut, fut, ora, asl, fut,
    php, ora, asl, fut, fut, ora, asl, fut,
    bpl, ora, fut, fut, fut, ora, asl, fut,
    clc, ora, fut, fut, fut, ora, asl, fut,
    jsr, and, fut, fut, bit, and, rol, fut,
    plp, and, rol, fut, bit, and, rol, fut,
    bmi, and, fut, fut, fut, and, rol, fut,
    sec, and, fut, fut, fut, and, rol, fut,
    rti, eor, fut, fut, fut, eor, lsr, fut,
    pha, eor, lsr, fut, jmp, eor, lsr, fut,
    bvc, eor, fut, fut, fut, eor, lsr, fut,
    cli, eor, fut, fut, fut, eor, lsr, fut,
    rts, adc, fut, fut, fut, adc, ror, fut,
    pla, adc, ror, fut, jmp, adc, ror, fut,
    bvs, adc, fut, fut, fut, adc, ror, fut,
    sei, adc, fut, fut, fut, adc, ror, fut,
    fut, sta, fut, fut, sty, sta, stx, fut,
    dey, fut, txa, fut, sty, sta, stx, fut,
    bcc, sta, fut, fut, sty, sta, stx, fut,
    tya, sta, txs, fut, fut, sta, fut, fut,
    ldy, lda, ldx, fut, ldy, lda, ldx, fut,
    tay, lda, tax, fut, ldy, lda, ldx, fut,
    bcs, lda, fut, fut, ldy, lda, ldx, fut,
    clv, lda, tsx, fut, ldy, lda, ldx, fut,
    cpy, cmp, fut, fut, cpy, cmp, dec, fut,
    iny, cmp, dex, fut, cpy, cmp, dec, fut,
    bne, cmp, fut, fut, fut, cmp, dec, fut,
    cld, cmp, fut, fut, fut, cmp, dec, fut,
    cpx, sbc, fut, fut, cpx, sbc, inc, fut,
    inx, sbc, nop, fut, cpx, sbc, inc, fut,
    beq, sbc, fut, fut, fut, sbc, inc, fut,
    sed, sbc, fut, fut, fut, sbc, inc, fut
};
