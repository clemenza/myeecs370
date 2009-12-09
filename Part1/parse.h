#include "apue.h"


/*define some opcode and reg position in 32bits machine code */

#define DECLEN 		11///In this 32bits machine,largest integer only takes 10 char

#define OPCODE(X) (X << 22)
#define REGA(R)   (R << 19)
#define REGB(R)   (R << 16)
#define DESTR(R)  (R)

/* define as file format, which field in which position in one line */

#define OPCODEF		8
#define REGAF		16
#define REGBF		24
#define DESTREGF	32
#define LABELF		32
#define OFFSETF		32

#define DIRECTIVE	8
#define VALUE		16
struct label{
	char name[7];
	int addr;
};

int bu_labl(char *, int);/* fill a global list, which consis of label names and address of it , take char * and int as input, only extract each label and assciate it with its address to establish a list*/

int bu_addr_ref_tab(char *, int);/* used to fill values specified by .fill directive, so actually we need scan as file 3 times */

int parse(char *, int *, int);/* element function of whole program, parse instruction line by line, and translate it into machine code, take char * as input, output machine code to char ** */

static void Rtypei(char *, int, int *);
static void Itypei(char *, int, int *,int);
static void Jtypei(char *, int, int *);
static void Otypei(char *, int, int *);

static void hfill(char *, int *);

static int addOflabel(char *);

