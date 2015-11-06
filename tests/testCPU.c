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

static char * ADC1() {
    //adding positives no overflow
    CPU *c = getCPU();
    int8_t accumVal = testADCHelper(c,13,14);
    mu_assert("ADC test 1 err, ACCUM reg != 27", accumVal == 27);
    mu_assert("ADC test 2 err, V flag != 0", getFlag(c,V) == 0);
    free(c);
    return 0;
}

static char * ADC2() {
    //adding positive to negative
    CPU *c = getCPU();
    int8_t accumVal = testADCHelper(c,-100,30);
    mu_assert("ADC test 2 err, ACCUM reg != -70", accumVal == -70);
    mu_assert("ADC test 2 err, V flag != 0", getFlag(c,V) == 0);
    free(c);
    return 0;
}

static char * ADC3() {
    //adding positives overflow
    CPU *c = getCPU();
    print(c);
    int8_t accumVal = testADCHelper(c,104,45);
    mu_assert("ADC test 3 err, ACCUM reg != -107", accumVal == -107);
    print(c);
    mu_assert("ADC test 3 err, V flag != 1", getFlag(c,V) == 1);
    free(c);
    return 0;
}

static char * ADC4() {
    //adding negatives overflow
    CPU *c = getCPU();
    int8_t accumVal = testADCHelper(c,-103,-69);
    mu_assert("ADC test 4 err, ACCUM reg != 84", accumVal == 84);
    mu_assert("ADC test 4 err, V flag != 1", getFlag(c,V) == 1);
    free(c);
    return 0;
}

static char * all_tests() {
    mu_run_test(ADC1);
    mu_run_test(ADC2);
    mu_run_test(ADC3);
    mu_run_test(ADC4);
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
