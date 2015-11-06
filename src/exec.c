#include <stdio.h>
#include <math.h>
#include "opcodes.h"

unsigned int get_hex_from_char(char c);
unsigned int get_hex_from_chars(char *c);

unsigned int get_hex_from_char(char c) {
    unsigned int hex_val;
    if (c-'0' >= 0 && c-'0' <= 9) {
        hex_val = c-'0';
    } else if (c-'a' >= 0 && c-'f' <= 0) {
        hex_val = c-'a' + 10;
    }
    return hex_val;
}

unsigned int get_hex_from_chars(char *c) {
    unsigned int hex_val = 0, length;
    for (int i = 0; c[i] != '\0'; i++)
        hex_val = get_hex_from_char(c[i]) + (hex_val << 4);
    return hex_val;
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

    unsigned int mem[0xffff];
    unsigned int pc, hex_val;
    // records how far into scan buffer the scanner is in
    unsigned int bufc;
    char scan_buf[5], val;

    while ((val = getc(hex)) != EOF) {
        if (val == ' ')
            break;
        scan_buf[bufc] = val;
        bufc++;
    }
    scan_buf[bufc-1] = '\0';
    bufc = 0;

    pc = get_hex_from_chars(scan_buf);

    while ((val = getc(hex)) != EOF) {
        if (val == '\n') {
            scan_buf[bufc] = '\0';
            mem[pc++] = get_hex_from_chars(scan_buf);
            bufc = 0;
            while ((val = getc(hex)) != EOF && val != ':') {
                // pass the PC
            }
        }
        else if (val == ' ') {
            scan_buf[bufc] = '\0';
            mem[pc++] = get_hex_from_chars(scan_buf);
            bufc = 0;
        } else {
            scan_buf[bufc] = val;
            bufc++;
        }
    }

    fclose(hex);

    for (int i = 0x600; i < 0x61c; i++) {
        printf("%x ", mem[i]);
    }
    printf("\n");

    return 0;
}
