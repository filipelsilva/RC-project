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

#define PORT "58013"

int main(int argc, char** argv) {
	const char* usage = "Usage: %s [-p DSport] [-v]\n"
		"\t-p PORT\t\tPort where the DS server accepts requests\n"
		"\t-v\t\tVerbose mode: outputs description of the received requests\n";

	// Default initialization of variables and flags
	int tmp, DSport = atoi(PORT), verbose = 0;
	char flag;

	// Argument parser
	while ((flag = getopt(argc, argv, "p:v")) != -1) {
		switch (flag) {
			case 'p':
				if ((tmp = strtol(optarg, NULL, 10)) == 0) {
					fprintf(stderr, "Error: invalid value for -p flag\n");
					fprintf(stderr, usage, argv[0]);
					exit(1);
				}
				DSport = tmp;
				break;

			case 'v':
				verbose = 1;
				break;

			default:
				fprintf(stderr, usage, argv[0]);
				exit(1);
				break;
		}
	}

	printf("DSport: %d\nVerbose mode: %d\n", DSport, verbose);

	int maxfd, counter;
	fd_set mask;
	TCPServer tcp = TCPServer(PORT);
	UDPServer udp = UDPServer(PORT);
	const char *message;

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
