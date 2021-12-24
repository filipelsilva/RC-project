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

class TCPClient {
	struct sigaction act;
	struct addrinfo hints, *res;
	int fd, n, errcode;
	ssize_t nbytes, nleft, nwritten, nread;
	char *ptr, buffer[128];
	const char *server, *port;

	public:
	TCPClient(const char *server, const char *port) {
		// Handling of SIGPIPE signal
		memset(&act, 0, sizeof(act));
		act.sa_handler = SIG_IGN;
		if ((errcode = sigaction(SIGPIPE, &act, NULL)) == -1) {
			fprintf(stderr, "Error: sigaction: %s\n", gai_strerror(errcode));
			exit(1);
		}

		// Server setup
		this->server = server;
		this->port = port;

		if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			fprintf(stderr, "Error: socket: %s\n", gai_strerror(fd));
			exit(1);
		}

		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;

		n = getaddrinfo("localhost", "58001", &hints, &res);
		if (n != 0) exit(1);
		n = connect(fd, res->ai_addr, res->ai_addrlen);
		if (n == -1) exit(1);
	}

	void sendData(const char *message) {
		ptr = strcpy(buffer, message);
		nbytes = strlen(message);

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
	}

	~TCPClient() {
		freeaddrinfo(res);
		close(fd);
	}
};

int main(int argc, char **argv) {
	TCPClient client = TCPClient("localhost", "58001");
	client.sendData("Teste de servidor TCP\n");
}
