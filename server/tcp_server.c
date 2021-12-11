#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "58001"

int main(int argc, char **argv) {
	int fd, newfd, errcode;
	ssize_t n, nbytes, nleft, nwritten, nread;
	socklen_t addrlen;
	struct addrinfo hints, *res;
	struct sockaddr_in addr;
	char buffer[128];
	char* ptr;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) /* error */ exit(1);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags=AI_PASSIVE;

	errcode = getaddrinfo(NULL, PORT, &hints, &res);
	if (errcode != 0) /* error */ exit(1);

	n = bind(fd, res->ai_addr, res->ai_addrlen);
	if (n == -1) /* error */ exit(1);

	if (listen(fd, 5) == -1) /* error */ exit(1);

	while (1) {
		addrlen = sizeof(addr);
		if ((newfd = accept(fd, (struct sockaddr*)&addr, &addrlen)) == -1) /* error */ exit(1);

		while((n=read(newfd,buffer,128))!=0) {
			if(n==-1)/*error*/exit(1);
			write(1,"echo: ",6);//stdout
			write(1,buffer,n);
			ptr = buffer;
			/* ptr=&buffer[0]; */
			while(n>0) {
				if((nwritten=write(newfd,ptr,n))<=0)/*error*/exit(1);
				n-=nwritten; ptr+=nwritten;
			}
		}

		close(newfd);
	}

	freeaddrinfo(res);
	close(fd);
}

