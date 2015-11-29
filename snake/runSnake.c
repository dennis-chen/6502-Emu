#include <time.h> //to seed RNG and for sleeping in game loop
#include "../src/load_prog.h"
#include "../src/gfx.h"
#include "../src/keyboard.h"
#include "../src/cpu.h"
#include "../src/load_prog.h"

void drawPoint(int x, int y){
    //draws point onto 'pixel' (actually a 8 by 8 square)
    int cornerX = x * 8;
    int cornerY = y * 8;
    gfx_rectangle(cornerX,cornerY,8,8);
}

void initializegfx(){
    int ysize = 256;
    int xsize = 256;
    gfx_open(xsize,ysize,"Snake");
    gfx_color(255,255,255);
    return;
}

void visualizeMemory(CPU *c){
    //visualizes memory addresses $0200 to $05ff
    //(32 by 32 bytes)
    //as a 256 by 256 pixel square
    gfx_clear();
    int x = 0;
    int y = 0;
    int i;
    for(i = 0x0200; i < 0x0600; i++){
        uint8_t memVal = c->addressSpace[i];
        if(memVal == 1){
            //in the snake game a memory val
            //of 1 represents a white color.
            //this special if statement is
            //because 1 needs to be mapped
            //to (255,255,255) in RGB
            gfx_color(255,255,255);
            drawPoint(x,y);
        } else if (memVal > 0){
            //this is to convert between
            //a memVal between 0-255 and
            //three 0-255 vals representing R,G,B
            //red is the first 3 bits
            uint8_t red = memVal & 0xE0;
            //blue is the next 3 bits
            uint8_t blue = (memVal << 3) & 0xE0;
            //green is the last 2
            uint8_t green = (memVal << 6) & 0xE0;
            gfx_color(red,blue,green);
            drawPoint(x,y);
        } else {}
        x++;
        if(x == 32){
            x = 0;
            y++;
        }
    }
    gfx_flush();
}

void getKeyboardInput(CPU *c){
    //reads keyboard input and
    //sets memory location $ff
    //to match keyboard input
    int dir = getDirection();
    switch(dir){
        case 0:
            //printf("left");
            c->addressSpace[0xff] = 0x61;
            break;
        case 1:
            //printf("right");
            c->addressSpace[0xff] = 0x64;
            break;
        case 2:
            //printf("up");
            c->addressSpace[0xff] = 0x77;
            break;
        case 3:
            //printf("down");
            c->addressSpace[0xff] = 0x73;
            break;
    }
}

void getRandomVal(CPU *c){
    //loads a random byte into $FE of
    //the cpu's memory (this is specifically
    //to get the snake game implementation
    //working and is NOT something the 6502
    //actually ever does
    uint8_t randNum = rand();
    c->addressSpace[0xfe] = randNum;
}

void initializerng(){
    //initialize random number generator
    srand(time(NULL));
}

void runSnake(CPU *c, int16_t end) {
    initializegfx();
    initializerng();
    int counter = 0;
    while (c->PC < end){
        getKeyboardInput(c);
        getRandomVal(c);
        run_op(c);
        visualizeMemory(c);
        nanosleep((const struct timespec[]){{0, 50000L}}, NULL);
    }
}

int main() {
    CPU *c = getCPU();
    int argc = 2;
    char *argv[2] = {"./runSnake", "../hexdump/snake"};
    int16_t end = load_program(argc, argv, c->addressSpace, 0x0600);
    printf("Focus in this terminal and press W,A,S,D to move the snake. Collect the apples and avoid colliding with the wall or yourself. Good luck!\n");
    runSnake(c, end);
    printf("\nSorry, you lose!\n");
    freeCPU(c);
    return 0;
}
