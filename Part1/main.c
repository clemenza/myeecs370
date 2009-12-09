#include<stdio.h>
#include<fcntl.h>
#include "apue.h"
#include "parse.h"
#include "itoa.h"

#define ONELINE 1024
int 
main(int argc, char *argv[]){

FILE *filer1, *filer2,*filew;
if ((filer1 = fopen(argv[1], "r")) < 0)
	err_sys("Open to scan error");

if ((filew = fopen(argv[2], "w")) < 0)
	err_sys("Write error");

char line[ONELINE] = {'\0',};
int addr = 0; /*phy addr for each instruction */
while ( fgets(line, sizeof line, filer1) != NULL){/*In this first pass we only build a label list, such that we can know each address of label.*/
	
	if(bu_labl(line, addr) < 0)
		err_sys("Build labels list error");

	addr++; //read one line, addr increment
}

///printLabelist();
	fclose(filer1);
if ((filer2 = fopen(argv[1], "r")) < 0)
	err_sys("Open to translate error");
char macodec[DECLEN];/*we're simulating a 32 bits machine, such that we only need 11 chars to hold decimal machine code */
int macode = 0;
int addr2 = 0;
while ( fgets(line, sizeof line, filer2) != NULL){/*This is the second pass, we translate assembler into machine code here ,line by line*/
	if(line[0] == '\n')
		continue;///skip newline
	if (parse(line, &macode, addr2) < 0)
		err_sys("Parse instruction error");
	addr2++;
	itoa(macode, macodec);
	if (fputs(macodec, filew) < 0)
		err_sys("Write file error");
	if (fputc('\n', filew) < 0)
		err_sys("Write newline error");
	fflush(filew);
}

	fclose(filer2);
	fclose(filew);	

exit(0);
}


