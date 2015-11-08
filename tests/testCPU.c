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
    //adding positives no overflow
    CPU *c = getCPU();
    int8_t accumVal = testADCHelper(c,13,14);
    mu_assert("ADC1 err, ACCUM reg != 27", accumVal == 27);
    mu_run_test_with_args(testRegStatus,c,"00100000",
            "ADC1 err, %s != %s");
    free(c);
    return 0;
}

static char * ADC2() {
    //adding positive to negative
    CPU *c = getCPU();
    int8_t accumVal = testADCHelper(c,-39,92);
    mu_assert("ADC2 err, ACCUM reg != 53", accumVal == 53);
    mu_run_test_with_args(testRegStatus,c,"00100001",
            "ADC2 err, %s != %s");
    free(c);
    return 0;
}

static char * ADC3() {
    //adding positives overflow
    CPU *c = getCPU();
    int8_t accumVal = testADCHelper(c,104,45);
    mu_assert("ADC3 err, ACCUM reg != -107", accumVal == -107);
    mu_run_test_with_args(testRegStatus,c,"11100000",
            "ADC3 err, %s != %s");
    free(c);
    return 0;
}

static char * ADC4() {
    //adding negatives overflow
    CPU *c = getCPU();
    int8_t accumVal = testADCHelper(c,-103,-69);
    mu_assert("ADC4 err, ACCUM reg != 84", accumVal == 84);
    mu_run_test_with_args(testRegStatus,c,"01100001",
            "ADC4 err, %s != %s");
    free(c);
    return 0;
}

static char * ADC5() {
    //adding with carry flag

}

static char * all_tests() {
    //mu_run_test(ADC1);
    mu_run_test(ADC2);
    mu_run_test(ADC3);
    mu_run_test(ADC4);
    mu_run_test(ADC5);
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
