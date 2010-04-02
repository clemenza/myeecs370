#include "apue.h"

///define how to extract needed info from machine code
//
#define OPCODE(X) ((X & (7 << 22)) >> 22)
#define REGA(R)   ((R & (7 << 19)) >> 19)
#define REGB(R)   ((R & (7 << 16)) >> 16)
#define destReg(R)  (R & 7) 

///extract lowest 16-bit offset
#define OFFSET(X) ( X & 0xFFFF)
