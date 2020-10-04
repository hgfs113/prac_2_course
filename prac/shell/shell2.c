#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
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

char** toMas(list l){
	int count = 0, size = 2;
	char ** rez = (char**)malloc(size * sizeof(char*));
	rez[1] = NULL;
	while(l){
		count++;
		if(count >= size){
			size*=2;
			rez = (char**)realloc(rez, size * sizeof(char*));
		}
		rez[count-1] = l->elem;
		rez[count] = NULL;
		l=l->next;
	}
	return rez;
}

int ifCD(char** cmnd){
	if(!strcmp(cmnd[0],"cd")){
		if(cmnd[1] == NULL){
			chdir("/home");
		}
		else if(chdir(cmnd[1]) || (cmnd[2]!=NULL)){
			printf("Error in changing directory\n");
			return -1;
		}
		return 1;
	}
	else return 0;	
}

int ifEXIT(char** cmnd){
	if(!strcmp(cmnd[0],"exit")){
		return 1;
	}
	else{
		return 0;	
	}
}

void command(char ** cmd,list l){
	int pid;
	pid = fork();
	if(pid == -1){//error in fork
		printf("error in creating proccess\n");
	}else if(!pid)//procces son
		{
			execvp(*cmd,cmd);
			printf("error in command\n");
			free(cmd);
			listDel(l);
			exit(1);
		}else{//procces father
			while ((wait(NULL))!=pid);
		}
}

int main(){
	list l = NULL;
	char * s = NULL;
	char ** cmd = NULL;
	while(!eoflag){	
		s = readW();
		if(s)
			insert(&l,s);
		if(Kflag){
			errQuot(l,s);
			l = NULL;
		}
		else if(Nflag){
			Nflag = 0;
			cmd = toMas(l);
			if(!ifCD(cmd)){
				if(!ifEXIT(cmd)){
					command(cmd,l);
				}else{
					eoflag = 1;
				}
			}
			free(cmd);
			cmd = NULL;
			listDel(l);
			l = NULL;
		}
	}
	return 0;
}
