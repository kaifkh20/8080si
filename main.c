#include <stdio.h>
#include <stdlib.h>

#include "disassembler.h"
#include "emulator.h"

int ReadFileIntoMemoryAt(State8080* state,char* filename,uint32_t offset){
	FILE *f = fopen(filename,"rb");
	if(f==NULL){
		printf("couldn't read the file");
		exit(1);
	}

	fseek(f,0L,SEEK_END);
	int fsize = ftell(f);
	fseek(f,0L,SEEK_SET);

	unsigned char *buffer = &state->memory[offset];
	fread(buffer,fsize,1,f);
	fclose(f);

	return fsize;	
//	return 0;

}

int main(int argc,char* argv[]){
	State8080* state = Init8080();

	int fsize = ReadFileIntoMemoryAt(state,argv[0],0);
//	DisassembleHelper(state,fsize);
	while(1){
		Emulate8080(state);
		state->pc++;
	}
}

