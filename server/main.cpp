#include "../Constants.hpp"
#include "./TCPServer.cpp"
#include "./UDPServer.cpp"

int main() {
	int maxfd, counter;
	fd_set mask;
	TCPServer tcp = TCPServer(DSPORT_DEFAULT);
	UDPServer udp = UDPServer(DSPORT_DEFAULT);
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
			tcp.sendData(message);
		}

		if (FD_ISSET(udp.fd, &mask)) {
			message = udp.getData();
			udp.sendData(message);
		}

		write(1, "INSIDE SERVER: ", strlen("INSIDE SERVER: "));
		write(1, message, strlen(message));
	}

	exit(0);
}
