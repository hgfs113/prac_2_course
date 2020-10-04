#include <stdio.h>
# include <stdlib.h>
#include <string.h>

int main(){
	FILE *f1,*f2;
	char s[81];
	int i = 0;
	if((f1=fopen("aaa","r")) == NULL) exit(1);
	if((f2=fopen("bbb","w")) == NULL) exit(1);
	while(fgets(s,81,f1) != NULL && i<10){
	i++;
	fputs(s,f2);
	}
	fclose(f1);
	fclose(f2);
	return 0;
}
