#ifndef LOAD_PROG_H
#define LOAD_PROG_H

#define int8_t  __int8_t
#define int16_t __int16_t
#define uint8_t __uint8_t

int get_hex_from_char(char c);
int get_hex_from_chars(char *c);
int16_t load_program(int argc, char **argv, int8_t *mem, int16_t start);

#endif
