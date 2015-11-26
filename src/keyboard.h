#ifndef KEYBOARD
#define KEYBOARD

#include <termios.h>
 
struct termios initial_settings,
               new_settings;

enum DIRECTION {
    left,
    right,
    up,
    down,
    none
};
 
char getChar();
char getDirection();

#endif
