#include "../src/cpu.h"
#include "../src/load_prog.h"

int main() {
    CPU *c = getCPU();
    int argc = 2;
    char *argv[2] = {"./runSnake", "../hexdump/snake"};
    int16_t end = load_program(argc, argv, c->addressSpace, 0x0600);
    printf("Focus in this terminal and press W,A,S,D to move the snake. Collect the apples and avoid colliding with the wall or yourself. Good luck!\n");
    run_ops(c, end);
    printf("\nSorry, you lose!\n");
    freeCPU(c);
    return 0;
}
