#include <stdio.h>
#include <string.h>
#include "apue.h"
#include "readcmd.h"

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000

typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
int exei(stateType *);
static void add(stateType *);
static void nand(stateType *);
static void lw(stateType *);
static void sw(stateType *);
static int convertNum(int);
static void beq(stateType *);
static void jalr(stateType *);

int
main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
	printf("error: usage: %s <machine-code file>\n", argv[0]);
	exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
	printf("error: can't open file %s", argv[1]);
	perror("fopen");
	exit(1);
    }

    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
	state.numMemory++) {
	if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
	    printf("error in reading address %d\n", state.numMemory);
	    exit(1);
	}
	printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }
///printState(&state);
int ret;///return value
if ((ret = exei(&state)) < 0)
	err_sys("execute instruction failed");
    return(0);
}

void
printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
	for (i=0; i<statePtr->numMemory; i++) {
	    printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
	}
    printf("\tregisters:\n");
	for (i=0; i<NUMREGS; i++) {
	    printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
	}
    printf("end state\n");
}

int
exei(stateType *statePtr)
{

	///assume default entry point is addr 0
	statePtr->pc = 0;
	int i;
	int instr;
	int total = 0;
	for(i = 0; i < NUMREGS; i++)
		statePtr->reg[i] = 0;
	printState(statePtr);
	while(1){
		instr = OPCODE(statePtr->mem[statePtr->pc]);
		switch(instr){
			case 0 :
				   add(statePtr);
				   total++;	
				   break;
			case 1 :
				   nand(statePtr);
				   total++;	
				   break;
			case 2:
				   lw(statePtr);
				   total++;
				   break;
			case 3 :
				   sw(statePtr);
				   total++;
				   break;
			case 4 :
				   beq(statePtr);
				   total++;	
				   break;
			case 5 :
				   jalr(statePtr);
				   total++;
  				   break;
			case 6 :///halt instruction
				   total++;
				   statePtr->pc++;	
				   goto end;
			case 7 :///noop do nothing
				   total++;
				   break;
			}///end switch
		}///end while
	end:///halt
		printf("machine halted\n");
		printf("total of %d instructions executed\n", total);
		printf("final state of machine:\n");
		printState(statePtr);

	return 1;
}			

static void 
add(stateType *statePtr){

	int instr = statePtr->mem[statePtr->pc];
	statePtr->reg[destReg(instr)] = statePtr->reg[REGA(instr)] + \
		statePtr->reg[REGB(instr)];

	statePtr->pc++;///add don't change control flow as a instruction 

	printState(statePtr);
}

static void 
nand(stateType *statePtr){

	int instr = statePtr->mem[statePtr->pc];

	int antiA = ~(statePtr->reg[REGA(instr)]);
	int antiB = ~(statePtr->reg[REGB(instr)]);

	statePtr->reg[destReg(instr)] = (antiA | antiB);///we implement nand using ~ and |

	statePtr->pc++;///refer to add

	printState(statePtr);
}		

static void 
lw(stateType *statePtr){

	int instr = statePtr->mem[statePtr->pc];
	int regA = statePtr->reg[REGA(instr)];
	int offset = convertNum(OFFSET(instr));

	statePtr->reg[REGB(instr)] = statePtr->mem[regA + offset];
	statePtr->pc++;///refer to add

	printState(statePtr);
	}


static int
convertNum(int num){

	/* convert a 16-bit number into a 32-bit Linux integer */

	if (num & (1 << 15) ){
		num -= (1 << 16);
	}
	return (num);
}

static void
sw(stateType *statePtr){

	int instr = statePtr->mem[statePtr->pc];
	int regA = statePtr->reg[REGA(instr)];
	int offset = convertNum(OFFSET(instr));

	statePtr->mem[regA + offset] = statePtr->reg[REGB(instr)];
	statePtr->pc++;///refer to add

	printState(statePtr);
	}

static void
beq(stateType *statePtr){

	int instr = statePtr->mem[statePtr->pc];
	int regA = statePtr->reg[REGA(instr)];
	int regB = statePtr->reg[REGB(instr)];
	int offset = convertNum(OFFSET(instr));

	if (regA == regB)
		statePtr->pc += (1 + offset);
	else
		statePtr->pc++;

	printState(statePtr);
	}

static void
jalr(stateType *statePtr){

	int instr = statePtr->mem[statePtr->pc];
	statePtr->reg[REGB(instr)] = statePtr->pc + 1;
	int regA = statePtr->reg[REGA(instr)];
	statePtr->pc = regA;

	printState(statePtr);
	}
	
