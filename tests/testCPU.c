#include "minunit.h"
#include "../src/cpu.h"

int tests_run = 0;

static char * testADC() {
    CPU *c = getCPU();
    print(c);
    OP_CODE_INFO *o = getOP_CODE_INFO(0,0,1);
    //mu_assert("error, bar != 5", bar == 5);
    return 0;
}

static char * all_tests() {
    mu_run_test(testADC);
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
