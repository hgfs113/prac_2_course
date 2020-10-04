#include <stdio.h>
# include <stdlib.h>
#include <string.h>

int main(int argC,char *argV[]){
    if(argC < 4)
	exit(1);
    FILE *f1,*f2;
    if((f1=fopen(argV[1],"r")) == NULL)
	 exit(1);
    if((f2=fopen(argV[2],"w")) == NULL)
	exit(1);
    int n = strlen(argV[3]);
    char *s = argV[3];
    s[n] ='\n';
    s[n+1] = 0;
    char s2[n];
    printf("lol\n");
    while(fgets(s2,n+2,f1) != NULL){
	if(strcmp(s,s2))
	    fputs(s2,f2);
    }
    fclose(f1);
    fclose(f2);
    return 0;
}
