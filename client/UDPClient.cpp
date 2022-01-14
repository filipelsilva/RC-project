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

	void closeConnection(){
		close(fd);
	}

	void sendData(const char *message, size_t size){
		int tries = 0;
		timerOn(fd);
		while ((n = sendto(fd, message, size, 0, res->ai_addr,
						res->ai_addrlen)) == -1) {
			tries++;
			if (tries == 3) {
				fprintf(stderr, "Error: sendto: %s\n", strerror(n));
				exit(1);
			}
		}
		tries = 0;
		timerOff(fd);
	}

	char *getData(size_t size) {
		int tries = 0;
		timerOn(fd);
		memset(buffer, 0, sizeof(buffer));

		addrlen = sizeof(addr);
		while ((n = recvfrom(fd, buffer, size, 0, (struct sockaddr*)&addr,
						&addrlen)) == -1) {
			tries++;
			if (tries == 3) {
				fprintf(stderr, "Error: recvfrom: %s\n", strerror(n));
				exit(1);
			}
		}

		//write(1, "Server: ", 8);
		//write(1, buffer, n);

		timerOff(fd);
		return buffer;
	}

	~UDPClient() {
		freeaddrinfo(res);
		close(fd);
	}
};
