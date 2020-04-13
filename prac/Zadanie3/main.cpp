#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <arpa/inet.h>
#include <ctime>

#define BACKLOG 5
#define BUFLEN 1024

char * itoa(int i){
	char * str =new char[11];
	sprintf(str,"%d",i);
	return str;
}

class Server{
	int serverSocket;
	struct sockaddr_in ServerAddress;
public:
	Server(int port = 8080){
		serverSocket = socket(AF_INET,SOCK_STREAM,0);
		memset(&ServerAddress, '0', sizeof(ServerAddress));
		ServerAddress.sin_family = AF_INET;
		ServerAddress.sin_port = htons(port);
		if(inet_pton(AF_INET, "127.0.0.1", &ServerAddress.sin_addr)<=0) {//
			printf("\n inet_pton error occured\n");
			exit(1);
		}
	}
	int get_sock()const{return serverSocket;}
	void sbind(){
		if(bind(serverSocket,(struct sockaddr*)&ServerAddress,sizeof(ServerAddress))<0){
			std::cerr<<"fatal error: cannot bind\n";
			exit(1);
		}
	}
	void slisten(){
		if(listen(serverSocket,BACKLOG)<0){
			std::cerr<<"fatal error: cannot listen\n";
			exit(1);
		}
	}
};

class Client{
	int clSocket;
	socklen_t ClAddrLen;
	struct sockaddr_in ClientAddress;
	int fcount(int fd)const{
		int size=0,len;
		char c[1024];
		while((len = read(fd,&c,1024))>0)size+=len;
		lseek(fd,0,0);
		return size;
	}
public:
	void cl_accept(int serverSocket){
		ClAddrLen = sizeof(ClientAddress);
		clSocket = accept( serverSocket,(struct sockaddr*) &ClientAddress, &ClAddrLen);
		if (clSocket < 0){
			// ошибка — если будет повторяться, то фатальна
			std::cout<<"error: losed socked\n";
		}
	}
	void cl_request(){
		int len;
		char buf[BUFLEN];
		int cont_type;
		if((len = recv(clSocket,&buf,BUFLEN,0))<0){
			shutdown(clSocket,1);
			close(clSocket);
			std::cout<<"error: reading socked\n";
			exit(2);
		}
//		printf("received: %s\n",buf);
		if(strncmp(buf,"GET /",5)){
		//BadRequest
			strcpy(buf,"HTTP/1.0 501 NotImplemented modelserver\nServer: Model HTTP Server/0.1\nConnection: keep-alive\nAllow: GET\nDate: ");
			time_t tm=time(NULL);
			strcat(buf,asctime(localtime(&tm)));
			strcat(buf,"\n\n");
			len = strlen(buf);
			send(clSocket,&buf,len,0);
			shutdown(clSocket,1);
			close(clSocket);
			return;
		}
		int i =5;
		while(buf[i] && (buf[i++]>' '));
		buf[i-1]=0;
		i=5;
		while(buf[i] && (buf[i++]!='.'));
		cont_type=2;
		if(buf[i]){
			if(!strncmp(buf+i,"html",4))cont_type = 0;
			else if(!strncmp(buf+i,"jpeg",4))cont_type = 1;
		}
		else cont_type = 0;
		int fd;
		if(strlen(buf) == 5) {
			fd = open("index.html",O_RDONLY);
		}
		else if((fd = open(buf+5,O_RDONLY))<0){
			fd = open("err404.html",O_RDONLY);
	//		int conlen = fcount(fd);
	//		std::cout<<conlen<<'\n';
			strcpy(buf,"HTTP/1.0 404 PageNotFound modelserver\nServer: Model HTTP Server/0.1\nAllow: GET\nConnection: keep-alive\nContent-length: 377\nDate: ");
			time_t tm=time(NULL);
			strcat(buf,asctime(localtime(&tm)));
			strcat(buf,"\n\n");
			len = strlen(buf);
			send(clSocket,&buf,len,0);
			while((len = read(fd,buf,BUFLEN))>0)
				send(clSocket,&buf,len,0);
			close(fd);
			shutdown(clSocket,1);
			close(clSocket);
			return;
		}
		strcpy(buf,"HTTP/1.0 200 OK modelserver\nServer: Model HTTP Server/0.1\nAllow: GET\nConnection: keep-alive\nDate: ");
		time_t tm=time(NULL);
		strcat(buf,asctime(localtime(&tm)));
		strcat(buf,"Content-length: ");
		char * cl = itoa(fcount(fd));
		strcat(buf,cl);
		delete [] cl;
		switch(cont_type){
			case 0: strcat(buf,"\nContent-type: text/html\n\n");break;
			case 1: strcat(buf,"\nContent-type: image/jpeg\n\n");break;
			case 2: strcat(buf,"\nContent-type: text/plain\n\n");break;
		}
		len = strlen(buf);
		send(clSocket,&buf,len,0);
		while((len = read(fd,buf,BUFLEN))>0)
			send(clSocket,&buf,len,0);
		close(fd);
		shutdown(clSocket,1);
		close(clSocket);
		return;
	}
};

int main(int argc, char ** argv){
	int port = 8080;
	if(argc > 1) port = atoi(argv[1]);
	Server s(port);
	s.sbind();
	s.slisten();
	for(;;){
		Client newCL;
		newCL.cl_accept(s.get_sock());
		newCL.cl_request();	
	}
	return 0;
}
