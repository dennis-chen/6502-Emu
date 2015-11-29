#include "../src/cpu.h"
#include "../src/load_prog.h"

int main() {
    CPU *c = getCPU();
    int argc = 2;
    char *argv[2] = {"./runSnake", "../hexdump/snake"};
    int16_t end = load_program(argc, argv, c->addressSpace, 0x0600);
    run_ops(c, end);
    freeCPU(c);
    return 0;
}
