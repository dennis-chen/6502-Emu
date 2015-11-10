#include "minunit.h"
#include "../src/cpu.h"

int tests_run = 0;

//get opcode info takes operand, address, mode

int8_t testADCHelper(CPU *c, int8_t accumByte, int8_t operand){
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,Immediate);
    setRegByte(c,ACCUM,accumByte);
    ADC(c,o);
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
            "ADC1 err, %s != %s");
    free(c);
    return 0;
}

static char * ADC2() {
    //adding positive to negative, no overflow, yes carry
    CPU *c = getCPU();
    int8_t accumVal = testADCHelper(c,-39,92);
    //mu_assert("ADC2 err, ACCUM reg != 53", accumVal == 53);
    mu_run_test_with_args(testRegStatus,c,"00100001",
            "ADC2 err, %s != %s");
    free(c);
    return 0;
}

static char * ADC3() {
    //adding positives, yes overflow, no carry
    CPU *c = getCPU();
    int8_t accumVal = testADCHelper(c,104,45);
    mu_assert("ADC3 err, ACCUM reg != -107", accumVal == -107);
    mu_run_test_with_args(testRegStatus,c,"11100000",
            "ADC3 err, %s != %s");
    free(c);
    return 0;
}

static char * ADC4() {
    //adding negatives, yes overflow, yes carry
    CPU *c = getCPU();
    int8_t accumVal = testADCHelper(c,-103,-69);
    mu_assert("ADC4 err, ACCUM reg != 84", accumVal == 84);
    mu_run_test_with_args(testRegStatus,c,"01100001",
            "ADC4 err, %s != %s");
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
            "ADC5 err, %s != %s");
    free(c);
    return 0;
}

static char * ADC6() {
    //adding in DECIMAL mode
    CPU *c = getCPU();
    setFlag(c,D,1);
    //TODO: implement decimal mode tests
    printf("TODO: implement ADC6\n");
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
    freeOP_CODE_INFO(o);
    int8_t accumVal = getRegByte(c,ACCUM);
    mu_assert("AND1 err, ACCUM reg != 0", accumVal == 0);
    mu_run_test_with_args(testRegStatus,c,"00100010",
            "AND1 err, %s != %s");
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
    freeOP_CODE_INFO(o);
    int8_t accumVal = getRegByte(c,ACCUM);
    mu_assert("AND2 err, ACCUM reg != -127", accumVal == -127);
    mu_run_test_with_args(testRegStatus,c,"10100000",
            "AND2 err, %s != %s");
    free(c);
    return 0;
}

static char * ASL1() {
    //Accumulator addressing mode
    CPU *c = getCPU();
    int8_t operand = 0xFF;
    OP_CODE_INFO *o = getOP_CODE_INFO(operand,0,Accumulator);
    ASL(c,o);
    freeOP_CODE_INFO(o);
    int8_t accumVal = getRegByte(c,ACCUM);
    mu_assert("ASL1 err, ACCUM reg != -2", accumVal == -2);
    mu_run_test_with_args(testRegStatus,c,"10100001",
            "ASL1 err, %s != %s");
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
    freeOP_CODE_INFO(o);
    int8_t accumVal = getRegByte(c,ACCUM);
    mu_assert("ASL2 err, ACCUM reg != 0", accumVal == 0);
    mu_run_test_with_args(testRegStatus,c,"10100001",
            "ASL2 err, %s != %s");
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
