#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 258

typedef struct node * tree;

struct node{
    int key;
    tree left,right;
    char *str;
};


void insert(tree * t, int k,char s[]){
    if(*t != NULL){
        if((*t)->key > k)
            insert(&((*t)->left),k, s);
        else if((*t)->key < k)
            insert(&((*t)->right),k,s);
	else if(strcmp((*t)->str ,s)>0)
            insert(&((*t)->left),k, s);
	else
            insert(&((*t)->right),k,s);	    
    }
    else{
	*t=(tree)malloc(sizeof(tree));
	(*t)->left = NULL;
	(*t)->right = NULL;
	(*t)->key = k;
	(*t)->str =  (char*)malloc(k+1);
	(*t)->str = s;
    }
}

void treePrint(tree t,FILE * f){
    if(t!=NULL){
	treePrint(t->left,f);
	fputs(t->str,f);
	treePrint(t->right,f);
    }   
}

void DelTree(tree t){
	if(t){
		DelTree(t->left);
		DelTree(t->right);
		free(t->str);
		free(t);
	}
}

int main(int argC,char * argV[]){

    if(argC < 2){
        printf("Too little arguments\n");
	exit(1);
    }
    FILE *f;
    if((f=fopen(argV[1],"r"))==NULL){
        printf("Cannot open file\n");
	exit(1);
    }
    tree T = NULL;
    char tmps[N];
    char * is;
    while(fgets(tmps,N,f)){ 
	is = strdup(tmps);
	insert(&T,strlen(is),is);
    }
    fclose(f);

    if((f=fopen(argV[1],"w"))==NULL){
        printf("Cannot open file\n");
	exit(1);
    }
    treePrint(T,f);
    DelTree(T);
    fclose(f);
    return 0;
}
