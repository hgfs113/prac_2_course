#include <stdio.h>
#include <stdlib.h>
#include "m.h"


//INPUT MATRIX M ROWS N COLUMN
void MInput(m * a,unsigned M, unsigned N){
    unsigned i,j;
    *a = (m)malloc(M * sizeof(double*));
    for (i = 0;i<M;i++){
        (*a)[i] = (double*)malloc(N * sizeof (double));
        for (j = 0;j < N;j++)
            scanf("%lf",&((*a)[i][j]));
    }
}
//OUTPUT MATRIX M ROWS N COLUMN
void MOutput(m a,unsigned M, unsigned N){
    unsigned i,j;
    for (i = 0;i<M;i++){
        for (j = 0;j < N;j++)
            printf("%5.3lf\t",(a[i][j]));
        printf("\n");
    }
}
//DELET MATRIX M COLUMNS
void MDel(m a,unsigned M){
    unsigned i;
    for (i = 0;i<M;i++)
        free(a[i]);
    free(a);
}
//SUMM MATRIX A = A + B ROWS N COLUMN
void MSumm(m A,m B, unsigned M, unsigned N){
    for (int i = 0 ; i<M;i++) 
        for (int j = 0 ; j<N;j++) 
            A[i][j] += B[i][j];
}
//MULLTIPLE MATRIX A = ALPHA * A M ROWS N COLUMN
void MMulI(double alpha,m A, unsigned M, unsigned N){
    for (int i = 0 ; i<M;i++) 
        for (int j = 0 ; j<N;j++) 
            A[i][j] *= alpha;
}
//MULTIPLE A = A*B !!!!!!ONLY FOR SQUARE MATRIX
void MMul(m* A, m B, unsigned N){
    unsigned j,i,k;
    //init c
    m C = (m)malloc(N * sizeof(double*));
    for(j = 0; j<N; j++)
        C[j] = (double*)malloc(N * sizeof(double));
    // C = A*B
    for(i=0; i<N; i++)
        for(j=0; j<N; j++){
            C[i][j] = 0;
            for(k=0; k<N; k++)
               C[i][j] += (*A)[i][k] * B[k][j];
        }
    // free A
    MDel(*A,N);
    // A = C
    *A = C;
}

void GetM(m * a, m b,int N,int i,int j){
    int ki, kj, di, dj;
    di = 0;
    for (ki = 0; ki < (N - 1); ki++) {
        if (ki == i)
            di = 1;
        dj = 0;
        for (kj = 0; kj< N - 1; kj++) {
            if (kj == j)
                dj = 1;
            (*a)[ki][kj] = b[ki + di][kj + dj];
        }
    }
}

double MDet(m A, unsigned N){
    double det = 0;
    // init tmp
    unsigned i,l;
    int k = 1;
    //rec det
    if(N == 1){
        return **A;
    }else if (N == 2) {
        return (A[0][0] * A[1][1] - (A[1][0] * A[0][1]));
    }
    else {
	    m tmp = (m)malloc((N-1) * sizeof(double*));
	    for (l = 0; l < N-1; l++)
		tmp[l] = (double*)malloc((N-1) * sizeof (double));
            for (i = 0; i<N; i++) {
                GetM(&tmp, A ,(int)N, (int)i, 0);
                det += k * A[i][0] * MDet(tmp,N - 1);
                k = -k;
            }
	    MDel(tmp,N-1);
        }
    return det;
}
