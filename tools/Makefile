../bin/cpu.o: ../src/cpu.c ../src/cpu.h
	gcc -c ../src/cpu.c -o ../bin/cpu.o
../bin/testCPU.o: ../tests/testCPU.c ../tests/minunit.h ../src/cpu.h
	gcc -c ../tests/testCPU.c -o ../bin/testCPU.o
../bin/testCPU: ../bin/cpu.o ../bin/testCPU.o
	gcc ../bin/cpu.o ../bin/testCPU.o  -o ../bin/testCPU
../bin/run_ops: ../bin/run_ops.o ../bin/load_prog.o
	gcc -o ../bin/run_ops ../bin/run_ops.o ../bin/load_prog.o
../bin/run_ops.o: ../src/run_ops.c ../src/opcodes.h ../src/load_prog.h
	gcc -c ../src/run_ops.c -o ../bin/run_ops.o
../bin/load_prog.o: ../src/load_prog.c ../src/load_prog.h
	gcc -c ../src/load_prog.c -o ../bin/load_prog.o

all: ../bin/testCPU
runops: ../bin/run_ops
