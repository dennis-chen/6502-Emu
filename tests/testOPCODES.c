#include "minunit.h"
#include "../src/cpu.h"

int tests_run = 0;

int8_t testADCHelper(CPU *c, int8_t accumByte, int8_t operand){
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,modeImmediate);
    setRegByte(c,ACCUM,accumByte);
    adc(c,o);
    freeOP_CODE_INFO(o);
    return getRegByte(c,ACCUM);
}

int8_t testSBCHelper(CPU *c, int8_t accumByte, int8_t operand){
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,modeImmediate);
    setRegByte(c,ACCUM,accumByte);
    sbc(c,o);
    freeOP_CODE_INFO(o);
    return getRegByte(c,ACCUM);
}

char * sprintfStatusCode(char *msg, char *actualStatus, char *correctStatus){
    //takes msg string with two %s' in it somewhere and returns a new string
    //with the two %s replaced by correctStatus and actualStatus, which must
    //be 8 chars in length
    int newLen = strlen(msg) + strlen(actualStatus) + strlen(correctStatus);
    char *res = malloc(sizeof(char)*newLen);
    sprintf(res,msg,actualStatus,correctStatus);
    return res;
}

static char * testRegStatus(CPU *c, char * correctStatus, char * errMsgBase){
    char * actualStatus = getStatus(c);
    char * errMsg = sprintfStatusCode(errMsgBase,actualStatus,correctStatus);
    mu_assert(errMsg, !strcmp(correctStatus,actualStatus));
    free(actualStatus);
    free(errMsg);
    return 0;
}

static char * ADC1() {
    //adding positives, no overflow, no carry
    CPU *c = getCPU();
    int8_t accumVal = testADCHelper(c,13,14);
    mu_assert("ADC1 err, ACCUM reg != 27", accumVal == 27);
    mu_run_test_with_args(testRegStatus,c,"00100000",
            "          NVUBDIZC    NVUBDIZC\nADC1 err, %s != %s");
    free(c);
    return 0;
}

static char * ADC2() {
    //adding positive to negative, no overflow, yes carry
    CPU *c = getCPU();
    int8_t accumVal = testADCHelper(c,-39,92);
    mu_assert("ADC2 err, ACCUM reg != 53", accumVal == 53);
    mu_run_test_with_args(testRegStatus,c,"00100001",
            "          NVUBDIZC    NVUBDIZC\nADC2 err, %s != %s");
    free(c);
    return 0;
}

static char * ADC3() {
    //adding positives, yes overflow, no carry
    CPU *c = getCPU();
    int8_t accumVal = testADCHelper(c,104,45);
    mu_assert("ADC3 err, ACCUM reg != -107", accumVal == -107);
    mu_run_test_with_args(testRegStatus,c,"11100000",
            "          NVUBDIZC    NVUBDIZC\nADC3 err, %s != %s");
    free(c);
    return 0;
}

static char * ADC4() {
    //adding negatives, yes overflow, yes carry
    CPU *c = getCPU();
    int8_t accumVal = testADCHelper(c,-103,-69);
    mu_assert("ADC4 err, ACCUM reg != 84", accumVal == 84);
    mu_run_test_with_args(testRegStatus,c,"01100001",
            "          NVUBDIZC    NVUBDIZC\nADC4 err, %s != %s");
    free(c);
    return 0;
}

static char * ADC5() {
    //adding with carry flag, no overflow, no carry
    CPU *c = getCPU();
    setFlag(c,C,1);
    int8_t accumVal = testADCHelper(c,30,12);
    mu_assert("ADC5 err, ACCUM reg != 43", accumVal == 43);
    mu_run_test_with_args(testRegStatus,c,"00100000",
            "          NVUBDIZC    NVUBDIZC\nADC5 err, %s != %s");
    free(c);
    return 0;
}

static char * ADC6() {
    //adding in DECIMAL mode
    CPU *c = getCPU();
    setFlag(c,D,1);
    //no overflow
    int8_t accumVal = testADCHelper(c,38,66);
    mu_assert("ADC6 err, ACCUM reg != 104", accumVal == 104);
    mu_run_test_with_args(testRegStatus,c,"00101000",
            "          NVUBDIZC    NVUBDIZC\nADC6 err, %s != %s");
    //overflow
    accumVal = testADCHelper(c,52,104);
    mu_assert("ADC6 err, ACCUM reg != 2", accumVal == 2);
    //NEG flag is one here because the zero and neg flags
    //are set before correction for decimal addition overflow
    //is done
    mu_run_test_with_args(testRegStatus,c,"11101001",
            "          NVUBDIZC    NVUBDIZC\nADC6 err, %s != %s");
    free(c);
    return 0;
}

static char * AND1() {
    //test zero reg setting
    CPU *c = getCPU();
    int8_t operand = 0x00;
    int8_t accum = 0xFF;
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,modeImmediate);
    setRegByte(c,ACCUM,accum);
    and(c,o);
    int8_t accumVal = getRegByte(c,ACCUM);
    mu_assert("AND1 err, ACCUM reg != 0", accumVal == 0);
    mu_run_test_with_args(testRegStatus,c,"00100010",
            "          NVUBDIZC    NVUBDIZC\nAND1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * AND2() {
    //test sign flag setting
    CPU *c = getCPU();
    int8_t operand = 0x85;
    int8_t accum = 0xF1;
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,modeImmediate);
    setRegByte(c,ACCUM,accum);
    and(c,o);
    int8_t accumVal = getRegByte(c,ACCUM);
    mu_assert("AND2 err, ACCUM reg != -127", accumVal == -127);
    mu_run_test_with_args(testRegStatus,c,"10100000",
            "          NVUBDIZC    NVUBDIZC\nAND2 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * ASL1() {
    //Accumulator addressing mode
    CPU *c = getCPU();
    int8_t operand = 0xFF;
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,modeAccumulator);
    asl(c,o);
    int8_t accumVal = getRegByte(c,ACCUM);
    mu_assert("ASL1 err, ACCUM reg != -2", accumVal == -2);
    mu_run_test_with_args(testRegStatus,c,"10100001",
            "          NVUBDIZC    NVUBDIZC\nASL1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * ASL2() {
    //Non modeAccumulator addressing mode
    CPU *c = getCPU();
    int8_t operand = 0xFF;
    uint16_t address = 0xEFEF;
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,address,modeImmediate);
    asl(c,o);
    int8_t accumVal = getRegByte(c,ACCUM);
    int8_t addrVal = read(c,address);
    mu_assert("ASL2 err, ACCUM reg != 0", accumVal == 0);
    mu_assert("ASL2 err, Address val != -2", addrVal == -2);
    mu_run_test_with_args(testRegStatus,c,"10100001",
            "          NVUBDIZC    NVUBDIZC\nASL2 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * BNE1() {
    CPU *c = getCPU();
    uint16_t address = 0x8FFE;
    OP_CODE_INFO *o = getOP_CODE_INFO(0,address,modeImmediate);
    setFlag(c,Z,0);
    bne(c,o);
    mu_assert("BNE1 err, PC != 0x8FFE", c->PC == 0x8FFE);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * BNE2() {
    CPU *c = getCPU();
    uint16_t address = 0xFFFF;
    OP_CODE_INFO *o = getOP_CODE_INFO(0,address,modeImmediate);
    setFlag(c,Z,1);
    bne(c,o);
    mu_assert("BNE2 err, PC != 0", c->PC == 0);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * BPL1() {
    CPU *c = getCPU();
    uint16_t address = 0x8FFE;
    OP_CODE_INFO *o = getOP_CODE_INFO(0,address,modeImmediate);
    setFlag(c,S,0);
    bpl(c,o);
    mu_assert("BPL1 err, PC != 0x8FFE", c->PC == 0x8FFE);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * BPL2() {
    CPU *c = getCPU();
    uint16_t address = 0xFFFF;
    OP_CODE_INFO *o = getOP_CODE_INFO(0,address,modeImmediate);
    setFlag(c,S,1);
    bpl(c,o);
    mu_assert("BPL2 err, PC != 0", c->PC == 0);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * BCC1() {
    CPU *c = getCPU();
    uint16_t address = 0x8FFE;
    OP_CODE_INFO *o = getOP_CODE_INFO(0,address,modeImmediate);
    setFlag(c,C,0);
    bcc(c,o);
    mu_assert("BCC1 err, PC != 0x8FFE", c->PC == 0x8FFE);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * BCC2() {
    CPU *c = getCPU();
    uint16_t address = 0xFFFF;
    OP_CODE_INFO *o = getOP_CODE_INFO(0,address,modeImmediate);
    setFlag(c,C,1);
    bcc(c,o);
    mu_assert("BCC2 err, PC != 0", c->PC == 0);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * BCS1() {
    CPU *c = getCPU();
    uint16_t address = 0x8FFE;
    OP_CODE_INFO *o = getOP_CODE_INFO(0,address,modeImmediate);
    setFlag(c,C,1);
    bcs(c,o);
    mu_assert("BCS1 err, PC != 0x8FFE", c->PC == 0x8FFE);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * BCS2() {
    CPU *c = getCPU();
    uint16_t address = 0x8FFE;
    OP_CODE_INFO *o = getOP_CODE_INFO(0,address,modeImmediate);
    setFlag(c,C,0);
    bcs(c,o);
    mu_assert("BCS2 err, PC != 0", c->PC == 0);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * BEQ1() {
    CPU *c = getCPU();
    uint16_t address = 0x8FFE;
    OP_CODE_INFO *o = getOP_CODE_INFO(0,address,modeImmediate);
    setFlag(c,Z,1);
    beq(c,o);
    mu_assert("BEQ1 err, PC != 0x8FFE", c->PC == 0x8FFE);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * BEQ2() {
    CPU *c = getCPU();
    uint16_t address = 0x1111;
    OP_CODE_INFO *o = getOP_CODE_INFO(0,address,modeImmediate);
    setFlag(c,Z,0);
    beq(c,o);
    mu_assert("BEQ2 err, PC != 0", c->PC == 0);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * BIT1() {
    CPU *c = getCPU();
    int8_t operand = 0xC0;
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,modeImmediate);
    setRegByte(c,ACCUM,0);
    bit(c,o);
    mu_run_test_with_args(testRegStatus,c,"11100010",
            "          NVUBDIZC    NVUBDIZC\nBIT1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * BIT2() {
    CPU *c = getCPU();
    int8_t operand = 0x01;
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,modeImmediate);
    setRegByte(c,ACCUM,1);
    bit(c,o);
    mu_run_test_with_args(testRegStatus,c,"00100000",
            "          NVUBDIZC    NVUBDIZC\nBIT2 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * CLC1() {
    CPU *c = getCPU();
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,modeImmediate);
    setFlag(c,C,1);
    mu_run_test_with_args(testRegStatus,c,"00100001",
            "          NVUBDIZC    NVUBDIZC\nCLC1 err, %s != %s");
    clc(c,o);
    mu_run_test_with_args(testRegStatus,c,"00100000",
            "          NVUBDIZC    NVUBDIZC\nCLC1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * DEC1() {
    CPU *c = getCPU();
    uint16_t address = 0xEFF;
    int8_t operand = -34;
    write(c,address,operand);
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,address,modeImmediate);
    dec(c,o);
    int8_t accumVal = read(c,address);
    mu_assert("DEC1 err, 0xEFF address != -35", accumVal == -35);
    mu_run_test_with_args(testRegStatus,c,"10100000",
            "          NVUBDIZC    NVUBDIZC\nCLC1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * DEC2() {
    CPU *c = getCPU();
    uint16_t address = 0xEFF;
    int8_t operand = 1;
    write(c,address,operand);
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,address,modeImmediate);
    dec(c,o);
    int8_t accumVal = read(c,address);
    mu_assert("DEC2 err, ACCUM reg != 0", accumVal == 0);
    mu_run_test_with_args(testRegStatus,c,"00100010",
            "          NVUBDIZC    NVUBDIZC\nCLC1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * DEC3() {
    //decrement X reg by 1 with zero
    //in X reg
    CPU *c = getCPU();
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,modeImmediate);
    setRegByte(c,ACCUM,0);
    dec(c,o);
    int8_t accumVal = getRegByte(c,ACCUM);
    mu_assert("DEC3 err, ACCUM reg != -1", accumVal == -1);
    mu_run_test_with_args(testRegStatus,c,"10100000",
            "          NVUBDIZC    NVUBDIZC\nCLC1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * DEX1() {
    //decrement X reg by 1 with neg twos complement val
    //in X reg
    CPU *c = getCPU();
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,modeImmediate);
    setRegByte(c,IND_X,-34);
    dex(c,o);
    int8_t xVal = getRegByte(c,IND_X);
    mu_assert("DEX1 err, IND_X reg != -35", xVal == -35);
    mu_run_test_with_args(testRegStatus,c,"10100000",
            "          NVUBDIZC    NVUBDIZC\nCLC1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * DEX2() {
    //decrement X reg by 1 with positive twos complement val
    //in X reg
    CPU *c = getCPU();
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,modeImmediate);
    setRegByte(c,IND_X,1);
    dex(c,o);
    int8_t xVal = getRegByte(c,IND_X);
    mu_assert("DEX2 err, IND_X reg != 0", xVal == 0);
    mu_run_test_with_args(testRegStatus,c,"00100010",
            "          NVUBDIZC    NVUBDIZC\nCLC1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * DEX3() {
    //decrement X reg by 1 with zero
    //in X reg
    CPU *c = getCPU();
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,modeImmediate);
    setRegByte(c,IND_X,0);
    dex(c,o);
    int8_t xVal = getRegByte(c,IND_X);
    mu_assert("DEX3 err, IND_X reg != -1", xVal == -1);
    mu_run_test_with_args(testRegStatus,c,"10100000",
            "          NVUBDIZC    NVUBDIZC\nCLC1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * DEY1() {
    //decrement Y reg by 1 with neg twos complement val
    //in Y reg
    CPU *c = getCPU();
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,modeImmediate);
    setRegByte(c,IND_Y,-34);
    dey(c,o);
    int8_t yVal = getRegByte(c,IND_Y);
    mu_assert("DEY1 err, IND_Y reg != -35", yVal == -35);
    mu_run_test_with_args(testRegStatus,c,"10100000",
            "          NVUBDIZC    NVUBDIZC\nCLC1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * DEY2() {
    //decrement Y reg by 1 with positive twos complement val
    //in Y reg
    CPU *c = getCPU();
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,modeImmediate);
    setRegByte(c,IND_Y,1);
    dey(c,o);
    int8_t yVal = getRegByte(c,IND_Y);
    mu_assert("DEY2 err, IND_Y reg != 0", yVal == 0);
    mu_run_test_with_args(testRegStatus,c,"00100010",
            "          NVUBDIZC    NVUBDIZC\nCLC1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * DEY3() {
    //decrement Y reg by 1 with zero
    //in Y reg
    CPU *c = getCPU();
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,modeImmediate);
    setRegByte(c,IND_Y,0);
    dey(c,o);
    int8_t yVal = getRegByte(c,IND_Y);
    mu_assert("DEY3 err, IND_Y reg != -1", yVal == -1);
    mu_run_test_with_args(testRegStatus,c,"10100000",
            "          NVUBDIZC    NVUBDIZC\nCLC1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * LDA1() {
    //load zero val into accumulator
    CPU *c = getCPU();
    int8_t operand = 0x00;
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,modeImmediate);
    setRegByte(c,ACCUM,10);
    lda(c,o);
    int8_t accumVal = getRegByte(c,ACCUM);
    mu_assert("LDA1 err, ACCUM reg != 0", accumVal == 0);
    mu_run_test_with_args(testRegStatus,c,"00100010",
            "          NVUBDIZC    NVUBDIZC\nCLC1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * LDA2() {
    //load neg val into accumulator
    CPU *c = getCPU();
    int8_t operand = -99;
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,modeImmediate);
    lda(c,o);
    int8_t accumVal = getRegByte(c,ACCUM);
    mu_assert("LDA2 err, ACCUM reg != -99", accumVal == -99);
    mu_run_test_with_args(testRegStatus,c,"10100000",
            "          NVUBDIZC    NVUBDIZC\nCLC1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * LDX1() {
    //load zero val into x reg
    CPU *c = getCPU();
    int8_t operand = 0x00;
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,modeImmediate);
    setRegByte(c,ACCUM,10);
    ldx(c,o);
    int8_t xVal = getRegByte(c,IND_X);
    mu_assert("LDX1 err, ACCUM reg != 0", xVal == 0);
    mu_run_test_with_args(testRegStatus,c,"00100010",
            "          NVUBDIZC    NVUBDIZC\nCLC1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * LDX2() {
    //load neg val into x reg
    CPU *c = getCPU();
    int8_t operand = -99;
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,modeImmediate);
    ldx(c,o);
    int8_t xVal = getRegByte(c,IND_X);
    mu_assert("LDX2 err, ACCUM reg != -99", xVal == -99);
    mu_run_test_with_args(testRegStatus,c,"10100000",
            "          NVUBDIZC    NVUBDIZC\nCLC1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * LDY1() {
    //load zero val into x reg
    CPU *c = getCPU();
    int8_t operand = 0x00;
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,modeImmediate);
    setRegByte(c,ACCUM,10);
    ldy(c,o);
    int8_t yVal = getRegByte(c,IND_Y);
    mu_assert("LDY1 err, ACCUM reg != 0", yVal == 0);
    mu_run_test_with_args(testRegStatus,c,"00100010",
            "          NVUBDIZC    NVUBDIZC\nCLC1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * LDY2() {
    //load neg val into x reg
    CPU *c = getCPU();
    int8_t operand = -99;
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,modeImmediate);
    ldy(c,o);
    int8_t yVal = getRegByte(c,IND_Y);
    mu_assert("LDY2 err, ACCUM reg != -99", yVal == -99);
    mu_run_test_with_args(testRegStatus,c,"10100000",
            "          NVUBDIZC    NVUBDIZC\nCLC1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * SBC1() {
    //pos ACUM, neg operand, no overflow, no carry
    CPU *c = getCPU();
    setFlag(c,C,1);
    int8_t accumVal = testSBCHelper(c,13,-14);
    mu_assert("SBC1 err, ACCUM reg != 27", accumVal == 27);
    mu_run_test_with_args(testRegStatus,c,"00100000",
            "          NVUBDIZC    NVUBDIZC\nSBC1 err, %s != %s");
    free(c);
    return 0;
}

static char * SBC2() {
    //pos ACUM, neg operand, no overflow, yes carry
    CPU *c = getCPU();
    setFlag(c,C,1);
    int8_t accumVal = testSBCHelper(c,-39,-92);
    mu_assert("SBC2 err, ACCUM reg != 53", accumVal == 53);
    mu_run_test_with_args(testRegStatus,c,"00100001",
            "          NVUBDIZC    NVUBDIZC\nSBC2 err, %s != %s");
    free(c);
    return 0;
}

static char * SBC3() {
    //pos ACUM, neg operand, yes overflow, no carry
    CPU *c = getCPU();
    setFlag(c,C,1);
    int8_t accumVal = testSBCHelper(c,104,-45);
    mu_assert("SBC3 err, ACCUM reg != -105", accumVal == -107);
    mu_run_test_with_args(testRegStatus,c,"11100000",
            "          NVUBDIZC    NVUBDIZC\nSBC3 err, %s != %s");
    free(c);
    return 0;
}

static char * SBC4() {
    //neg ACUM, pos operand, yes overflow, yes carry
    CPU *c = getCPU();
    setFlag(c,C,1);
    int8_t accumVal = testSBCHelper(c,-103,69);
    mu_assert("SBC4 err, ACCUM reg != 84", accumVal == 84);
    mu_run_test_with_args(testRegStatus,c,"01100001",
            "          NVUBDIZC    NVUBDIZC\nSBC4 err, %s != %s");
    free(c);
    return 0;
}

static char * SBC5() {
    //pos ACUM, neg operand, no overflow, carry flag zeroed
    CPU *c = getCPU();
    setFlag(c,C,0);
    int8_t accumVal = testSBCHelper(c,30,-12);
    mu_assert("SBC5 err, ACCUM reg != 41", accumVal == 41);
    mu_run_test_with_args(testRegStatus,c,"00100000",
            "          NVUBDIZC    NVUBDIZC\nSBC5 err, %s != %s");
    free(c);
    return 0;
}

static char * SBC6() {
    //subtraction in decimal mode
    CPU *c = getCPU();
    setFlag(c,D,1);
    setFlag(c,C,1);
    //no overflow, 66 - 38 in BCD mode
    int8_t accumVal = testSBCHelper(c,0x66,0x38);
    mu_assert("SBC6 err, ACCUM reg != 0x28", accumVal == 0x28);
    mu_run_test_with_args(testRegStatus,c,"00101001",
            "          NVUBDIZC    NVUBDIZC\nSBC6 err, %s != %s");
    free(c);
    return 0;
}

static char * SBC7() {
    //subtraction in decimal mode
    CPU *c = getCPU();
    setFlag(c,D,1);
    setFlag(c,C,1);
    //no overflow, 93 - 87 in BCD mode
    int8_t accumVal = testSBCHelper(c,0x93,0x87);
    mu_assert("SBC7 err, ACCUM reg != 0x06", accumVal == 0x06);
    mu_run_test_with_args(testRegStatus,c,"00101001",
            "          NVUBDIZC    NVUBDIZC\nSBC7 err, %s != %s");
    free(c);
    return 0;
}

static char * SEC1() {
    CPU *c = getCPU();
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,modeImmediate);
    sec(c,o);
    mu_assert("SEC1 err, Carry flag != 1", getFlag(c,C) == 1);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * TAY1() {
    CPU *c = getCPU();
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,modeImmediate);
    setRegByte(c,ACCUM,-1);
    tay(c,o);
    int8_t yVal = getRegByte(c,IND_Y);
    mu_assert("TAY1 err, Y reg != -1", yVal == -1);
    mu_run_test_with_args(testRegStatus,c,"10100000",
            "          NVUBDIZC    NVUBDIZC\nTAY1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * TYA1() {
    CPU *c = getCPU();
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,modeImmediate);
    setRegByte(c,IND_Y,0);
    setRegByte(c,ACCUM,-39);
    tya(c,o);
    int8_t accumVal = getRegByte(c,ACCUM);
    mu_assert("TYA1 err, ACCUM reg != 0", accumVal == 0);
    mu_run_test_with_args(testRegStatus,c,"00100010",
            "          NVUBDIZC    NVUBDIZC\nTYA1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * TXA1() {
    CPU *c = getCPU();
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,modeImmediate);
    setRegByte(c,IND_X,-50);
    setRegByte(c,ACCUM,39);
    txa(c,o);
    int8_t accumVal = getRegByte(c,ACCUM);
    mu_assert("TXA1 err, ACCUM reg != -50", accumVal == -50);
    mu_run_test_with_args(testRegStatus,c,"10100000",
            "          NVUBDIZC    NVUBDIZC\nTXA1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * STA1() {
    CPU *c = getCPU();
    uint16_t address = 0xFFEE;
    OP_CODE_INFO *o = getOP_CODE_INFO(0,address,modeImmediate);
    setRegByte(c,ACCUM,-39);
    sta(c,o);
    int8_t addrVal = read(c,address);
    mu_assert("STA1 err, address at 0xFFEE != -39", addrVal == -39);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * STX1() {
    CPU *c = getCPU();
    uint16_t address = 0xFFEE;
    OP_CODE_INFO *o = getOP_CODE_INFO(0,address,modeImmediate);
    setRegByte(c,IND_X,-39);
    stx(c,o);
    int8_t addrVal = read(c,address);
    mu_assert("STX1 err, address at 0xFFEE != -39", addrVal == -39);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * CMP1() {
    CPU *c = getCPU();
    int8_t operand = 1;
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,modeImmediate);
    setRegByte(c,ACCUM,1);
    cmp(c,o);
    mu_run_test_with_args(testRegStatus,c,"00100011",
            "          NVUBDIZC    NVUBDIZC\nCMP1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * CMP2() {
    CPU *c = getCPU();
    int8_t operand = 50;
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,modeImmediate);
    setRegByte(c,ACCUM,1);
    cmp(c,o);
    mu_run_test_with_args(testRegStatus,c,"10100000",
            "          NVUBDIZC    NVUBDIZC\nCMP2 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * CPX1() {
    CPU *c = getCPU();
    int8_t operand = 5;
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,modeImmediate);
    setRegByte(c,IND_X,8);
    cpx(c,o);
    mu_run_test_with_args(testRegStatus,c,"00100001",
            "          NVUBDIZC    NVUBDIZC\nCPX1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * CPX2() {
    CPU *c = getCPU();
    int8_t operand = 75;
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,modeImmediate);
    setRegByte(c,IND_X,59);
    cpx(c,o);
    mu_run_test_with_args(testRegStatus,c,"10100000",
            "          NVUBDIZC    NVUBDIZC\nCPX2 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * INC1() {
    CPU *c = getCPU();
    uint16_t address = 0xEFF;
    int8_t operand = -1;
    write(c,address,operand);
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,address,modeImmediate);
    inc(c,o);
    int8_t accumVal = read(c,address);
    mu_assert("INC1 err, ACCUM reg != 0", accumVal == 0);
    mu_run_test_with_args(testRegStatus,c,"00100010",
            "          NVUBDIZC    NVUBDIZC\nINC1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * INC2() {
    CPU *c = getCPU();
    uint16_t address = 0xEFF;
    int8_t operand = -103;
    write(c,address,operand);
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,address,modeImmediate);
    inc(c,o);
    int8_t accumVal = read(c,address);
    mu_assert("INC2 err, ACCUM reg != -102", accumVal == -102);
    mu_run_test_with_args(testRegStatus,c,"10100000",
            "          NVUBDIZC    NVUBDIZC\nINC2 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * INX1() {
    CPU *c = getCPU();
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,modeImmediate);
    setRegByte(c,IND_X,-1);
    inx(c,o);
    int8_t xVal = getRegByte(c,IND_X);
    mu_assert("INX1 err, IND_X reg != 0", xVal == 0);
    mu_run_test_with_args(testRegStatus,c,"00100010",
            "          NVUBDIZC    NVUBDIZC\nINX1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * INX2() {
    CPU *c = getCPU();
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,modeImmediate);
    setRegByte(c,IND_X,-103);
    inx(c,o);
    int8_t xVal = getRegByte(c,IND_X);
    mu_assert("INX2 err, IND_X reg != -102", xVal == -102);
    mu_run_test_with_args(testRegStatus,c,"10100000",
            "          NVUBDIZC    NVUBDIZC\nINX2 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * JMP1() {
    CPU *c = getCPU();
    uint16_t address = 0xCFEE;
    OP_CODE_INFO *o = getOP_CODE_INFO(0,address,modeImmediate);
    jmp(c,o);
    mu_assert("JMP1 err, PC != 0xCFEE", c->PC == 0xCFEE);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * JSR1() {
    CPU *c = getCPU();
    c->PC = 0xCFEE;
    uint16_t address = 0xABAB;
    OP_CODE_INFO *o = getOP_CODE_INFO(0,address,modeImmediate);
    jsr(c,o);
    uint8_t stackPointer = getRegByte(c,STACK);
    mu_assert("JSR1 err, Stack Pointer != 0xFD", stackPointer == 0xFD);
    mu_assert("JSR1 err, PC != 0xABAB", c->PC == 0xABAB);
    uint8_t returnAddressLower = read(c,0x01FE);
    uint8_t returnAddressUpper = read(c,0x01FF);
    uint16_t returnAddress = (returnAddressUpper << 8) | returnAddressLower;
    mu_assert("JSR1 err, Return Address != 0xCFED", returnAddress == 0xCFED);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * LSR1() {
    //test LSR and save to memory address
    CPU *c = getCPU();
    uint16_t address = 0xCFEE;
    int8_t operand = 0xFF;
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,address,modeImmediate);
    lsr(c,o);
    int8_t memVal = read(c,address);
    int8_t accumVal = getRegByte(c,ACCUM);
    mu_assert("LSR1 err, memVal != 0x7F", memVal == 0x7F);
    mu_assert("LSR1 err, accumVal != 0", accumVal == 0);
    mu_run_test_with_args(testRegStatus,c,"00100001",
            "          NVUBDIZC    NVUBDIZC\nLSR1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * LSR2() {
    //test LSR and save to memory address
    CPU *c = getCPU();
    uint16_t address = 0xCFEE;
    int8_t operand = 0xFF;
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,address,modeAccumulator);
    lsr(c,o);
    int8_t memVal = read(c,address);
    int8_t accumVal = getRegByte(c,ACCUM);
    mu_assert("LSR2 err, memVal != 0", memVal == 0);
    mu_assert("LSR2 err, accumVal != 0x7F", accumVal == 0x7F);
    mu_run_test_with_args(testRegStatus,c,"00100001",
            "          NVUBDIZC    NVUBDIZC\nLSR2 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * NOP1() {
    CPU *c = getCPU();
    uint16_t address = 0xFFFF;
    int8_t operand = 0xFF;
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,address,modeImmediate);
    int8_t statusBef = getRegByte(c,STATUS);
    int8_t stackBef = getRegByte(c,STACK);
    int8_t accumBef = getRegByte(c,ACCUM);
    int8_t indXBef = getRegByte(c,IND_X);
    int8_t indYBef = getRegByte(c,IND_Y);
    uint16_t PCBef = c->PC;
    nop(c,o);
    int8_t statusAft = getRegByte(c,STATUS);
    int8_t stackAft = getRegByte(c,STACK);
    int8_t accumAft = getRegByte(c,ACCUM);
    int8_t indXAft = getRegByte(c,IND_X);
    int8_t indYAft = getRegByte(c,IND_Y);
    uint16_t PCAft = c->PC;
    mu_assert("NOP1 err, STATUS reg changed", statusBef == statusAft);
    mu_assert("NOP1 err, STACK reg changed", stackBef == stackAft);
    mu_assert("NOP1 err, ACCUM reg changed", accumBef == accumAft);
    mu_assert("NOP1 err, IND_X reg changed", indXBef == indXAft);
    mu_assert("NOP1 err, IND_Y reg changed", indYBef == indYAft);
    mu_assert("NOP1 err, PC changed", PCBef == PCAft);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * RTS1() {
    CPU *c = getCPU();
    c->PC = 0xCFEE;
    uint16_t address = 0xABAB;
    OP_CODE_INFO *o1 = getOP_CODE_INFO(0,address,modeImmediate);
    jsr(c,o1);
    OP_CODE_INFO *o2 = getOP_CODE_INFO(0,0,modeImmediate);
    rts(c,o2);
    mu_assert("RTS1 err, PC != 0xCFEE", c->PC == 0xCFEE);
    freeOP_CODE_INFO(o1);
    freeOP_CODE_INFO(o2);
    free(c);
    return 0;
}

static char * EOR1() {
    CPU *c = getCPU();
    c->PC = 0xCFEE;
    setRegByte(c,ACCUM,0x3F);
    OP_CODE_INFO *o = getOP_CODE_INFO(0x3F,0,modeImmediate);
    eor(c,o);
    mu_assert("EOR1 err, ACCUM != 0x00", (0xFF & getRegByte(c,ACCUM)) == 0x00);
    mu_assert("EOR1 err, SIGN != 0", getFlag(c, S) == 0);
    mu_assert("EOR1 err, ZERO != 1", getFlag(c, Z) == 1);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * EOR2() {
    CPU *c = getCPU();
    c->PC = 0xCFEE;
    setRegByte(c,ACCUM,0x3F);
    OP_CODE_INFO *o = getOP_CODE_INFO(0xFF,0,modeImmediate);
    eor(c,o);
    mu_assert("EOR2 err, ACCUM != 0xC0", (0xFF & getRegByte(c,ACCUM)) == 0xc0);
    mu_assert("EOR2 err, SIGN != 1", getFlag(c, S) == 1);
    mu_assert("EOR2 err, ZERO != 0", getFlag(c, Z) == 0);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * EOR3() {
    CPU *c = getCPU();
    c->PC = 0xCFEE;
    setRegByte(c,ACCUM,0x8A);
    OP_CODE_INFO *o = getOP_CODE_INFO(0x92,0,modeImmediate);
    eor(c,o);
    mu_assert("EOR2 err, ACCUM != 0x18", (0xFF & getRegByte(c,ACCUM)) == 0x18);
    mu_assert("EOR2 err, SIGN != 0", getFlag(c, S) == 0);
    mu_assert("EOR2 err, ZERO != 0", getFlag(c, Z) == 0);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * RTI1() {
    CPU *c = getCPU();
    c->PC = 0x6EFC;
    setRegByte(c, STACK, 0x2);
    c->addressSpace[0x103] = 0x4;
    c->addressSpace[0x104] = 0x00;
    c->addressSpace[0x105] = 0x06;
    OP_CODE_INFO *o = getOP_CODE_INFO(0, 0, modeImplied);
    rti(c,o);
    mu_assert("RTI1 err, PC != 0x600", c->PC == 0x600);
    mu_assert("RTI1 err, STATUS != 0x4", getRegByte(c,STATUS) == 0x4);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * RTI2() {
    CPU *c = getCPU();
    c->PC = 0xCE66;
    setRegByte(c, STACK, 0x2);
    c->addressSpace[0x103] = 0x3;
    c->addressSpace[0x104] = 0x66;
    c->addressSpace[0x105] = 0x60;
    OP_CODE_INFO *o = getOP_CODE_INFO(0, 0, modeImplied);
    rti(c,o);
    mu_assert("RTI2 err, PC != 0x6066", c->PC == 0x6066);
    mu_assert("RTI2 err, STATUS != 0x3", getRegByte(c,STATUS) == 0x3);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * RTI3() {
    CPU *c = getCPU();
    c->PC = 0xCE66;
    setRegByte(c, STACK, 0xF0);
    c->addressSpace[0x1F1] = 0xf;
    c->addressSpace[0x1F2] = 0xff;
    c->addressSpace[0x1F3] = 0xff;
    OP_CODE_INFO *o = getOP_CODE_INFO(0, 0, modeImplied);
    rti(c,o);
    mu_assert("RTI3 err, PC != 0x6066", c->PC == 0xFFFF);
    mu_assert("RTI3 err, STATUS != 0x3", getRegByte(c,STATUS) == 0xF);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * ROL1() {
    CPU *c = getCPU();
    setRegByte(c, ACCUM, 0xA1);
    OP_CODE_INFO *o = getOP_CODE_INFO(0xA1, 0, modeAccumulator);
    rol(c,o);
    mu_assert("ROL1 err, ACCUM != 0x42", (0xFF & getRegByte(c,ACCUM)) == 0x42);
    mu_assert("ROL1 err, CARRY != 1", getFlag(c,C) == 1);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * ROL2() {
    CPU *c = getCPU();
    setRegByte(c, ACCUM, 0xA1);
    OP_CODE_INFO *o1 = getOP_CODE_INFO(getRegByte(c,ACCUM) & 0xFF, 0, modeAccumulator);
    rol(c,o1);
    OP_CODE_INFO *o2 = getOP_CODE_INFO(getRegByte(c,ACCUM) & 0xFF, 0, modeAccumulator);
    rol(c,o2);
    mu_assert("ROL2 err, ACCUM != 0x85", (0xFF & getRegByte(c,ACCUM)) == 0x85);
    mu_assert("ROL2 err, CARRY != 0", getFlag(c,C) == 0);
    freeOP_CODE_INFO(o1);
    freeOP_CODE_INFO(o2);
    free(c);
    return 0;
}

static char * ROR1() {
    CPU *c = getCPU();
    setRegByte(c, ACCUM, 0xA1);
    OP_CODE_INFO *o = getOP_CODE_INFO(getRegByte(c,ACCUM) & 0xFF, 0, modeAccumulator);
    ror(c,o);
    mu_assert("ROR1 err, ACCUM != 0x50", (0xFF & getRegByte(c,ACCUM)) == 0x50);
    mu_assert("ROR1 err, CARRY != 1", getFlag(c,C) == 1);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * ROR2() {
    CPU *c = getCPU();
    setRegByte(c, ACCUM, 0x50);
    setFlag(c,C,1);
    OP_CODE_INFO *o = getOP_CODE_INFO(getRegByte(c,ACCUM) & 0xFF, 0, modeAccumulator);
    ror(c,o);
    mu_assert("ROR2 err, ACCUM != 0xA8", (0xFF & getRegByte(c,ACCUM)) == 0xA8);
    mu_assert("ROR2 err, CARRY != 0", getFlag(c,C) == 0);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * SEI1() {
    CPU *c = getCPU();
    OP_CODE_INFO *o = getOP_CODE_INFO(0, 0, modeImplied);
    sei(c,o);
    mu_assert("SEI1 err, INTERRUPT != 1", getFlag(c,I) == 1);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * TXS1() {
    CPU *c = getCPU();
    setRegByte(c,IND_X,0x99);
    OP_CODE_INFO *o = getOP_CODE_INFO(0, 0, modeImplied);
    txs(c,o);
    mu_assert("TXS1 err, STACK != 0x99", (getRegByte(c,STACK)&0xFF) == 0x99);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * TSX1() {
    CPU *c = getCPU();
    setRegByte(c,STACK,0x99);
    OP_CODE_INFO *o = getOP_CODE_INFO(0, 0, modeImplied);
    tsx(c,o);
    mu_assert("TXS1 err, STACK != 0x99", (getRegByte(c,IND_X)&0xFF) == 0x99);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * SED1() {
    CPU *c = getCPU();
    OP_CODE_INFO *o = getOP_CODE_INFO(0, 0, modeImplied);
    sed(c,o);
    mu_assert("SED1 err, DEC != 1", getFlag(c,D) == 1);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * all_tests() {
    mu_run_test(ADC1);
    mu_run_test(ADC2);
    mu_run_test(ADC3);
    mu_run_test(ADC4);
    mu_run_test(ADC5);
    mu_run_test(ADC6);
    mu_run_test(AND1);
    mu_run_test(AND2);
    mu_run_test(ASL1);
    mu_run_test(ASL2);
    mu_run_test(BCC1);
    mu_run_test(BCC2);
    mu_run_test(BCS1);
    mu_run_test(BCS2);
    mu_run_test(BEQ1);
    mu_run_test(BEQ2);
    mu_run_test(BNE1);
    mu_run_test(BNE2);
    mu_run_test(BPL1);
    mu_run_test(BPL2);
    mu_run_test(BIT1);
    mu_run_test(BIT2);
    mu_run_test(CLC1);
    mu_run_test(CMP1);
    mu_run_test(CMP2);
    mu_run_test(CPX1);
    mu_run_test(CPX2);
    mu_run_test(DEY1);
    mu_run_test(DEY2);
    mu_run_test(DEY3);
    mu_run_test(INC1);
    mu_run_test(INC2);
    mu_run_test(INX1);
    mu_run_test(INX2);
    mu_run_test(JMP1);
    mu_run_test(JSR1);
    mu_run_test(LDA1);
    mu_run_test(LDA2);
    mu_run_test(LDX1);
    mu_run_test(LDX2);
    mu_run_test(LDY1);
    mu_run_test(LDY2);
    mu_run_test(LSR1);
    mu_run_test(LSR2);
    mu_run_test(NOP1);
    mu_run_test(RTS1);
    mu_run_test(SBC1);
    mu_run_test(SBC2);
    mu_run_test(SBC3);
    mu_run_test(SBC4);
    mu_run_test(SBC5);
    mu_run_test(SBC6);
    mu_run_test(SBC7);
    mu_run_test(SEC1);
    mu_run_test(TAY1);
    mu_run_test(TYA1);
    mu_run_test(TXA1);
    mu_run_test(STA1);
    mu_run_test(STX1);
    mu_run_test(EOR1);
    mu_run_test(EOR2);
    mu_run_test(EOR3);
    mu_run_test(RTI1);
    mu_run_test(RTI2);
    mu_run_test(RTI3);
    mu_run_test(ROL1);
    mu_run_test(ROL2);
    mu_run_test(ROR1);
    mu_run_test(SEI1);
    mu_run_test(TXS1);
    mu_run_test(TSX1);
    mu_run_test(SED1);
    return 0;
}

int main() {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);
    return result != 0;
}
