#include <stdio.h>
#include <stdlib.h>

typedef struct node* list;
struct node{
	char * elem;
	list next;	
};

short eoflag = 0, Kflag = 0, Nflag = 0;

char * readW(){
	int i=0,size = 16, fl=0, c = getchar();
	char * str = (char*)calloc(size,1);
	while((c !=EOF) &&  (c!='\n')){
		if(!Kflag && (c == ' ' || c=='\t')){
			if(!fl){
				free(str);
				return NULL;
			}
		
			else{
				str[i] = '\0';
				eoflag = c == EOF;
				Nflag = c == '\n';
				return str;
			}
		}
		if(c == '"'){
			Kflag = !Kflag;
		}else{
			if(++i >= size){
				size *=2;
				str = (char*)realloc(str,size);
			}
			str[i-1] = c;
		}
		fl = -1;
		c = getchar();
	}
	eoflag = c == EOF;
	Nflag = c == '\n';
	if(!fl){
		free(str);
		return NULL;
	}
	str[i] = '\0';
	return str;
}

void listPrint(list l){
	if(l){
		printf("%s\n",l->elem);
		listPrint(l->next);
	}	
}

void insert(list * l, char * s){
	if(*l){
		insert(&((*l)->next),s);		
	}else{
		*l = (list)malloc(sizeof(struct node));
		(*l)->elem = s;
		(*l)->next = NULL;
	}
}

void listDel(list l){
	if(l){
		listDel(l->next);
		free(l->elem);
		free(l);
	}
}

void errQuot(list l,char * s){
	listPrint(l);
	listDel(l);
	Kflag = 0;
	printf("ERROR: QUOTES\n");
}

int main(){
	list l = NULL;
	char * s = NULL;
	while(!eoflag){	
		s = readW();
		if(s)
			insert(&l,s);
		if(Kflag){
			errQuot(l,s);
			l = NULL;
		}
		else if(Nflag || eoflag){
			Nflag = 0;
			listPrint(l);
			listDel(l);
			l = NULL;
		}
		if(Kflag) break;
	}
	return 0;
}
