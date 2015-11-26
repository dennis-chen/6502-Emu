../bin/gfx.o: ../src/gfx.c ../src/gfx.h
	gcc -c ../src/gfx.c -o ../bin/gfx.o
../bin/load_prog.o: ../src/load_prog.c ../src/load_prog.h
	gcc -c ../src/load_prog.c -o ../bin/load_prog.o
../bin/cpu.o: ../src/cpu.c ../src/cpu.h ../src/opcodes.h ../src/load_prog.h ../src/gfx.h
	gcc -c ../src/cpu.c -o ../bin/cpu.o

#testOPCODES runs op code test suite
../bin/testOPCODES.o: ../tests/testOPCODES.c ../tests/minunit.h ../src/cpu.h 
	gcc -c ../tests/testOPCODES.c -o ../bin/testOPCODES.o
../bin/testOPCODES: ../bin/gfx.o ../bin/cpu.o ../bin/testOPCODES.o 
	gcc ../bin/cpu.o ../bin/gfx.o ../bin/testOPCODES.o  -o ../bin/testOPCODES -lX11 -lm

#testFib loads and runs fib assembly program 
../bin/testFib.o: ../tests/testFib.c ../src/cpu.h ../src/load_prog.h
	gcc -c ../tests/testFib.c -o ../bin/testFib.o
../bin/testFib: ../bin/gfx.o ../bin/cpu.o ../bin/load_prog.o ../bin/testFib.o 
	gcc ../bin/cpu.o ../bin/load_prog.o ../bin/testFib.o ../bin/gfx.o -o ../bin/testFib -lX11 -lm

all: ../bin/testOPCODES ../bin/testFib
