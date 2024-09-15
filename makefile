all:
	gcc -o main main.c disassembler.c emulator.c -Wall -g

clean :
	rm main
