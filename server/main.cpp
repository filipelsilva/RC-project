#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
extern "C" {
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
}

#include "../server/TCPServer.cpp"
#include "../server/UDPServer.cpp"

#define max(A,B) (A >= B ? A : B)

#define PORT "58001"

int main() {
	int maxfd, counter;
	fd_set mask;
	TCPServer tcp = TCPServer(PORT);
	UDPServer udp = UDPServer(PORT);
	char *message;

	// Set mask and maxfd to select
	FD_ZERO(&mask);
	maxfd = max(tcp.fd, udp.fd) + 1;

	while (1) {
		FD_SET(tcp.fd, &mask);
		FD_SET(udp.fd, &mask);

		if ((counter = select(maxfd, &mask, (fd_set*)NULL, (fd_set*)NULL,
						(struct timeval*)NULL)) <= 0) {
			fprintf(stderr, "Error: socket: %s\n", gai_strerror(counter));
			exit(1);
		}

		if (FD_ISSET(tcp.fd, &mask)) {
			message = tcp.getData();
		}

		if (FD_ISSET(udp.fd, &mask)) {
			message = udp.getData();
		}

		write(1, "INSIDE SERVER: ", strlen("INSIDE SERVER: "));
		write(1, message, strlen(message));
	}

	exit(0);
}
