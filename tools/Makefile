../bin/gfx.o: ../src/gfx.c ../src/gfx.h
	gcc -c ../src/gfx.c -o ../bin/gfx.o
../bin/keyboard.o: ../src/keyboard.c ../src/keyboard.h
	gcc -c ../src/keyboard.c -o ../bin/keyboard.o
../bin/load_prog.o: ../src/load_prog.c ../src/load_prog.h
	gcc -c ../src/load_prog.c -o ../bin/load_prog.o
../bin/cpu.o: ../src/cpu.c ../src/cpu.h ../src/load_prog.h ../src/gfx.h ../src/keyboard.h
	gcc -c ../src/cpu.c -o ../bin/cpu.o

#testOPCODES runs op code test suite
../bin/testOPCODES.o: ../tests/testOPCODES.c ../tests/minunit.h ../src/cpu.h 
	gcc -c ../tests/testOPCODES.c -o ../bin/testOPCODES.o
../bin/testOPCODES: ../bin/gfx.o ../bin/keyboard.o ../bin/cpu.o ../bin/testOPCODES.o
	gcc ../bin/cpu.o ../bin/gfx.o ../bin/keyboard.o ../bin/testOPCODES.o  -o ../bin/testOPCODES -lX11 -lm

#testFib loads and runs fib assembly program 
../bin/runSnake.o: ../snake/runSnake.c ../src/cpu.h ../src/load_prog.h
	gcc -c ../snake/runSnake.c -o ../bin/runSnake.o
../bin/runSnake: ../bin/gfx.o ../bin/keyboard.o ../bin/cpu.o ../bin/load_prog.o ../bin/runSnake.o 
	gcc ../bin/cpu.o ../bin/keyboard.o ../bin/load_prog.o ../bin/runSnake.o ../bin/gfx.o -o ../bin/runSnake -lX11 -lm

all: ../bin/testOPCODES ../bin/runSnake
