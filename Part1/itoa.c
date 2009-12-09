#include "apue.h"
#include "itoa.h"

void itoa(int num, char *string){

	int i, sign;
	if ((sign = num ) < 0)
		num = -num;

	i = 0;
	do {
	string[i++] = num % 10 + '0';
	}while((num /= 10) > 0);

	if(sign < 0)
		string[i++] = '-';
	string[i] = '\0';

	reverse(string);

}

static void reverse(char *s){

	int c, i, j;
	for(i = 0, j = strlen(s) - 1; i < j; i++,j--){
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}
