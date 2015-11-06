#include <stdio.h>
#include "opcodes.h"

unsigned int get_hex_from_chars(char *c) {
    //
    if (c[0]-'0' >= '0' && c[0]-'0' =< '9') {
        c[0]-'0'
}

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

    // records how far into scan buffer the scanner is in
    unsigned int bufc;

    char scan_buf[5], val;
    //fscanf(hex, "%x: ", &hex_val);
    //pc = hex_val;

    /*for (int i = 0; i < 50; i++) {
        printf("%x\n", hex_val);
        if (hex_val > 0xff) {
            fscanf(hex, "%x: ", &hex_val);
        }
        fscanf(hex, "%x", &hex_val);
    }*/

    while ((val = getc(hex)) != EOF) {
        if (val == ' ')
            break;
        scan_buf[bufc] = val;
        bufc++;
    }
    scan_buf[bufc-1] = '\0';
    bufc = 0;

    printf("PC: %s\n", scan_buf);

    while ((val = getc(hex)) != EOF) {
        if (val == '\n') {
            scan_buf[bufc] = '\0';
            printf("%s ", scan_buf);
            bufc = 0;
            while ((val = getc(hex)) != EOF && val != ':') {
                // pass the PC
            }
        }
        else if (val == ' ') {
            scan_buf[bufc] = '\0';
            printf("%s ", scan_buf);
            bufc = 0;
        } else {
            scan_buf[bufc] = val;
            bufc++;
        }
    }

    fclose(hex);
    return 0;
}
