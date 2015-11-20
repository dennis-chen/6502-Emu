#include "minunit.h"
#include "../src/cpu.h"
#include "../src/load_prog.h"

int main() {
    CPU *c = getCPU();
    c->PC = 0x600;
    int argc = 2;
    char *argv[2] = {"./testCPU", "../hexdump/fibonacci"};
    int16_t end = load_program(argc, argv, c->addressSpace, c->PC);
    if (c->PC == end) {
        printf("testFib failed\n");
        return 1;
    }
    run_ops(c, end);
    printf("\n");
    freeCPU(c);
    return 0;
}
