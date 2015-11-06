#include <stdio.h>
#include "opcodes.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Error: expecting command line argument with hex dump file to be opened.\n");
        return 1;
    }
    if (argc > 2) {
        printf("Error: too many command line arguments (only 1 is needed)\n");
        return 1;
    }

    FILE *hex;

    if ( (hex = fopen(argv[1], "r")) == NULL) {
        printf("Error: unable to open file %s\n", argv[1]);
        return 2;
    }

    unsigned int pc, hex_val;
    fscanf(hex, "%x: ", &hex_val);
    pc = hex_val;

    //while (hex_val != EOF) {
    for (int i = 0; i < 10; i++) {
        printf("hex: %x, dec:%d\n", hex_val, hex_val);
        fscanf(hex, "%x", &hex_val);
    }

    //printf("%s starts with a pointer counter value of 0x%x (%d)\n", argv[1], pc, pc);


    fclose(hex);
    return 0;
}
