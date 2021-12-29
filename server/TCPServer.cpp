#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
extern "C" {
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
}

class TCPServer {
	public:
	struct sigaction act;
	struct addrinfo hints, *res;
	int fd, newfd, errcode;
	ssize_t n, nw;
	struct sockaddr_in addr;
	socklen_t addrlen;
	char *ptr, buffer[128];
	const char *port;

	TCPServer(const char *port) {
		// Handling of SIGPIPE signal
		memset(&act, 0, sizeof(act));
		act.sa_handler = SIG_IGN;
		if ((errcode = sigaction(SIGPIPE, &act, NULL)) == -1) {
			fprintf(stderr, "Error: sigaction: %s\n", gai_strerror(errcode));
			exit(1);
		}

		// Server setup
		this->port = port;

		if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			fprintf(stderr, "Error: socket: %s\n", gai_strerror(fd));
			exit(1);
		}

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;

		if ((errcode = getaddrinfo(NULL, port, &hints, &res)) != 0) {
			fprintf(stderr, "Error: getaddrinfo: %s\n", gai_strerror(errcode));
			exit(1);
		}

		if ((n = bind(fd, res->ai_addr, res->ai_addrlen)) == -1) {
			fprintf(stderr, "Error: bind: %s\n", gai_strerror(n));
			exit(1);
		}

		if ((errcode = listen(fd, 5)) == -1) {
			fprintf(stderr, "Error: bind: %s\n", gai_strerror(n));
			exit(1);
		}
	}

	char *getData() {
		addrlen = sizeof(addr);

		if ((newfd = accept(fd, (struct sockaddr*)&addr, &addrlen)) == -1) {
			fprintf(stderr, "Error: accept: %s\n", gai_strerror(newfd));
			exit(1);
		}

		while ((n = read(newfd, buffer, 128)) != 0) {
			if (n == -1) {
				fprintf(stderr, "Error: read: %s\n", gai_strerror(n));
				exit(1);
			}

			ptr = &buffer[0];

			write(1, "received: ", 10);
			write(1, ptr, n);

			while (n > 0) {
				if ((nw = write(newfd, ptr, n)) == -1) {
					fprintf(stderr, "Error: write %s\n", gai_strerror(nw));
					exit(1);
				}
				n -= nw;
				ptr += nw;
			}
		}

		close(newfd);
		return buffer;
	}

	~TCPServer() {
		freeaddrinfo(res);
		close(fd);
	}
};

int main(int argc, char **argv) {
	TCPServer server = TCPServer("58001");
	char *message = server.getData();
}
