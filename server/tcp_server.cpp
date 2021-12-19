#include <cstdlib>
#include <netdb.h>
#include <cstring>
#include <csignal>
extern "C" {
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
}

#define PORT "58001"

class TCPServer {
	struct sigaction act;
	struct addrinfo hints, *res;
	int fd, newfd, errcode;
	ssize_t n, nw;
	struct sockaddr_in addr;
	socklen_t addrlen;
	char *ptr, buffer[128];

	public:
	void setupServer() {
		// Handling of SIGPIPE signal
		memset(&act, 0, sizeof(act));
		act.sa_handler = SIG_IGN;
		if (sigaction(SIGPIPE, &act, NULL) == -1) exit(1);

		// Server setup
		if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) exit(1);

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;

		if ((errcode = getaddrinfo(NULL, PORT, &hints, &res)) != 0) exit(1);
		if (bind(fd, res->ai_addr, res->ai_addrlen) == -1) exit(1);
		if (listen(fd, 5) == -1) exit(1);
	}

	void getData() {
		while (1) {
			addrlen = sizeof(addr);
			if ((newfd = accept(fd, (struct sockaddr*)&addr, &addrlen)) == -1)
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
		freeaddrinfo(res);
		close(fd);
	}
};
