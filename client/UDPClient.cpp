#include "./Client.hpp"

class UDPClient : public Client {
	public:
	UDPClient(const char *server, const char *port) {
		this->server = server;
		this->port = port;

		if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
			fprintf(stderr, "Error: socket: %s\n", strerror(fd));
			exit(1);
		}

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_DGRAM;

		if ((errcode = getaddrinfo(server, port, &hints, &res)) != 0) {
			fprintf(stderr, "Error: getaddrinfo: %s\n", gai_strerror(errcode));
			exit(1);
		}
	}

	void sendData(const char *message, size_t size){
		if ((n = sendto(fd, message, size, 0, res->ai_addr,
						res->ai_addrlen)) == -1) {
			fprintf(stderr, "Error: sendto: %s\n", strerror(n));
			exit(1);
		}
	}

	char *getData() {
		memset(buffer, 0, sizeof(buffer));

		addrlen = sizeof(addr);
		if ((n = recvfrom(fd, buffer, COMMAND_SIZE, 0, (struct sockaddr*)&addr,
						&addrlen)) == -1) {
			fprintf(stderr, "Error: recvfrom: %s\n", strerror(n));
			exit(1);
		}

		//write(1, "Server: ", 8);
		//write(1, buffer, n);

		return buffer;
	}

	~UDPClient() {
		freeaddrinfo(res);
		close(fd);
	}
};
