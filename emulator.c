#include <stdint.h>
#include "emulator.h"
#include <stdlib.h>
#include <stdio.h>

void UnimplementedInstruction(State8080* state){
    printf("Error : Unimplemented Instruction\n");
    exit(1);
}


int Parity(int x, int size)
{
	int i;
	int p = 0;
	x = (x & ((1<<size)-1));
	for (i=0; i<size; i++)
	{
		if (x & 0x1) p++;
		x = x >> 1;
	}
	return (0 == (p & 0x1));
}

void LogicFlagsA(State8080 *state)
{
	state->cc.cy = state->cc.ac = 0;
	state->cc.z = (state->a == 0);
	state->cc.s = (0x80 == (state->a & 0x80));
	state->cc.p = Parity(state->a, 8);
}

void ArithFlagsA(State8080 *state, uint16_t res)
{
	state->cc.cy = (res > 0xff);
	state->cc.z = ((res&0xff) == 0);
	state->cc.s = (0x80 == (res & 0x80));
	state->cc.p = Parity(res&0xff, 8);
}



void Emulate8080(State8080* state){
    uint8_t *opcode = &state->memory[state->pc];
    
    state->pc +=1;

    switch(*opcode){
        case 0x00: break;
        case 0x01:{
            state->c = opcode[1];
            state->d = opcode[2];
            state->pc+=2;
            break;
        }
        case 0x02:{
            UnimplementedInstruction(state);
            break;
        }
        case 0x03:{
 //         UnimplementedInstruction(state);
            state->b++;
            state->c++;
            break;
        }
        case 0x04:{
            UnimplementedInstruction(state);
            break;
        }
        case 0x05:{
            uint8_t res = state->b -1 ;
            state->cc.z = (res==0);
            state->cc.s = (0x80==(res&0x80));
            state->cc.p = Parity(res,8);
            state->b = res;
            break;
        }
        case 0x06:{
            state->b = opcode[1];
            state->pc+=1;
            break;
        }
        case 0x09:{
            uint32_t hl = (state->h<<8) | (state->l);
            uint32_t bc = (state->b<<8) | (state->c);
            uint32_t res = hl+bc;
            state->h = (res&0xff00) >> 8;
            state->l = res&0xff;
            state->cc.cy = ((res&0xffff0000) > 0);

        }
        case 0x0d:{
            uint8_t res = state->c -1;
            state->cc.z = (res==0);
            state->cc.s = (0x80==(res&0x80));
            state->cc.p = Parity(res,8);
            state->c = res;
            break;

        }
        case 0x0e:{
            state->c = opcode[1];
            state->pc +=1;
            break;
        }
        case 0x0f:{
            state->a >>=1;
            state->cc.cy = (1==(state->a & 1));
            break;
        }
        case 0x11:{
            state->e = opcode[1];
            state->d = opcode[2];
            state->pc+=2;
            break;
        }
        case 0x13:{
            state->e++;
            if(state->e==0) state->d++; //if the e overflows and become 0 then we increment d to mimic de+1
            break;
        }
        case 0x19:{
            uint32_t hl = (state->h<<8) | state->l;
            uint32_t de = (state->d<<8) | state->e;
            uint32_t res = hl + de;
            state->h = (res&0xff00)>>8;
            state->l = res&0xff;
            state->cc.cy = ((res&0xffff0000)!=0);
            break;
        }
        case 0x1a:{
            uint16_t offset = (state->d<<8) | state->e;
            state->a = state->memory[offset];
            break;
        }
        case 0x21:{
            state->h = opcode[2];
            state->l = opcode[1];
            state->pc+=2;
            break;
        }
        case 0x23:{
            state->l++;
            if(state->l==0) state->h++;
            break;
        }
        case 0x26:{
            state->h = opcode[1];
            state->pc++;
            break;
        }
        case 0x29:{
            uint32_t hl = (state->h<<8) | (state->l);
            uint32_t res = hl+hl;
            state->h = (res&0xff00)>>8;
            state->l = res&0xff;
            state->cc.cy = ((res&0xffff0000)!=0);
            break;
        }
        case 0x31:{
            state->sp = (opcode[2]<<8)|opcode[1];
            state->pc+=2;
            break;
        }
        case 0x32:{
            uint16_t offset = (opcode[2]<<8) | (opcode[1]);
            state->memory[offset] = state->a;
            state->pc+=2;
            break;
        }
        case 0x36:{
            uint16_t offset = (state->h<<8) | state->l;
            state->memory[offset] = opcode[1];
            state->pc+=1;
            break;
        }
        case 0x3a:{
            uint16_t offset = (opcode[2]<<8) | (opcode[1]);
            state->a = state->memory[offset];
            state->pc+=2;
            break;
        }
        case 0x3e:{
            state->a = opcode[1];
            state->pc+=1;
            break;
        }
        case 0x41:{
            state->b = state->c;
            break;
        }
        case 0x42:{
            state->b = state->d;
            break;
        }
        case 0x43:{
            state->b = state->e;
            break;
        }
        case 0x56:{
            uint16_t offset = (state->h<<8) | state->l;
            state->d = state->memory[offset];
            break;
        }
        case 0x5e:{
            uint16_t offset = (state->h<<8) | state->l;
            state->e = state->memory[offset];
            break;
        }
        case 0x66:{
            uint16_t offset = (state->h<<8) | state->l;
            state->h = state->memory[offset];
            break;
        }
        case 0x77:{
            uint16_t offset = (state->h<<8) | state->l;
            state->memory[offset] = state->a;
            break;
        }
        case 0x7a:{ 
            state->a = state->d;
            break;
        }
        case 0x7b:{
            state->a = state->e;
            break;
        }
        case 0x7c:{
            state->a = state->h;
            break;
        }
        case 0x7d:{
            state->a = state->l;
            break;
        }
        case 0x7e:{
            uint16_t offset = (state->h<<8) | (state->l);
            state->a = state->memory[offset];
            break;
        }
        case 0x80:{
            uint16_t answer = (uint16_t)state->a + (uint16_t) state->b;

            if((answer & 0xff) == 0){
                state->cc.z = 1;
            }else state->cc.z = 0;

            if(answer&0x80){
                state->cc.s = 1;
            }else state->cc.s = 0;

            if(answer>0xff){
                state->cc.cy = 1;
            }else state->cc.cy = 0;

            state->cc.p = Parity(answer&0xff,8);
            state->a = answer & 0xff;
        }
        case 0x81:{
            uint16_t answer = (uint16_t)state->a + (uint16_t)state->c;
            state->cc.z = ((answer&0xff)==0);
            state->cc.s = ((answer&0x80)!=0);
            state->cc.cy = ((answer>0xff));
            state->cc.p = Parity(answer&0xff,8);
            state->a = answer&0xff;
        }

        case 0x86:{
            uint16_t offset = (state->h<<8) | (state->l);
            uint16_t answer = (uint16_t) state->a + state->memory[offset];
            state->cc.z = ((answer&0xff) == 0);
            state->cc.s = ((answer&0x80)!=0);
            state->cc.cy =  (answer>0xff);
            state->cc.p = Parity(answer&0xff,8);
            state->a = answer&0xff;
        }
        case 0xa7:{
            state->a&=state->a;
            LogicFlagsA(state);
            break;
        }
        case 0xaf:{
            state->a^=state->a;
            LogicFlagsA(state);
            break;
        }
        case 0xc1:{
            state->c = state->memory[state->sp];
            state->b = state->memory[state->sp+1];
            state->sp+=2;
            break;
        }
        case 0xc2:{
            if(0==state->cc.z){
                state->pc = opcode[2]<<8 | opcode[1];
            }else{
                state->pc+=2;
            }
            break;
        }
        case 0xc3:{
            state->pc = (opcode[2]<<8) | opcode[1];
            //state->pc+=2;
            break;
        }
        case 0xc5:{
            state->memory[state->sp-2] = state->c;
            state->memory[state->sp-1] = state->b;
            state->sp-=2;
            break;
        }
        case 0xc6:{
            uint16_t x = (uint16_t) state->a + (uint16_t) opcode[1];
            state->cc.z = ((x & 0xff) == 0);
            state->cc.s = (0x80 == (x & 0x80));
            state->cc.p = Parity((x&0xff), 8);
            state->cc.cy = (x > 0xff);
            state->a = (uint8_t) x;
            state->pc++;
            break;
        }
        case 0xc9:{
            state->pc = state->memory[state->sp] | state->memory[state->sp+1]<<8;
            state->sp+=2;
            break;
        }
        case 0xcd:{
            uint16_t	ret = state->pc+2;
            state->memory[state->sp-1] = (ret >> 8) & 0xff;
            state->memory[state->sp-2] = (ret & 0xff);
            state->sp = state->sp - 2;
            state->pc = (opcode[2] << 8) | opcode[1];
            break;
        }
        case 0xd1:{
            state->e = state->memory[state->sp];
            state->d = state->memory[state->sp+1];
            state->sp+=2;
            break;
        }
        case 0xd3:{
            state->pc++;
            break;
        }
        case 0xd5:{
            state->memory[state->sp-2] = state->e;
            state->memory[state->sp-1] = state->d;
            state->sp-=2;
            break;
        }
        case 0xe1:{
            state->l = state->memory[state->sp];
            state->h = state->memory[state->sp+1];
            state->sp+=2;
            break;
        }
        case 0xe5:{
            state->memory[state->sp-2] = state->l;
            state->memory[state->sp-1] = state->h;
            state->sp-=2;
            break;
        }
        case 0xe6:{
            state->a&=opcode[1];
            LogicFlagsA(state);
            state->pc++;
            break;
        }
        case 0xeb:{
            uint8_t temp1 = state->d;
            state->d = state->h;
            state->h = temp1;

            uint8_t temp2 = state->l;
            state->l = state->e ;
            state->e = temp2;

            break;
        }
        case 0xf1:{
            state->a = state->memory[state->sp+1];
            uint8_t psw = state->memory[state->sp];
            state->cc.z  = (0x01 == (psw & 0x01));
            state->cc.s  = (0x02 == (psw & 0x02));
            state->cc.p  = (0x04 == (psw & 0x04));
            state->cc.cy = (0x05 == (psw & 0x08));
            state->cc.ac = (0x10 == (psw & 0x10));
            state->sp += 2;
        }
        case 0xf5:{
            state->memory[state->sp-1] = state->a;
            uint8_t psw = (state->cc.z |    
                            state->cc.s << 1 |    
                            state->cc.p << 2 |    
                            state->cc.cy << 3 |    
                            state->cc.ac << 4 );
            state->memory[state->sp-2] = psw;
            state->sp-=2;
            break;
        }
        case 0xfe:{
            uint8_t x = state->a - opcode[1];
            state->cc.z = (x==0);
            state->cc.s = (0x80 == (x & 0x80));
            state->cc.p = Parity(x,8);
            state->cc.cy = (state->a < opcode[1]);
            state->pc+=1;
            break;
        }

        default:{
            UnimplementedInstruction(state);
            break;
        }
    }
        printf("\t");
	printf("%c", state->cc.z ? 'z' : '.');
	printf("%c", state->cc.s ? 's' : '.');
	printf("%c", state->cc.p ? 'p' : '.');
	printf("%c", state->cc.cy ? 'c' : '.');
	printf("%c  ", state->cc.ac ? 'a' : '.');
	printf("A $%02x B $%02x C $%02x D $%02x E $%02x H $%02x L $%02x SP %04x\n", state->a, state->b, state->c,
				state->d, state->e, state->h, state->l, state->sp);
}

State8080* Init8080(void){
    State8080* state = calloc(1,sizeof(State8080));
    state->memory = malloc(0x10000);
    return state;
}


