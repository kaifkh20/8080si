#ifndef DISASSEMBLE_H

#define DISASSEMBLE_H
#include "emulator.h"

void DisassembleHelper(State8080* state,int fsize);
int Disassemble8080(State8080* state,int fsize);

#endif
