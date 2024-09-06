#include <stdint.h>
#include "emulator.h"


void UnimplementedInstruction(State8080* state){
    printf("Error : Unimplemented Instruction\n");
    exit(1);
}


void Emulate8080(State8080* state){
    uint8_t *opcode = &state->memory[state->pc];
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
            UnimplementedInstruction(state);
            break;
        }
        case 0x04:{
            UnimplementedInstruction(state);
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
        case 0x80:{
            uint16_t answer = (uint16_t)state->a + (uint16_t) state->b;

            if(answer & 0xff == 0){
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
            state->cc.p = Parity(answer&0xff);
            state->a = answer&0xff;
        }
    }
    state->pc +=1;

}



