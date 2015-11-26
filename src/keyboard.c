#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include "keyboard.h"
 
struct termios initial_settings,
               new_settings;

char getChar(){
  //code from http://cboard.cprogramming.com/c-programming/130243-non-blocking-getchar.html
  int n;
  unsigned char key;
  tcgetattr(0,&initial_settings);
  new_settings = initial_settings;
  new_settings.c_lflag &= ~ICANON;
  new_settings.c_lflag &= ~ECHO;
  new_settings.c_lflag &= ~ISIG;
  new_settings.c_cc[VMIN] = 0;
  new_settings.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &new_settings);
  n = getchar();
  if(n != EOF) {
    key = n;
  } else {
    key = -1;
  }
  tcsetattr(0, TCSANOW, &initial_settings);
  return key;
}

char getDirection(){
 //noblocking, reads from user input and returns direction enum
 //where val depeneds on if one of wasd keys are pressed
    char key = getChar();
    switch(key){
        case 97:
            return left;
            break;
        case 100:
            return right;
            break;
        case 119:
            return up;
            break;
        case 115:
            return down;
            break;
        default:
            return none;
            break;
    }
}
