#include "./Client.hpp"

class UDPClient : public Client {
	int fd, errcode;
	ssize_t n;
	socklen_t addrlen;
	struct addrinfo hints, *res;
	struct sockaddr_in addr;
	char buffer[COMMAND_SIZE], host[NI_MAXHOST], service[NI_MAXSERV];
	const char *server, *port;

	public:
	UDPClient(const char *server, const char *port) {
		this->server = server;
		this->port = port;

		if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
			fprintf(stderr, "Error: socket: %s\n", gai_strerror(fd));
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

	string getData(){
		return buffer;
	}

	void sendData(const char *message) {
		if ((n = sendto(fd, message, strlen(message), 0, res->ai_addr,
						res->ai_addrlen)) == -1) {
			fprintf(stderr, "Error: sendto: %s\n", gai_strerror(n));
			exit(1);
		}

		addrlen = sizeof(addr);

		if ((n = recvfrom(fd, buffer, COMMAND_SIZE, 0, (struct sockaddr*)&addr,
						&addrlen)) == -1) {
			fprintf(stderr, "Error: recvfrom: %s\n", gai_strerror(n));
			exit(1);
		}

		//write(1, "Server: ", 8);
		//write(1, buffer, n);

		if ((errcode = getnameinfo((struct sockaddr *)&addr, addrlen, host,
						sizeof(host), service, sizeof(service), 0)) != 0) {
			fprintf(stderr, "Error: getnameinfo: %s\n", gai_strerror(errcode));
			exit(1);
		}
		else {
			//printf("Sent by [%s:%s]\n", host, service);
		}
	}

	~UDPClient() {
		freeaddrinfo(res);
		close(fd);
	}
};
