../bin/cpu.o: ../src/cpu.c ../src/cpu.h
	gcc -c ../src/cpu.c -o ../bin/cpu.o
../bin/testCPU.o: ../tests/testCPU.c ../tests/minunit.h ../src/cpu.h
	gcc -c ../tests/testCPU.c -o ../bin/testCPU.o
../bin/testCPU: ../bin/cpu.o ../bin/testCPU.o
	gcc ../bin/cpu.o ../bin/testCPU.o  -o ../bin/testCPU

all: ../bin/testCPU
