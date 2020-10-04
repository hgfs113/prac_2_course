#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define MAXPROC 40
#define lbit 0x80000000

typedef struct node* list;
struct node{
	char * elem;
	list next;	
};
char fileIN[20],fileOUT[20];
int pids[MAXPROC], rez;
list ll = NULL;
char ** tmpV = NULL;
short eoflag = 0, Kflag = 0, Nflag = 0, Aflag = 0, BPcount = 0,AEflag = 0,APPflag=0, Pcount = 0, DOflag = 0,first = 0, Pflag  = 0;
int fd0=0,fd1=0;
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
		if(!Kflag && (c == ' ' || c=='\t' || c=='&' || c=='>' || c=='<' || c=='|' || c==';'|| c=='('|| c==')')){
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
			else if(c==';'){
				if(!i){
					str[0]=';';
					str[1]='\0';
					return str;
				}else{
					ungetc(c,stdin);
					return str;
				}
			}
			else if(c=='('){
				if(!i){
					str[0]='(';
					str[1]='\0';
					return str;
				}else{
					ungetc(c,stdin);
					return str;
				}
			}
			else if(c==')'){
				if(!i){
					str[0]=')';
					str[1]='\0';
					return str;
				}else{
					ungetc(c,stdin);
					return str;
				}
			}
			else if(c=='|'){
				if(!i){
			
					if((c=getchar()) == '|'){
						str[0]='|';
						str[1]='|';
						str[2]='\0';
						return str;
					}else{
						ungetc(c,stdin);
						Pcount +=1;
						str[0]='|';
						str[1]='\0';
						return str;
					}
				}else{
					ungetc(c,stdin);
					return str;
				}
			}
			else if(c=='&'){
				if( (c=getchar()) == '&' ){
						str[0]='&';
						str[1]='&';
						str[2]='\0';\
						return str;
				}
				else{
					ungetc(c,stdin);
					Aflag = 1;
					if(!fl){
						free(str);
						str = NULL;
					}
					return str;
				}
					
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
		(*l)->elem = strdup(s);
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

list toList(char** m){
	int count = 0;
	list rez = NULL;
	while(m[count]){
		count++;
		insert(&rez,m[count-1]);
	}
	return rez;
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

int ifCD(list l){
	if(l == NULL){
		return 1;
	}
	if(!strcmp(l->elem,"cd")){
		if(l->next == NULL){
			chdir(getenv("HOME"));
		}
		else if(chdir((l->next)->elem) || ((l->next)->next !=NULL)){
			printf("Error in changing directory\n");
			return -1;
		}
		return 1;
	}
	return 0;	
}

int ifEXIT(list l){
	if(!strcmp(l->elem,"exit")){
		return 1;
	}
	else{
		return 0;	
	}
}

char * findFile(list l,char *c){
	char * filename;
	char* t=NULL;
	list prev = NULL;
	while(l){
		if(!strcmp(l->elem, c))
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
	prev->next = (l->next)->next;
	free((l->next)->elem);
	free(l->next);
	free(l->elem);
	free(l);
	t = findFile(prev,c);
	if(t){
		free(filename);
		filename = t;
	}
	return filename;
}

char ** getLP(char ** argv, char * spec){
	int i = 0;
	char**  rez = (char**) calloc(1, sizeof(char*));
	while(strcmp(argv[i++], spec)){
		rez[i-1] = argv[i-1];
		rez = (char**)realloc(rez, (i+1) * sizeof(char*));
	}
	rez[i-1] = NULL;
	return rez;
}

char ** getRP(char ** argv, char * spec){
	int i = 0;
	char**  rez = (char**) calloc(1, sizeof(char*));
	while(strcmp(argv[i++], spec));
	int k = i;
	while(argv[i++]){
		rez[i-1-k] = argv[i-1];
		rez = (char**)realloc(rez, (i-k+1) * sizeof(char*));
	}
	rez[i-1-k] = NULL;
	return rez;
}

void inb(int pid){
	int i = 0;
	while(pids[i++]);
	pids[i-1] = pid;
}

void testP(){
	int i = 0;
	while(++i < MAXPROC)
		printf("%d\n",pids[i-1]);
}
void testV(char ** v){
	int i = 0;
	while(v[i++])
		printf("%s\n",v[i-1]);
	printf("NULL\n");
}
char * findSpecs(list argv){
	while(argv){
		if((argv->elem)[0] == ';' || (argv->elem)[0] == '&' || !strcmp(argv->elem,"||"))
			return argv->elem;
		argv = argv->next;
	}
	return NULL;
}

char ** getParL(char ** com){
	int i = 1,k=1;
	char**  rez = (char**) malloc(sizeof(char*));
	while(i){
		rez[k-1] = com[k];
		rez = (char**)realloc(rez, (k+1) * sizeof(char*));
		if(com[k][0] == '(') i++;
		else if(com[k][0] == ')') i--;
		k++;		
	}
	rez[k-2] = NULL;
	return rez;
	
}
char ** getParR(char ** com){
	int i = 1,k=1;
	while(i){
		if(com[k][0] == '(') i++;
		else if(com[k][0] == ')') i--;
		k++;		
	}
	if(com[k-1] == NULL)
		return NULL;
	char**  rez = (char**) malloc(1 * sizeof(char*));
	i = 0;
	while(com[k-1 +i] != NULL){
		rez = (char**)realloc(rez, (i+1) * sizeof(char*));
		rez[i] = com[k+i];
		i++;
	}
	if(rez[0] == NULL){
		free(rez);
		return NULL;
	}
	return rez;
	
}

int parF(list l){
	while(l){
		if(!strcmp(l->elem, ")")){
			if(l->next)
				if(!strcmp((l->next)->elem,">"))
					break;
		}
		l= l->next;
	}
	if(l) return 1;
	return 0;
}

int command(list l){
	if(l)
	if(parF(l) && !Pflag && !strcmp(l->elem,"(")){
		char * fnameOUT = findFile(l,">");
		char * fnameIN = findFile(l,"<");
		if(fnameOUT){
			strcpy(fileOUT,fnameOUT);
			if((fd1=open(fnameOUT,O_WRONLY | (APPflag & O_APPEND) | O_CREAT,0777))==-1){ 
				printf("error in open file out %s\n",fnameOUT);
				free(fnameIN);
				free(fnameOUT);
				fnameOUT = NULL;
				fnameIN = NULL;
				return -1;
			}
		}
		if(fnameIN){
			strcpy(fileIN,fnameIN);
			if((fd0=open(fnameIN,O_RDONLY,0777))==-1){ 
				printf("error in open file in %s\n",fnameIN);
				free(fnameIN);
				free(fnameOUT);
				fnameOUT = NULL;
				fnameIN = NULL;
				return -1;
			}
		}
		free(fnameIN);
		free(fnameOUT);
		fnameOUT = NULL;
		fnameIN = NULL;
		Pflag = 1;
	}

	int pid, s;
	first++;
	char ** cmd = NULL;
	char * spec = findSpecs(l);
	if(spec){
		int i=0;
		cmd = toMas(l);
		char spes[3];
		char **com1 = NULL;
		char **com2 = NULL;
		if(!strcmp(spec,cmd[0])){
			com2 = getRP(cmd,spec);
		}
		else if(cmd[0][0] != '('){
			com1 = getLP(cmd,spec);
			com2 = getRP(cmd,spec);
		}
		else{
			com1 = getParL(cmd);
			com2 = getParR(cmd);
			
		}
		tmpV=com2;
		free(cmd);
		list tmpL = NULL;
		do{
			if(i){
				if(com2[0][0]=='('){
					com1 = getParL(com2);
					tmpV = getParR(com2);
					free(com2);
					com2 = tmpV;
				}
				else{
					com1 = getLP(com2,spec);
					tmpV = getRP(com2, spec);
					free(com2);
					com2 = tmpV;
				}
			}
			strcpy(spes, spec);
			listDel(tmpL);
			i++;
			if(com1){
				if(com1[0]){
					tmpL = toList(com1);
					free(com1);
					com1 = NULL;
					if(!DOflag)
						rez = command(tmpL);
					else
						rez = 0;
					listDel(tmpL);
				}
				else{
					free(com1);
				}
			}
			if( ((spes[0]=='|') && rez) || ((spes[0]=='&') && !rez) || (spes[0] == ';'))
				DOflag = 0;
			else 
				DOflag = -1;
			if(com2 != NULL)
				tmpL = toList(com2);
			else tmpL = NULL;
		}while((spec = findSpecs(tmpL)) != NULL);
		free(com2);
		tmpV = NULL;
		if(!DOflag && tmpL !=NULL)
			rez = command(tmpL);
		else
			rez = 0;	
		listDel(tmpL);
		return rez;
	}
	else if(DOflag){
		DOflag = 0;
		return 0;
	}
	if(!Pcount){	//usl
		if(!Pflag){
			char * fnameOUT = findFile(l,">");
			char * fnameIN = findFile(l,"<");
			if(fnameOUT){
				if((fd1=open(fnameOUT,O_WRONLY | (APPflag & O_APPEND) | O_CREAT,0777))==-1){ 
					printf("error in open file out %s\n",fnameOUT);
					free(fnameIN);
					free(fnameOUT);
					fnameOUT = NULL;
					fnameIN = NULL;
					return -1;
				}
			}
			if(fnameIN){
				if((fd0=open(fnameIN,O_RDONLY,0777))==-1){ 
					printf("error in open file in %s\n",fnameIN);
					free(fnameIN);
					free(fnameOUT);
					fnameOUT = NULL;
					fnameIN = NULL;
					return -1;
				}
			}
			free(fnameIN);
			free(fnameOUT);
			fnameOUT = NULL;
			fnameIN = NULL;
		}
		else{
			if(fd1){
				if((fd1=open(fileOUT,O_WRONLY |  O_APPEND ,0777))==-1){ 
					printf("error in open file out %s\n",fileOUT);
					return -1;
				}
			}
			if(fd0){
				if((fd0=open(fileIN,O_RDONLY,0777))==-1){ 
					printf("error in open file in %s\n",fileIN);
					return -1;
				}
			}

		}
		cmd = toMas(l);
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
			//	if(l != ll)
			//		listDel(ll);
				listDel(l);
				free(tmpV);
				exit(2);
			}else{//procces father
				if(Aflag == 0){
					waitpid(pid,&s,0);
					if(fd1) close (fd1);
					if(fd0) close (fd0);
					free(cmd);
					if(!WIFEXITED(s))
						return	-1;
					else
						return WEXITSTATUS(s);
				}else{
				//	printf("BG mode pid = %d\n",pid);
					inb(pid);
				}
			}
		free(cmd);	
		return -1;
	}else{//конвеер
		int fd[2];
		int fread = dup(0);
		int pid=0, i=0;
		list tmpL = NULL;
		cmd = toMas(l);
		char ** comL = getLP(cmd,"|");
		char ** comR = getRP(cmd,"|");
		free(cmd);
		char ** tmp;
		int P = Pcount+1;
		Pcount = 0;
		while (i++ < P) {
			if(i-1){//в comL кладем очередную команду
				if(P - i){
					comL = getLP(comR,"|");
					tmp = getRP(comR,"|");
					free(comR);
					comR = tmp;
				}else{
					comL = comR;
				}				
			}
			if (i == P)
				fd[1] = dup(1);
			else
				pipe(fd);
			if (!(fork())) {
				dup2(fread, 0);
				close(fread);
				dup2(fd[1], 1);
				close(fd[1]);
				close(fd[0]);
				if(P - i){
					free(comR);
					comR = NULL;
				}
				tmpL = toList(comL);
				free(comL);
				comL = NULL;
				listDel(l);
				pid = command(tmpL);
				listDel(tmpL);
				exit(0);
			} else {
				close(fd[1]);
				dup2(fd[0], fread);
				close(fd[0]);
			}
			free(comL);
			comL = NULL;
		}
		close(fread);
		while(i--)
			wait(NULL);
		return pid;
	}
}



void phandler(int sig){
	int pid1,i,s;
	for(i=0;i<MAXPROC;i++){	
		if(pids[i]){
			if((pid1=waitpid(pids[i],&s,WNOHANG))>0){
				if(WIFEXITED(s)){
					BPcount--;
					pids[i] |= lbit;
				}
			}
		}
	}
}

void checkterms(){
	int i;
	for(i=0; i<MAXPROC; i++){
		if(lbit & pids[i]){
			pids[i] &= ~lbit;
			printf("[%d] terminated\n", pids[i]);
			pids[i] = 0;
		}
	}	
}


int main(){
	signal(SIGCHLD, phandler);
	list l = NULL;
	char * s = NULL;
	while(!eoflag){	
		s = readW();
		if(s){
			insert(&l,s);
			free(s);
		}
		if(Kflag){
			errQuot(l,s);
			l = NULL;
		}
		else if(Nflag){
			Nflag = 0;
			if(!AEflag){
				if(!ifCD(l)){
					if(!ifEXIT(l)){
						if(Aflag && (BPcount == MAXPROC)){
							printf("2 many proccess\n");
						}else{
							ll = l;	
							 command(l);
							//printf("pid %d\n",pid);
						}
						if (Aflag==1){
							BPcount++;
							//printf("[%d] %d\n",BPcount,pid);
						}
					}else{
						eoflag = 1;
					}
				}
				
			}
			AEflag = 0; Aflag = 0; APPflag = 0;DOflag = 0;first = 0;Pflag = 0;fd0=0;fd1=0;
			listDel(l);
			l = NULL;
			checkterms();
		}
	}
	return 0;
}
