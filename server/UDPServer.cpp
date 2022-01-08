#include "./Server.hpp"

class UDPServer : public Server {
	public:
		UDPServer(const char *port, int verbose) {
			this->verbose = verbose;
			this->port = port;

			if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
				fprintf(stderr, "Error: socket: %s\n", strerror(fd));
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
				fprintf(stderr, "Error: bind: %s\n", strerror(n));
				exit(1);
			}
		}

		char *getData() {
			addrlen = sizeof(addr);
			memset(buffer, 0, sizeof(buffer));

			if ((n = recvfrom(fd, buffer, COMMAND_SIZE, 0, (struct sockaddr*)&addr, &addrlen)) == -1) {
				fprintf(stderr, "Error: recvfrom: %s\n", strerror(n));
				exit(1);
			}

			printPrompt(verbose);
			write(1, buffer, n);

			return buffer;
		}

		void sendData(const char *message) {
			if ((n = sendto(fd, message, strlen(message), 0, (struct sockaddr*)&addr, addrlen)) == -1) {
				fprintf(stderr, "Error: sendto: %s\n", strerror(n));
				exit(1);
			}
		}

		~UDPServer() {
			freeaddrinfo(res);
			close(fd);
		}
};
