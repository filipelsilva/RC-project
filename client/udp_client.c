#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "58001"

int main(int argc, char **argv) {
	int fd, errcode;
	ssize_t n;
	socklen_t addrlen;
	struct addrinfo hints, *res;
	struct sockaddr_in addr;
	char buffer[128], host[NI_MAXHOST], service[NI_MAXSERV];
	char* server;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd == -1) /* error */ exit(1);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	server = "localhost";
	errcode = getaddrinfo(server, PORT, &hints, &res);
	if (errcode != 0) /* error */ exit(1);

	n = sendto(fd, "Hello!\n", 7, 0, res->ai_addr, res->ai_addrlen);
	if (n == -1) /* error */ exit(1);

	addrlen = sizeof(addr);
	n = recvfrom(fd, buffer, 128, 0, (struct sockaddr*)&addr, &addrlen);
	if (n == -1) /* error */ exit(1);

	write(1, "echo: ", 6); write(1, buffer, n);

	if((errcode = getnameinfo((struct sockaddr *)&addr, addrlen, host, sizeof(host), service, sizeof(service), 0))!=0)
		fprintf(stderr, "Error: getnameinfo: %s\n", gai_strerror(errcode));
	else
		printf("Sent by [%s:%s]\n", host, service);

	freeaddrinfo(res);
	close(fd);
}
