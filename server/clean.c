#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define max(A,B) (A >= B ? A : B)

#define PORT "58001"

int main(void) {
	struct sigaction act;
	struct addrinfo hints, *res;
	struct sockaddr_in addr;
	int errcode, tcpfd, udpfd, newfd, maxfd, counter;
	char *buffer, *ptr;
	ssize_t n, nw;
	socklen_t addrlen;
	fd_set mask;

	memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	if (sigaction(SIGPIPE, &act, NULL) == -1) exit(1);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = 0;
	hints.ai_flags = AI_PASSIVE;
	if ((errcode = getaddrinfo(NULL, PORT, &hints, &res)) != 0) exit(1);

	// TCP server setup
	if ((tcpfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) exit(1);
	if (bind(tcpfd, res->ai_addr, res->ai_addrlen) == -1) exit(1);
	if (listen(tcpfd, 5) == -1) exit(1);

	// UDP server setup
	if ((udpfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) exit(1);
	if (bind(udpfd, res->ai_addr, res->ai_addrlen) == -1) exit(1);

	// Set mask and maxfd to select
	FD_ZERO(&mask);
	maxfd = max(tcpfd, udpfd) + 1;

	while (1) {
		FD_SET(tcpfd, &mask);
		FD_SET(udpfd, &mask);

		counter = select(maxfd, &mask, (fd_set*)NULL, (fd_set*)NULL, (struct
					timeval*)NULL);
		if (counter <= 0)
			exit(1);

		if (FD_ISSET(tcpfd, &mask)) {
			write(1, "TCPPPPPPP\n", strlen("TCPPPPPPP\n"));
			addrlen = sizeof(addr);
			if ((newfd = accept(tcpfd, (struct sockaddr*)&addr, &addrlen)) == -1)
				exit(1);
			while ((n = read(newfd, buffer, 128)) != 0) {
				if (n == -1) exit(1);
				ptr = &buffer[0];
				write(1, "received: ", 10);
				write(1, ptr, n);
				while (n > 0) {
					if ((nw = write(newfd, ptr, n)) == -1) exit(1);
					n -= nw;
					ptr += nw;
				}
			}
			close(newfd);
		}

		if (FD_ISSET(udpfd, &mask)) {
			write(1, "UDPPPPPPP\n", strlen("UDPPPPPPP\n"));
			addrlen = sizeof(addr);
			n = recvfrom(udpfd, buffer, 128, 0, (struct sockaddr*)&addr, &addrlen);
			if (n == -1) /* error */ exit(1);
			write(1, "received: ", 10);
			write(1, buffer, n);

			n = sendto(udpfd, buffer, n, 0, (struct sockaddr*)&addr, addrlen);
			if (n == -1) /* error */ exit(1);
		}
	}

	close(tcpfd);
	close(udpfd);
	freeaddrinfo(res);
	exit(0);
}
