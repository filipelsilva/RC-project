#include "./Server.hpp"

class UDPServer : public Server {
	public:
		UDPServer(const char *port, int verbose) {
			// Server setup
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

		char *getData(size_t size) {
			int tries = 0;
			timerOn(fd);
			addrlen = sizeof(addr);
			memset(buffer, 0, sizeof(buffer));

			while ((n = recvfrom(fd, buffer, size, 0, (struct sockaddr*)&addr, &addrlen)) == -1) {
				tries++;
				if (tries == 3) {
					fprintf(stderr, "Error: recvfrom: %s\n", strerror(n));
					exit(1);
				}
			}
			tries = 0;

			printPrompt();
			write(1, buffer, n);
			timerOff(fd);

			return buffer;
		}

		void sendData(const char *message, size_t size) {
			int tries = 0;
			timerOn(fd);
			while ((n = sendto(fd, message, size, 0, (struct sockaddr*)&addr, addrlen)) == -1) {
				tries++;
				if (tries == 3) {
					fprintf(stderr, "Error: sendto: %s\n", strerror(n));
					exit(1);
				}
			}
			tries = 0;
			timerOff(fd);
		}

		~UDPServer() {
			freeaddrinfo(res);
			close(fd);
		}
};
