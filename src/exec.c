#include <stdio.h>
#include <math.h>
#include "opcodes.h"
#include "exec.h"

unsigned int get_hex_from_char(char c) {
    unsigned int hex_val;
    if (c-'0' >= 0 && c-'0' <= 9) {
        hex_val = c-'0';
    } else if (c-'a' >= 0 && c-'f' <= 0) {
        hex_val = c-'a' + 10;
    } else {
        printf("Invalid hex symbol\n");
        return 0;
    }
    return hex_val;
}

unsigned int get_hex_from_chars(char *c) {
    unsigned int hex_val = 0, length;
    for (int i = 0; c[i] != '\0' && c[i] != ' ' && c[i] != '\n'; i++)
        hex_val = get_hex_from_char(c[i]) + (hex_val << 4);
    return hex_val;
}

int16_t load_program(int argc, char **argv, uint8_t *mem, int16_t start) {
    /*
     * Reads the specified hex dump file and loads it into memory
     * mem should have allocated 0xFFFF entries of 1 byte of memory
     * Returns the program counter that corresponds to the last
     * instruction
     */
    if (argc < 2) {
        printf("Error: expecting command line argument with hex dump file to be opened.\n");
        return start;
    }
    if (argc > 2) {
        printf("Error: too many command line arguments (only 1 is needed)\n");
        return start;
    }

    FILE *hex;

    if ( (hex = fopen(argv[1], "r")) == NULL) {
        printf("Error: unable to open file %s\n", argv[1]);
        return start;
    }

    int16_t pc;
    uint8_t hex_val;
    // records how far into scan buffer the scanner is in
    unsigned int bufc = 0;
    char scan_buf[6], val;

    pc = start;

    while ((val = getc(hex)) != EOF) {
        while (val != '\n' && val != ' ' && val != EOF) {
            scan_buf[bufc++] = val;
            val = getc(hex);
        }
        scan_buf[bufc] = '\0';
        if (bufc && scan_buf[bufc-1] != ':')
            mem[pc++] = (uint8_t) get_hex_from_chars(scan_buf);
        bufc = 0;
    }

    fclose(hex);

    return pc;
}

void test_1() {
    char *test_words[6] = {"0600: ", "8f", "16", "3f", " 4d", "4d "};
    int8_t testhex = 0;
    for (int i = 0; i < sizeof(test_words)/sizeof(test_words[0]); i++) {
        testhex = get_hex_from_chars(test_words[i]);
        printf("%s\t%x\t%d\n", test_words[i], testhex, testhex);
    }
}

void test_2(int argc, char **argv) {
    uint8_t mem[0xffff];
    int16_t start = 0x60, end = load_program(argc, argv, mem, start);

    if (start == end) {
        printf("Test 2 failed");
        return;
    }

    printf("Loaded program into memory location %x to %x\n", start, end);
    for (int i = start; i < end; i++)
        printf("%02x ", mem[i]);
    printf("\n");

    printf("This memory should match the input hexdump file \"%s\"\n", argv[1]);
    FILE *hexdump = fopen(argv[1], "r");
    char c;
    while ((c = getc(hexdump)) != EOF)
        printf("%c", c);
    fclose(hexdump);
}
