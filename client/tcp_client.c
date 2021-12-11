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
	int fd, errcode;
	ssize_t n, nbytes, nleft, nwritten, nread;
	socklen_t addrlen;
	struct addrinfo hints, *res;
	struct sockaddr_in addr;
	char buffer[128];
	char *ptr, *server;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) /* error */ exit(1);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	server = "localhost";
	errcode = getaddrinfo(server, PORT, &hints, &res);
	if (errcode != 0) /* error */ exit(1);

	n = connect(fd, res->ai_addr, res->ai_addrlen);
	if (n == -1) /* error */ exit(1);

	ptr = strcpy(buffer, "Hello!\n");
	nbytes = strlen(ptr);
	nleft = nbytes;
	while (nleft > 0) {
		nwritten = write(fd, ptr, nleft);
		if (nwritten <= 0) /* error */ exit(1);
		nleft -= nwritten;
		ptr += nwritten;
	}

	nleft = nbytes; ptr=buffer;
	while (nleft > 0) {
		nread = read(fd, ptr, nleft);
		if (nread == -1) /* error */ exit(1);
		else if (nread == 0) break; // closed by peer
		nleft -= nread;
		ptr += nread;
	}
	nread = nbytes - nleft;

	write(1, "echo: ", 6); write(1, buffer, n);

	freeaddrinfo(res);
}
