#ifndef EXEC_H
#define EXEC_H

#define int8_t  __int8_t
#define int16_t __int16_t
#define uint8_t __uint8_t

unsigned int get_hex_from_char(char c);
unsigned int get_hex_from_chars(char *c);
int16_t load_program(int argc, char **argv, uint8_t *mem, int16_t start);

#endif
