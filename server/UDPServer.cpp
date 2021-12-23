#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
extern "C" {
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
}

class UDPServer {
	int fd, errcode;
	ssize_t n;
	socklen_t addrlen;
	struct addrinfo hints, *res;
	struct sockaddr_in addr;
	char buffer[128];
	const char *port, *message;

	public:
	UDPServer(const char *port) {
		this->port = port;

		if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
			fprintf(stderr, "Error: socket: %s\n", gai_strerror(fd));
			exit(1);
		}

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_flags = AI_PASSIVE;

		if ((errcode = getaddrinfo(NULL, port, &hints, &res)) != 0) {
			fprintf(stderr, "Error: getaddrinfo: %s\n", gai_strerror(errcode));
			exit(1);
		}

		if ((n = bind(fd, res->ai_addr, res->ai_addrlen)) == -1) {
			fprintf(stderr, "Error: bind: %s\n", gai_strerror(n));
			exit(1);
		}
	}

	void getData() {
		while (1) {
			addrlen = sizeof(addr);
			if ((n = recvfrom(fd, buffer, 128, 0, (struct sockaddr*)&addr, &addrlen)) == -1) {
				fprintf(stderr, "Error: recvfrom: %s\n", gai_strerror(n));
				exit(1);
			}

			write(1, "received: ", 10);
			write(1, buffer, n);

			if ((n = sendto(fd, buffer, n, 0, (struct sockaddr*)&addr, addrlen)) == -1) {
				fprintf(stderr, "Error: sendto: %s\n", gai_strerror(n));
				exit(1);
			}
		}
	}

	~UDPServer() {
		freeaddrinfo(res);
		close(fd);
	}
};

int main(int argc, char **argv) {
	UDPServer server = UDPServer("58001");
	server.getData();
}
