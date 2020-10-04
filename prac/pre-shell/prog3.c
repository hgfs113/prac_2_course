#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct node * tree;
struct node{
    int key,lvl;
    tree left,right;
};

void insert(tree * t, int k,int l){
    if(*t != NULL){
	l++;
        if((*t)->key > k)
            insert(&((*t)->left),k,l);
        else if((*t)->key < k)
            insert(&((*t)->right),k,l);
    }
    else{
	*t=(tree)malloc(sizeof(tree));
	(*t)->left = NULL;
	(*t)->right = NULL;
	(*t)->key = k;
	(*t)->lvl = l;
    }
}

int maxlvl(tree t){
	int max=0,m1,m2;
	if(t){
	m1=maxlvl(t->left);
	m2=maxlvl(t->right);
	if(m1 > m2)
		 max = 1 + m1;
	else
	         max = 1 + m2;
	}
	return max;
}
void treePrint(tree t){
    if(t){
	treePrint(t->left);
	printf("%d\t of lvl %d\n",t->key,t->lvl);
	treePrint(t->right);
    }
}

void treePrintLv(tree t,int l)
{
	if(t){
		if(t->lvl == l){
			printf("%d ",t->key);
		}
		else{
			treePrintLv(t->left,l);
			treePrintLv(t->right,l);
		}
	}
}
void DelTree(tree t){
	if(t){
		DelTree(t->left);
		DelTree(t->right);
		free(t);
	}
}

int main(){
    int i;
    tree a = NULL;
    scanf("%d",&i);
    while(i != 0){
	insert(&a,i,0);
	scanf("%d",&i);
    }
//    treePrint(a);
//    DelTree(a);
    int m = maxlvl(a);
  //  printf("\n%d\n",m);
    i = 0;
    while(i++ <m){
	printf("\n");
	treePrintLv(a,i-1);
    }
    DelTree(a);
    printf("\n");
    return 0;
}
