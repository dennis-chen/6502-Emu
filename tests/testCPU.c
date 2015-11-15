#include "minunit.h"
#include "../src/cpu.h"

int tests_run = 0;

int8_t testADCHelper(CPU *c, int8_t accumByte, int8_t operand){
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,Immediate);
    setRegByte(c,ACCUM,accumByte);
    ADC(c,o);
    freeOP_CODE_INFO(o);
    return getRegByte(c,ACCUM);
}

int8_t testSBCHelper(CPU *c, int8_t accumByte, int8_t operand){
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,Immediate);
    setRegByte(c,ACCUM,accumByte);
    SBC(c,o);
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
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,Immediate);
    setRegByte(c,ACCUM,accum);
    AND(c,o);
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
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,Immediate);
    setRegByte(c,ACCUM,accum);
    AND(c,o);
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
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,Accumulator);
    ASL(c,o);
    int8_t accumVal = getRegByte(c,ACCUM);
    mu_assert("ASL1 err, ACCUM reg != -2", accumVal == -2);
    mu_run_test_with_args(testRegStatus,c,"10100001",
            "          NVUBDIZC    NVUBDIZC\nASL1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * ASL2() {
    //Non Accumulator addressing mode
    //TODO: Add test of writing to given memory address
    printf("TODO: finish implementing ASL2\n");
    CPU *c = getCPU();
    int8_t operand = 0xFF;
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,Immediate);
    ASL(c,o);
    int8_t accumVal = getRegByte(c,ACCUM);
    mu_assert("ASL2 err, ACCUM reg != 0", accumVal == 0);
    mu_run_test_with_args(testRegStatus,c,"10100001",
            "          NVUBDIZC    NVUBDIZC\nASL2 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * BNE1() {
    CPU *c = getCPU();
    uint16_t address = 0x8FFE;
    OP_CODE_INFO *o = getOP_CODE_INFO(0,address,Immediate);
    setFlag(c,Z,0);
    BNE(c,o);
    mu_assert("BNE1 err, PC != 0x8FFE", c->PC == 0x8FFE);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * BNE2() {
    CPU *c = getCPU();
    uint16_t address = 0xFFFF;
    OP_CODE_INFO *o = getOP_CODE_INFO(0,address,Immediate);
    setFlag(c,Z,1);
    BNE(c,o);
    mu_assert("BNE1 err, PC != 0", c->PC == 0);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * CLC1() {
    CPU *c = getCPU();
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,Immediate);
    setFlag(c,C,1);
    mu_run_test_with_args(testRegStatus,c,"00100001",
            "          NVUBDIZC    NVUBDIZC\nCLC1 err, %s != %s");
    CLC(c,o);
    mu_run_test_with_args(testRegStatus,c,"00100000",
            "          NVUBDIZC    NVUBDIZC\nCLC1 err, %s != %s");
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * DEY1() {
    //decrement Y reg by 1 with neg twos complement val
    //in Y reg
    CPU *c = getCPU();
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,Immediate);
    setRegByte(c,IND_Y,-34);
    DEY(c,o);
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
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,Immediate);
    setRegByte(c,IND_Y,1);
    DEY(c,o);
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
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,Immediate);
    setRegByte(c,IND_Y,0);
    DEY(c,o);
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
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,Immediate);
    setRegByte(c,ACCUM,10);
    LDA(c,o);
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
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,Immediate);
    LDA(c,o);
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
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,Immediate);
    setRegByte(c,ACCUM,10);
    LDX(c,o);
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
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,Immediate);
    LDX(c,o);
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
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,Immediate);
    setRegByte(c,ACCUM,10);
    LDY(c,o);
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
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,Immediate);
    LDY(c,o);
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
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,Immediate);
    SEC(c,o);
    mu_assert("SEC1 err, Carry flag != 1", getFlag(c,C) == 1);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * TAY1() {
    CPU *c = getCPU();
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,Immediate);
    setRegByte(c,ACCUM,-1);
    TAY(c,o);
    int8_t yVal = getRegByte(c,IND_Y);
    mu_assert("TAY1 err, Y reg != 0xFE", yVal == -1);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * TYA1() {
    CPU *c = getCPU();
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,Immediate);
    setRegByte(c,IND_Y,-39);
    TYA(c,o);
    int8_t accumVal = getRegByte(c,ACCUM);
    mu_assert("TYA1 err, ACCUM reg != 0xFE", accumVal == -39);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * STA1() {
    CPU *c = getCPU();
    uint16_t address = 0xFFEE;
    OP_CODE_INFO *o = getOP_CODE_INFO(0,address,Immediate);
    setRegByte(c,ACCUM,-39);
    STA(c,o);
    int8_t addrVal = read(c,address);
    mu_assert("STA1 err, address at 0xFFEE != -39", addrVal == -39);
    freeOP_CODE_INFO(o);
    free(c);
    return 0;
}

static char * STX1() {
    CPU *c = getCPU();
    uint16_t address = 0xFFEE;
    OP_CODE_INFO *o = getOP_CODE_INFO(0,address,Immediate);
    setRegByte(c,IND_X,-39);
    STX(c,o);
    int8_t addrVal = read(c,address);
    mu_assert("STX1 err, address at 0xFFEE != -39", addrVal == -39);
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
    mu_run_test(BNE1);
    mu_run_test(BNE2);
    mu_run_test(CLC1);
    mu_run_test(DEY1);
    mu_run_test(DEY2);
    mu_run_test(DEY3);
    mu_run_test(LDA1);
    mu_run_test(LDA2);
    mu_run_test(LDX1);
    mu_run_test(LDX2);
    mu_run_test(LDY1);
    mu_run_test(LDY2);
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
    mu_run_test(STA1);
    mu_run_test(STX1);
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
