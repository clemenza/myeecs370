/* FSM for LC */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "readcmd.h"

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000

#define ADD	0
#define NAND	1
#define LW	2
#define SW	3
#define BEQ	4
#define JALR	5
#define HALT	6
#define NOOP	7

typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int memoryAddress;
    int memoryData;
    int instrReg;
    int aluOperand;
    int aluResult;
    int numMemory;
} stateType;

void printState(stateType *, char *);
void run(stateType);
int memoryAccess(stateType *, int);
int convertNum(int);

int
main(int argc, char *argv[])
{
    int i;
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
	printf("error: usage: %s <machine-code file>n", argv[0]);
	exit(1);
    }

    /* initialize memories and registers */
    for (i=0; i<NUMMEMORY; i++) {
	state.mem[i] = 0;
    }
    for (i=0; i<NUMREGS; i++) {
	state.reg[i] = 0;
    }

    state.pc=0;

    /* read machine-code file into instruction/data memory (starting at
	address 0) */

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
	printf("error: can't open file %s\n", argv[1]);
	perror("fopen");
	exit(1);
    }

    for (state.numMemory=0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
	state.numMemory++) {
	if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
	    printf("error in reading address %d\n", state.numMemory);
	    exit(1);
	}
	printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    printf("\n");

    /* run never returns */
    run(state);

    return(0);
}

void
printState(stateType *statePtr, char *stateName)
{
    int i;
    static int cycle = 0;
    printf("\n@@@\nstate %s (cycle %d)\n", stateName, cycle++);
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
	for (i=0; i<statePtr->numMemory; i++) {
	    printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
	}
    printf("\tregisters:\n");
	for (i=0; i<NUMREGS; i++) {
	    printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
	}
    printf("\tinternal registers:\n");
    printf("\t\tmemoryAddress %d\n", statePtr->memoryAddress);
    printf("\t\tmemoryData %d\n", statePtr->memoryData);
    printf("\t\tinstrReg %d\n", statePtr->instrReg);
    printf("\t\taluOperand %d\n", statePtr->aluOperand);
    printf("\t\taluResult %d\n", statePtr->aluResult);
}

/*
 * Access memory:
 *     readFlag=1 ==> read from memory
 *     readFlag=0 ==> write to memory
 * Return 1 if the memory operation was successful, otherwise return 0
 */
int
memoryAccess(stateType *statePtr, int readFlag)
{
    static int lastAddress = -1;
    static int lastReadFlag = 0;
    static int lastData = 0;
    static int delay = 0;

    if (statePtr->memoryAddress < 0 || statePtr->memoryAddress >= NUMMEMORY) {
	printf("memory address out of range\n");
	exit(1);
    }

    /*
     * If this is a new access, reset the delay clock.
     */
    if ( (statePtr->memoryAddress != lastAddress) ||
	     (readFlag != lastReadFlag) ||
	     (readFlag == 0 && lastData != statePtr->memoryData) ) {
	delay = statePtr->memoryAddress % 3;
	lastAddress = statePtr->memoryAddress;
	lastReadFlag = readFlag;
	lastData = statePtr->memoryData;
    }

    if (delay == 0) {
	/* memory is ready */
	if (readFlag) {
	    statePtr->memoryData = statePtr->mem[statePtr->memoryAddress];
	} else {
	    statePtr->mem[statePtr->memoryAddress] = statePtr->memoryData;
	}
	return(1);
    } else {
	/* memory is not ready */
	delay--;
	return(0);
    }
}

int
convertNum(int num)
{
    /* convert a 16-bit number into a 32-bit integer */
    if (num & (1 << 15) ) {
	num -= (1 << 16);
    }
    return(num);
}

void 
run(stateType state)
{
	int bus;///used to connect other components together,we only need this var besides stateType itself
	fetch:
		printState(&state, "fetch");
		bus = state.pc;
		state.memoryAddress = bus;
		if(memoryAccess(&state, 1))
			goto decode;
		else
			goto fetch;

	decode:
		printState(&state, "decode");
		bus = state.memoryData;
		state.instrReg = bus;
		if( OPCODE(state.instrReg) == ADD)
			goto add;
		else if( OPCODE(state.instrReg) == NAND)
			goto nand;
		else if( OPCODE(state.instrReg) == LW)
			goto lw;
		else if( OPCODE(state.instrReg) == SW)
			goto sw;
		else if( OPCODE(state.instrReg) == BEQ)
			goto beq;
		else if( OPCODE(state.instrReg) == JALR)
			goto jalr;
		else if( OPCODE(state.instrReg) == HALT)
			goto halt;
		else	goto noop;

	add:
		printState(&state, "add");
		state.aluOperand = state.reg[REGA(state.instrReg)];
		bus = state.reg[REGB(state.instrReg)];
		state.aluResult = state.aluOperand + bus;
		bus = state.aluResult;
		state.reg[destReg(state.instrReg)] = bus;
		state.pc++;
		goto fetch;

	nand:
		printState(&state, "nand");	
		state.aluOperand = state.reg[REGA(state.instrReg)];
		bus = state.reg[REGB(state.instrReg)];
		state.aluResult = ~(state.aluOperand & bus);
		bus = state.aluResult;
		state.reg[destReg(state.instrReg)] = bus;
		state.pc++;
		goto fetch;
	
	lw:
		printState(&state, "lw");
		state.aluOperand = state.reg[REGA(state.instrReg)];
		bus = convertNum(state.instrReg & 0xFFFF);
		state.aluResult = state.aluOperand + bus;
		bus = state.aluResult;
		state.memoryAddress = bus;
		if(memoryAccess(&state, 1))
			goto lw2;
		else
			goto lw;
	
	lw2:
		bus = state.memoryData;
		state.reg[REGB(state.instrReg)] = bus;
		state.pc++;
		goto fetch;

	sw:
		printState(&state, "sw");
		state.aluOperand = state.reg[REGA(state.instrReg)];
		bus = convertNum(state.instrReg & 0xFFFF);
		state.aluResult = state.aluOperand + bus;
		bus = state.aluResult;
		state.memoryAddress = bus;
		bus = state.reg[REGB(state.instrReg)];
		state.memoryData = bus;
		if(memoryAccess(&state, 0))
			goto sw2;
		else
			goto sw;
	sw2:
		state.pc++;
		goto fetch;

	beq:
		printState(&state, "beq");
		state.aluOperand = state.reg[REGA(state.instrReg)];
		bus = state.reg[REGB(state.instrReg)];
		state.aluResult = state.aluOperand - bus;
		if( state.aluResult == 0){///pending...need review,be doubted disobey specified rule
			bus = convertNum(state.instrReg & 0xFFFF);
			state.pc += (1 + bus);
			goto fetch;
				}
			
		else	{
			state.pc++;
			goto fetch;
				}

	jalr:
		printState(&state, "jalr");
		state.pc++;
		bus = state.pc;
		state.reg[REGB(state.instrReg)] = bus;
		bus = state.reg[REGA(state.instrReg)];
		state.pc = bus;
		goto fetch;

	halt:
		printState(&state, "halt");
		exit(0);

	noop:
		printState(&state, "noop");
		state.pc++;
		goto fetch;
				
}

