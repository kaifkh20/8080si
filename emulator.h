#ifndef EMULATOR_H

#define EMULATOR_H

#include <stdint.h>


/*
 * This can be also interpreted as Flags. Flags in a 8 bit regester with 5 one bit flag and a 3bit flag;
 * */

/* This is the link which can be referred to http://www.uyar.com/files/301/ch2.pdf . */

typedef struct {
	uint8_t z:1; //
	uint8_t s:1;
	uint8_t p:1;
	uint8_t cy:1;
	uint8_t ac:1;
	uint8_t pad:3;
}ConditionCodes;


typedef struct{
	uint8_t a,b,c,d,e,h,l;
	uint16_t sp,pc;
	uint8_t *memory;
	ConditionCodes cc;
	uint8_t int_enable; //interrput enable

}State8080;

void UnimplementedInstruction(State8080* state);
void Emulate8080(State8080* state);
State8080* Init8080(void);

#endif
