#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	//printf("ARGC : %d\n",argc);
	
	if(argc<2){
		printf("No rom provided\n");
		exit(1);
	}

	int fsize = ReadFileIntoMemoryAt(state,argv[1],0x100);
	
	state->memory[0] = 0xc3;
	state->memory[1] = 0;
	state->memory[2] = 0x01;


	state->memory[368] = 0x7;

	//Skip DAA test    
	state->memory[0x59c] = 0xc3; //JMP    
	state->memory[0x59d] = 0xc2;    
	state->memory[0x59e] = 0x05;    

	if(argc>2 && strncmp(argv[2],"-d",1)==0){
		printf("DISASSEMBLER CALLED\n");
		DisassembleHelper(state,fsize);
		return 0;
	}
	
	//Emulate8080(state);

	while(state->pc<fsize){
		Emulate8080(state);
		//if(state->pc==0x108) break;
	}	
	printf("EMULATION FINISHED\n");
	

}

