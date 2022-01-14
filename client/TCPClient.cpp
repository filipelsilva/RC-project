#include "./Client.hpp"

class TCPClient : public Client {
	public:
	TCPClient(const char *server, const char *port) {
		// Handling of SIGPIPE signal
		memset(&act, 0, sizeof(act));
		act.sa_handler = SIG_IGN;
		if ((errcode = sigaction(SIGPIPE, &act, NULL)) == -1) {
			fprintf(stderr, "Error: sigaction: %s\n", strerror(errcode));
			exit(1);
		}

		// Server setup
		this->server = server;
		this->port = port;

		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;

		if ((errcode = getaddrinfo(server, port, &hints, &res)) != 0) {
			fprintf(stderr, "Error: getaddrinfo: %s\n", gai_strerror(errcode));
			exit(1);
		}
	}

	void createSocketAndConnect() {
		if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			fprintf(stderr, "Error: socket: %s\n", strerror(fd));
			exit(1);
		}

		if ((errcode = connect(fd, res->ai_addr, res->ai_addrlen)) == -1) {
			fprintf(stderr, "Error: connect: %s\n", strerror(errcode));
			exit(1);
		}
	}

	char *getData(size_t size) {
		memset(buffer, 0, sizeof(buffer));

		ptr = buffer;
		while ((nread = read(fd, ptr, size)) > 0){
			if(nread == -1){

				fprintf(stderr, "Error: read: %s\n", strerror(nread));
				exit(1);
			}
			ptr += nread;
		}

		//write(1, "Server: ", 8);
		//write(1, buffer, nread);
		return buffer;
	}


	void sendData(const char *message, size_t size) {
		// createSocketAndConnect();

		nleft = size;
		while (nleft > 0) {
			nwritten = write(fd, message, nleft);
			if (nwritten <= 0){
				fprintf(stderr, "Error: write: %s\n", strerror(nwritten));
			 	exit(1);

			}
			nleft -= nwritten;
			message += nwritten;
		}
	}

	~TCPClient() {
		freeaddrinfo(res);
		close(fd);
	}
};
