../bin/load_prog.o: ../src/load_prog.c ../src/load_prog.h
	gcc -c ../src/load_prog.c -o ../bin/load_prog.o
../bin/cpu.o: ../src/cpu.c ../src/cpu.h ../src/opcodes.h ../src/load_prog.h
	gcc -c ../src/cpu.c -o ../bin/cpu.o
../bin/testCPU.o: ../tests/testCPU.c ../tests/minunit.h ../src/cpu.h ../src/load_prog.h
	gcc -c ../tests/testCPU.c -o ../bin/testCPU.o
../bin/testCPU: ../bin/cpu.o ../bin/testCPU.o ../bin/load_prog.o
	gcc ../bin/cpu.o ../bin/testCPU.o ../bin/load_prog.o -o ../bin/testCPU

all: ../bin/testCPU
