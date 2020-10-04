#include <sys/stat.h>
#include <fcntl.h>
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

short eoflag = 0, Kflag = 0, Nflag = 0, Aflag = 0, BPcount = 0,AEflag = 0,APPflag=0, Pcount = 0;

char * readW(){
	int i=0,size = 16, fl=0, c = getchar();
	char * str = (char*)calloc(size,1);
	while((c !=EOF) &&  (c!='\n')){
		if(Aflag){
			printf("incorrect input\n");
			while(((c = getchar()) !=EOF) &&  (c!='\n'));
			eoflag = c == EOF;
			Nflag = c == '\n';
			AEflag = 1;
			free(str);
			return NULL;
		}
		if(!Kflag && (c == ' ' || c=='\t' || c=='&' || c=='>' || c=='<' || c=='|')){
			if(c=='>'){
				if(!i){
					if((c=getchar()) == '>'){
						APPflag = -1;
						c=getchar();
					}else{
						ungetc(c,stdin);
					}
					str[0]='>';
					str[1]='\0';
					return str;
				}else{
					ungetc(c,stdin);
					return str;
				}
			}
			else if(c=='<'){
				if(!i){
					str[0]='<';
					str[1]='\0';
					return str;
				}else{
					ungetc(c,stdin);
					return str;
				}
			}
			else if(c=='|'){
				if(!i){
					Pcount +=1;
					str[0]='|';
					str[1]='\0';
					return str;
				}else{
					ungetc(c,stdin);
					return str;
				}
			}
			else if(c=='&'){
				Aflag = 1;
				if(!fl){
					free(str);
					str = NULL;
				}
				return str;
					
			}
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
	char ** rez = (char**)calloc(size, sizeof(char*));
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
	if(cmnd[0] == '\0'){
		return 1;
	}
	if(!strcmp(cmnd[0],"cd")){
		if(cmnd[1] == NULL){
			chdir(getenv("HOME"));
		}
		else if(chdir(cmnd[1]) || (cmnd[2]!=NULL)){
			printf("Error in changing directory\n");
			return -1;
		}
		return 1;
	}
	return 0;	
}

int ifEXIT(char** cmnd){
	if(!strcmp(cmnd[0],"exit")){
		return 1;
	}
	else{
		return 0;	
	}
}

char ** getLP(char ** argv){
	int i = 0;
	char**  rez = (char**) calloc(1, sizeof(char*));
	while(argv[i++][0] != '|'){
		rez[i-1] = argv[i-1];
		rez = (char**)realloc(rez, (i+1) * sizeof(char*));
	}
	rez[i-1] = NULL;
	return rez;
}

char ** getRP(char ** argv){
	int i = 0;
	char**  rez = (char**) calloc(1, sizeof(char*));
	while(argv[i++][0] != '|');
	int k = i;
	while(argv[i++]){
		rez[i-1-k] = argv[i-1];
		rez = (char**)realloc(rez, (i-k+1) * sizeof(char*));
	}
	rez[i-1-k] = NULL;
	return rez;
}

int command(char ** cmd,list l,char*fnameIN,char*fnameOUT){
	int pid;
	if(fnameIN || fnameOUT){//есть перенаправление

		int fd0=0,fd1=0;
		if(fnameIN){
			if((fd1=open(fnameIN,O_WRONLY | (APPflag & O_APPEND)))==-1){ 
				printf("error in open file\n");
				free(cmd);
				free(fnameIN);
				free(fnameOUT);
				listDel(l);
				return -1;
			}
		}
		if(fnameOUT){
			if((fd0=open(fnameOUT,O_RDONLY))==-1){ 
				printf("error in open file\n");
				free(cmd);
				free(fnameIN);
				free(fnameOUT);
				listDel(l);
				return -1;
			}
		}
		if(Aflag){
			printf("background procces mode\n");
		}
		pid = fork();
		if(pid == -1){//error in fork
			printf("error in creating proccess\n");
		}else if(!pid)//procces son
			{
				if(fd1){
					dup2(fd1,1);
					close(fd1);
					}
				if(fd0){
					dup2(fd0,0);
					close(fd0);
				}
				execvp(*cmd,cmd);
				printf("error in command\n");
				free(cmd);
				free(fnameIN);
				free(fnameOUT);
				listDel(l);
				exit(1);
			}else{//procces father
				if(Aflag == 0){
					while ((wait(NULL))!=pid);
				}
			}
		if(fd1) close (fd1);
		if(fd0) close (fd0);
		return pid;
	}else if(!Pcount){//все обычно

		if(Aflag){
			printf("background procces mode\n");
		}
		pid = fork();
		if(pid == -1){//error in fork
			printf("error in creating proccess\n");
		}else if(!pid)//procces son
			{
				execvp(*cmd,cmd);
				printf("error in command\n");
				free(cmd);
				free(fnameIN);
				free(fnameOUT);
				listDel(l);
				exit(1);
			}else{//procces father
				if(Aflag == 0){
					while ((wait(NULL))!=pid);
				}
			}
		return pid;

	}else{//конвеер
		int fd[2];
		pipe(fd);
		char **com1 = getLP(cmd);
		char **com2 = getRP(cmd);
		if ((pid = fork())== 0){
			dup2(fd[1], 1);
			close(fd[1]);
			close(fd[0]);
			execvp(*com1,com1);
			printf("error in command\n");
			free(cmd);
			free(com1);
			free(com2);
			listDel(l);
			exit(1); 
		}else{
			if ((pid = fork())== 0){
				dup2(fd[0], 0);
				close(fd[0]);
				close(fd[1]);
				execvp(*com2,com2);
				printf("error in command\n");
				free(cmd);
				listDel(l);
				free(com1);
				free(com2);
				exit(1); 
			}else{
				close(fd[0]);
				close(fd[1]);
				wait(NULL);
				wait(NULL);
			}
		}
		free(com1);
		free(com2);
		return pid;
	}
}

void terminateSONS(){
	int pid,s;	
	if((pid=waitpid(-1,&s,WNOHANG))>0)
	{
			printf("[%d] terminated\n", pid);
			BPcount--;
	}
}

char * findFileIn(list l){
	char * filename;
	list prev = NULL;
	while(l){
		if(!strcmp(">",l->elem))
			break;
		prev = l;
		l = l->next;
	}
	if(l == NULL)
		return NULL;
	if(l->next == NULL){
		printf("i need file\n");
		return NULL;
	}
	filename = strdup((l->next)->elem);
	listDel(l);
	prev->next = NULL;
	return filename;
}

char * findFileOut(list l){
	char * filename;
	list prev = NULL;
	while(l){
		if(!strcmp("<",l->elem))
			break;
		prev = l;
		l = l->next;
	}
	if(l == NULL)
		return NULL;
	if(l->next == NULL){
		printf("i need file\n");
		return NULL;
	}
	prev->next = (l->next)->next;
	filename = (l->next)->elem;
	free(l->next);
	free(l->elem);
	free(l);
	return filename;
}

void testarg(char ** a){
	int i = 0;
	while(a[i++])
		printf("%s\n",a[i-1]);
	
}

int main(){
	int pid;
	list l = NULL;
	char * s = NULL, ** cmd = NULL, * fnameIN = NULL, * fnameOUT = NULL;
	while(!eoflag){	
		s = readW();
		if(s)
			insert(&l,s);
		if(Kflag){
			errQuot(l,s);
			l = NULL;
		}
		else if(Nflag){
			fnameOUT = findFileOut(l);
			fnameIN = findFileIn(l);
			Nflag = 0;
			cmd = toMas(l);
			if(!AEflag){
				if(Pcount<=1 ){
					if(!ifCD(cmd)){
						if(!ifEXIT(cmd)){
							pid = command(cmd,l,fnameIN,fnameOUT);
							if (Aflag==1){
								BPcount++;
								printf("[%d] %d\n",BPcount,pid);
							}
						}else{
							eoflag = 1;
						}
					}
				}else{printf("too many pipes\n");}
			}
			Pcount = 0;
			AEflag = 0;
			Aflag = 0;
			APPflag = 0;
			free(cmd);
			cmd = NULL;
			listDel(l);
			l = NULL;
			free(fnameIN);
			fnameIN = NULL;
			free(fnameOUT);
			fnameOUT = NULL;
			terminateSONS();
		}
	}
	return 0;
}
