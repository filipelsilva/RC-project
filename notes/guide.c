//gethostname

//test.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
extern int errno;

int main(void)
{
	char buffer[128];

	if(gethostname(buffer,128)==-1)
		fprintf(stderr,"error: %s\n",strerror(errno));
	else printf("host name: %s\n",buffer);
	exit(0);
}

//getaddrinfo

// test.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

int main(void)
{
	struct addrinfo hints,*res,*p;
	int errcode;
	char buffer[INET_ADDRSTRLEN];
	struct in_addr *addr;

	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_INET;//IPv4
	hints.ai_socktype=SOCK_STREAM;
	hints.ai_flags=AI_CANONNAME;

	if((errcode=getaddrinfo("tejo.tecnico.ulisboa.pt",NULL,&hints,&res))!=0)
		fprintf(stderr,"error: getaddrinfo: %s\n",gai_strerror(errcode));
	else{
		printf("canonical hostname: %s\n",res->ai_canonname);
		for(p=res;p!=NULL;p=p->ai_next){
			addr=&((struct sockaddr_in *)p->ai_addr)->sin_addr;
			printf("internet address: %s (%08lX)\n",
					inet_ntop(p->ai_family,addr,buffer,sizeof buffer),
					(long unsigned int)ntohl(addr->s_addr));
		}
		freeaddrinfo(res);
	}
	exit(0);
}

//UDP, socket and sendto

//test.c
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

int main(void)
{
	struct addrinfo hints,*res;
	int fd,errcode;
	ssize_t n;
	fd=socket(AF_INET,SOCK_DGRAM,0);//UDP socket
	if(fd==-1)/*error*/exit(1);
	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_INET;//IPv4
	hints.ai_socktype=SOCK_DGRAM;//UDP socket

	errcode=getaddrinfo("tejo.tecnico.ulisboa.pt","58001",&hints,&res);
	if(errcode!=0)/*error*/exit(1);
	n=sendto(fd,"Hello!\n",7,0,res->ai_addr,res->ai_addrlen);
	if(n==-1)/*error*/exit(1);
	/*...*/
	freeaddrinfo(res);
	exit(0);
}

//UDP and recvfrom

//test.c
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	int fd;
	struct sockaddr_in addr;
	socklen_t addrlen;
	ssize_t n;
	char buffer[128];

	/*...*///see previous task code

	addrlen=sizeof(addr);
	n=recvfrom(fd,buffer,128,0,(struct sockaddr*)&addr,&addrlen);
	if(n==-1)/*error*/exit(1);
	write(1,"echo: ",6);//stdout
	write(1,buffer,n);
	close(fd);
	exit(0);
}

//getnameinfo

//test.c
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
/* ... */

int main(void)
{
	int fd;
	struct sockaddr_in addr;
	socklen_t addrlen;
	ssize_t n;
	char buffer[128];
	int errcode;
	char host[NI_MAXHOST],service[NI_MAXSERV];//consts in <netdb.h>
	/*...*/// see previous task code

	addrlen=sizeof(addr);
	n=recvfrom(fd,buffer,128,0,(struct sockaddr*)&addr,&addrlen);
	if(n==-1)/*error*/exit(1);
	/*...*/

	if((errcode=getnameinfo((struct sockaddr *)&addr,addrlen,host,sizeof host,service,sizeof service,0))!=0)
		fprintf(stderr,"error: getnameinfo: %s\n",gai_strerror(errcode));
	else
		printf("sent by [%s:%s]\n",host,service);

	exit(0);
}

//TCP, socket and connect

//test.c
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

int main(void)
{
	struct addrinfo hints,*res;
	int fd,n;
	fd=socket(AF_INET,SOCK_STREAM,0);//TCP socket
	if(fd==-1)exit(1);//error
	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_INET;//IPv4
	hints.ai_socktype=SOCK_STREAM;//TCP socket

	n=getaddrinfo("tejo.tecnico.ulisboa.pt","58001",&hints,&res);
	if(n!=0)/*error*/exit(1);
	n=connect(fd,res->ai_addr,res->ai_addrlen);
	if(n==-1)/*error*/exit(1);
	/*...*/
}

//TCP, write and read

//test.c
#include <unistd.h>
#include <string.h>
/* ... */
int main(void)
{
	int fd;
	ssize_t nbytes,nleft,nwritten,nread;
	char *ptr,buffer[128];
	/*...*///see previous task code
	ptr=strcpy(buffer,"Hello!\n");
	nbytes=7;

	nleft=nbytes;
	while(nleft>0){nwritten=write(fd,ptr,nleft);
		if(nwritten<=0)/*error*/exit(1);
		nleft-=nwritten;
		ptr+=nwritten;}
	nleft=nbytes; ptr=buffer;
	while(nleft>0){nread=read(fd,ptr,nleft);
		if(nread==-1)/*error*/exit(1);
		else if(nread==0)break;//closed by peer
		nleft-=nread;
		ptr+=nread;}
	nread=nbytes-nleft;
	close(fd);
	write(1,"echo: ",6);//stdout
	write(1,buffer,nread);
	exit(0);
}

// TCP and the SIGPIPE signal

//test.c
#include <signal.h>

/*...*/
int main(void)
{
	/*...*/
	struct sigaction act;

	memset(&act,0,sizeof act);
	act.sa_handler=SIG_IGN;

	if(sigaction(SIGPIPE,&act,NULL)==-1)/*error*/exit(1);
	/*...*/
}

//UDP server and bind

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

int main(void)
{
	struct addrinfo hints,*res;
	int fd,errcode;
	struct sockaddr_in addr;
	socklen_t addrlen;
	ssize_t n,nread;
	char buffer[128];

	if((fd=socket(AF_INET,SOCK_DGRAM,0))==-1)exit(1);//error

	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_INET;//IPv4
	hints.ai_socktype=SOCK_DGRAM;//UDP socket
	hints.ai_flags=AI_PASSIVE;
	if((errcode=getaddrinfo(NULL,"58001",&hints,&res))!=0)/*error*/exit(1);

	if(bind(fd,res->ai_addr,res->ai_addrlen)==-1)/*error*/exit(1);

	while(1){addrlen=sizeof(addr);
		nread=recvfrom(fd,buffer,128,0,(struct sockaddr*)&addr,&addrlen);
		if(nread==-1)/*error*/exit(1);
		n=sendto(fd,buffer,nread,0,(struct sockaddr*)&addr,addrlen);
		if(n==-1)/*error*/exit(1);
	}
	//freeaddrinfo(res);
	//close(fd);
	//exit(0);
}

//TCP server, bind, listen and accept

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	struct addrinfo hints,*res;
	int fd,newfd,errcode;
	ssize_t n,nw;
	struct sockaddr_in addr;
	socklen_t addrlen;
	char *ptr,buffer[128];

	if((fd=socket(AF_INET,SOCK_STREAM,0))==-1)exit(1);//error

	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_INET;//IPv4
	hints.ai_socktype=SOCK_STREAM;//TCP socket
	hints.ai_flags=AI_PASSIVE;
	if((errcode=getaddrinfo(NULL,"58001",&hints,&res))!=0)/*error*/exit(1);

	if(bind(fd,res->ai_addr,res->ai_addrlen)==-1)/*error*/exit(1);
	if(listen(fd,5)==-1)/*error*/exit(1);

	while(1){addrlen=sizeof(addr);
		if((newfd=accept(fd,(struct sockaddr*)&addr,&addrlen))==-1)
			/*error*/exit(1);
		while((n=read(newfd,buffer,128))!=0){if(n==-1)/*error*/exit(1);
			ptr=&buffer[0];
			while(n>0){if((nw=write(newfd,ptr,n))<=0)/*error*/exit(1);
				n-=nw; ptr+=nw;}
		}
		close(newfd);
	}
	//freeaddrinfo(res);close(fd);exit(0);
}

//select

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
/* ... */
#define max(A,B) ((A)>=(B)?(A):(B))

int main(void)
{
	int fd,newfd,afd=0;
	fd_set rfds;
	enum {idle,busy} state;
	int maxfd,counter;
	/*...*/
	/*fd=socket(…);bind(fd,…);listen(fd,…);*/
	state=idle;
	while(1){FD_ZERO(&rfds);
		FD_SET(fd,&rfds);maxfd=fd;
		if(state==busy){FD_SET(afd,&rfds);maxfd=max(maxfd,afd);}

		counter=select(maxfd+1,&rfds,
				(fd_set*)NULL,(fd_set*)NULL,(struct timeval *)NULL);
		if(counter<=0)/*error*/exit(1);

		if(FD_ISSET(fd,&rfds))
		{
			addrlen=sizeof(addr);
			if((newfd=accept(fd,(struct sockaddr*)&addr,&addrlen))==-1)/*error*/exit(1);
			switch(state)
			{
				case idle: afd=newfd;state=busy;break;
				case busy: /* ... *///write “busy\n” in newfd
						   close(newfd); break;
			}
		}
		if(FD_ISSET(afd,&rfds))
			{
				if((n=read(afd,buffer,128))!=0)
				{if(n==-1)/*error*/exit(1);
					/* ... *///write buffer in afd
				}
				else{close(afd);state=idle;}//connection closed by peer
			}
	}//while(1)
	/*close(fd);exit(0);*/
}

//fork

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
extern int errno;

int main(void)
{
	struct sigaction act;
	struct addrinfo hints,*res;
	int fd, newfd,ret;
	ssize_t n,nw;
	struct sockaddr_in addr;
	socklen_t addrlen;
	char *ptr,buffer[128];
	pid_t pid;
	memset(&act,0,sizeof act);
	act.sa_handler=SIG_IGN;
	if(sigaction(SICHLD,&act,NULL)==-1)/*error*/exit(1);

	if((fd=socket(AF_INET,SOCK_STREAM,0))==-1)exit(1);//error
	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_INET;//IPv4
	hints.ai_socktype=SOCK_STREAM;//TCP socket
	hints.ai_flags=AI_PASSIVE;
	if((ret=getaddrinfo(NULL,"58001",&hints,&res))!=0)/*error*/exit(1);
	if(bind(fd,res->ai_addr,res->ai_addrlen)==-1)/*error*/exit(1);
	if(listen(fd,5)==-1)/*error*/exit(1);
	freeaddrinfo(res);//frees the memory allocated by getaddrinfo (no longer needed)

	while(1){addrlen=sizeof(addr);
		do newfd=accept(fd,(struct sockaddr*)&addr,&addrlen);//wait for a connection
		while(newfd==-1&&errno==EINTR);
		if(newfd==-1)/*error*/exit(1);

		if((pid=fork())==-1)/*error*/exit(1);
		else if(pid==0)//child process
		{close(fd);
			while((n=read(newfd,buffer,128))!=0){if(n==-1)/*error*/exit(1);
				ptr=&buffer[0];
				while(n>0){if((nw=write(newfd,ptr,n))<=0)/*error*/exit(1);
					n-=nw; ptr+=nw;}
			}
			close(newfd); exit(0);}
		//parent process
		do ret=close(newfd);while(ret==-1&&errno==EINTR);
		if(ret==-1)/*error*/exit(1);
	}
	/*close(fd);exit(0);*/
}
