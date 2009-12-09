#include "apue.h"
#include "parse.h"
#include "itoa.h"
#define MAXLAB 100 /*At most keep MAXLAB labels */
#define NUMISA 8   /*We only have 8 instructions here */
#define LABLEN 6
#define ISNLEN 5   /*For now, the longest name string 'halt','noop' or 'jalr' take 4 bytes and an extra '\0' char */

char labelist[MAXLAB][LABLEN + 1] = {{'\0'}};

///int Addr_Ref_Tab[MAXLAB] = {-1,};///We at most handle MAXLAB .fill directives

char ISA[NUMISA][ISNLEN] = {"add","nand","lw","sw","beq","jalr","halt","noop",};/*index of this array is each instruction's machine code, and .fill is just a directive, we treat it in another way */

int bu_labl(char *line, int addr){

	int i;
	if(isalpha(line[0])){
		for(i = 0;!isspace(line[i]) && line[i] != '\0'; i++)
			labelist[addr][i] = line[i];
		labelist[addr][i+1] = '\0';
		return 1;
		}
	else if(line[0] != ' ' && line[0] != '\n')
		err_sys("Label must be started by letter");
	else
		return 1;

}
		
void printLabelist(){

int i,j;
	for(i = 0;i < MAXLAB; i++){
	if(labelist[i][0] != '\0')
		printf("label: %s \t addr: %d \n",labelist[i], i);
}
}	
	
static int addrOflabel(char *line){
	int i;
	for(i = 0; i < MAXLAB; i++)
		if(strcmp(line, labelist[i]) == 0)
			return i;

	return -1;
}
			

int parse(char *line, int *macode, int addr2){
	char instr[ISNLEN + 1];///still need handle .fill 
	int i,j = 0;
	for(i = 0; i < ISNLEN; i++)
		instr[i] = line[8 + i];/*instruction start by index 8 in our format*/

	while(isalpha(instr[j]) || instr[j] == '.')
		j++;

	instr[j] = '\0';/*make it a real c-style string ,easy to compare */

	int k;
	int knowni = 0;///use this bool value to detect unknown instruction
	for(k = 0; k < NUMISA; k++){
		if(strcmp(ISA[k],instr) == 0){
			switch(k){
				case 0 : case 1 :
					knowni = 1;
					Rtypei(line, k, macode);
					break;
				case 2 : case 3 : case 4 :
					knowni = 1;
					Itypei(line, k, macode, addr2);
					break;
				case 5 :
					knowni = 1;
					Jtypei(line, k, macode);
					break;
				case 6 : case 7 :
					knowni = 1;
					Otypei(line, k, macode);
					break;
			}///switch end 
		}///if end

		else if(strcmp(".fill", instr) == 0){
				knowni = 1;
				hfill(line, macode);}
	}///for end
	
	if ( knowni == 0)
		return -1;
	else	
		return 1;

}


static void Rtypei(char *line, int k, int *macode){

	int machCode = 0;///Clear this 32 bits integer whole zero

	int regA = line[REGAF] - '0';
	int regB = line[REGBF] - '0';
	int destReg = line[DESTREGF] - '0';

	machCode |= OPCODE(k)|REGA(regA)|REGB(regB)|DESTR(destReg);

	///itoa(machCode, *macode, 10);///TRY this offical conversion function itoa
	*macode = machCode;
}

static void Itypei(char *line, int k, int *macode, int addr2){

	int machCode = 0;///like Rtypei
	int i;
	int addr;
	///int tmp;
	unsigned short offset;
	char labelname[LABLEN + 1] = {'\0',};
	if (isalpha(line[LABELF])){///it's a label
		for(i = 0; !isspace(line[LABELF + i]) && line[LABELF + i] != '\0'; i++)
			labelname[i] = line[LABELF + i];
		labelname[i + 1] = '\0';

	if ( (addr = addrOflabel(labelname)) >= 0){
		///tmp = addr - addr2 - 1;
		if(addr >= addr2)
			offset = (unsigned short)(addr - addr2);
		else
			offset = (unsigned short)((~(-(addr - addr2 -1)))+1);
			}	
	else
		err_sys("Label undefined");
		}

	else{///it's a numberic
		int tmpnum;
		char charnum[DECLEN];
		for(i = 0; !isspace(line[OFFSETF + i]) && line[OFFSETF + i] != '\0'; i++)
			charnum[i] = line[OFFSETF + i];
		charnum[i + 1] = '\0';

		tmpnum = atoi(charnum);
///		printf("tmpnum = %d\n", tmpnum);
		if(tmpnum >= 0)
			offset = (unsigned short)tmpnum;
		else
			offset = (unsigned short) ((~(-tmpnum))+1) ;
		}///end else


	int regA = line[REGAF] - '0';
	int regB = line[REGBF] - '0';
	machCode |= OPCODE(k)|REGA(regA)|REGB(regB)|offset;
	///itoa(machCode, *macode, 10);
	*macode = machCode;
}
					

		
static void Jtypei(char *line, int k, int *macode){

	int machCode = 0;
	int regA = line[REGAF] - '0';
	int regB = line[REGBF] - '0';
	machCode |= OPCODE(k)|REGA(regA)|REGB(regB);

	///itoa(machCode, *macode, 10);
	*macode = machCode;
}

static void Otypei(char *line, int k, int *macode){

	int machCode = 0;
	machCode |= OPCODE(k);
	///itoa(machCode, *macode, 10);
	*macode = machCode;
}

static void hfill(char *line, int *macode){


	int take, i;
	char label[LABLEN + 1] = {'\0',};
	static char num[DECLEN];
	if(isalpha(line[REGAF])){
	for(i = 0; !isspace(line[REGAF + i]) && line[REGAF + i] != '\0'; i++)
		label[i] = line[REGAF + i];

	label[i + 1] = '\0';
	take = addrOflabel(label);
	///itoa(take, *macode, 10);
	*macode = take;
		}
	else {

	for(i = 0; !isspace(line[REGAF + i]) && line[REGAF + 1] != '\0'; i++)
		num[i] = line[REGAF + i];
		num[i + 1] = '\0';
	*macode = atoi(num);
	}
}
	
	
