#include <stdlib.h>
#include <stdio.h>
#include "m.h"

int main()
{
	int i,j,k=10;
	m a = (m)malloc(k* sizeof(double*));
  	for (i = 0;i<k;i++)
        	a[i] = (double*)malloc(k * sizeof (double));
	for(i=0;i<k;i++)
		for(j=0;j<k;j++)
			a[i][j] = i+j;
	MOutput(a,k,k);
	printf("%5.3lf",MDet(a,k));
	MDel(a,k);
	return 0;
}
