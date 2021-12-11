#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>

int main(void) {
	struct sigaction act;
	struct addrinfo hints, *res;
	int fd, n;
	ssize_t nbytes, nleft, nwritten, nread;
	char *ptr, buffer[128];

	memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	if (sigaction(SIGPIPE, &act, NULL) == -1) exit(1);

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) exit(1);
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	n = getaddrinfo("localhost", "58001", &hints, &res);
	if (n != 0) exit(1);
	n = connect(fd, res->ai_addr, res->ai_addrlen);
	if (n == -1) exit(1);

	ptr = strcpy(buffer, "Hello!\n");
	nbytes = 7;

	nleft = nbytes;
	while (nleft > 0) {
		nwritten = write(fd, ptr, nleft);
		if (nwritten <= 0) exit(1);
		nleft -= nwritten;
		ptr += nwritten;
	}
	nleft = nbytes;
	ptr = buffer;

	while (nleft > 0){
		nread = read(fd, ptr, nleft);
		if(nread == -1) exit(1);
		else if (nread == 0) break;
		nleft -= nread;
		ptr += nread;
	}

	nread = nbytes - nleft;

	write(1, "echo: ", 6);
	write(1, buffer, nread);

	freeaddrinfo(res);
	close(fd);
}
