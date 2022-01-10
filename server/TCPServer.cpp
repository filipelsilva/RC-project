#include "./Server.hpp"

class TCPServer : public Server {
	public:
		TCPServer(const char *port, int verbose) {
			// Handling of SIGPIPE signal
			memset(&act, 0, sizeof(act));
			act.sa_handler = SIG_IGN;
			if ((errcode = sigaction(SIGPIPE, &act, NULL)) == -1) {
				fprintf(stderr, "Error: sigaction: %s\n", strerror(errcode));
				exit(1);
			}

			// TCPServer setup
			this->verbose = verbose;
			this->port = port;

			if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
				fprintf(stderr, "Error: socket: %s\n", strerror(fd));
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
				fprintf(stderr, "Error: bind: %s\n", strerror(n));
				exit(1);
			}

			if ((errcode = listen(fd, 5)) == -1) {
				fprintf(stderr, "Error: listen: %s\n", strerror(n));
				exit(1);
			}
		}

		char *getData(size_t size) {
			addrlen = sizeof(addr);
			memset(buffer, 0, sizeof(buffer));

			if ((newfd = accept(fd, (struct sockaddr*)&addr, &addrlen)) == -1) {
				fprintf(stderr, "Error: accept: %s\n", strerror(newfd));
				exit(1);
			}

			while ((n = read(newfd, buffer, size)) != 0) {
				if (n == -1) {
					fprintf(stderr, "Error: read: %s\n", strerror(n));
					exit(1);
				}
				ptr = &buffer[0];

				printPrompt(verbose);
			}
			close(newfd);
			return buffer;
		}

		void printCommand() {
			write(1, ptr, n);
		}

		void sendData(const char *message, size_t size) {
			addrlen = sizeof(addr);

			if ((newfd = accept(fd, (struct sockaddr*)&addr, &addrlen)) == -1) {
				fprintf(stderr, "Error: accept: %s\n", strerror(newfd));
				exit(1);
			}

			n = size;
			while (n > 0) {
				if ((nw = write(newfd, message, n)) == -1) {
					fprintf(stderr, "Error: write %s\n", strerror(nw));
					exit(1);
				}
				n -= nw;
				ptr += nw;
			}
			close(newfd);
		}

		~TCPServer() {
			freeaddrinfo(res);
			close(fd);
		}
};
