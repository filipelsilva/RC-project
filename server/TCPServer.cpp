#include "./Server.hpp"

class TCPServer : public Server {
	public:
		int flag = 0;
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

		void acceptConnection() {
			addrlen = sizeof(addr);
			memset(buffer, 0, sizeof(buffer));

			if ((newfd = accept(fd, (struct sockaddr*)&addr, &addrlen)) == -1) {
				fprintf(stderr, "Error: accept: %s\n", strerror(newfd));
				exit(1);
			}
			fdcopy = dup(newfd);
		}

		void closeConnection() {
			close(newfd);
			close(fdcopy);
		}

		char *getData(ssize_t size) {
			int tries = 0;
			timerOn(fdcopy);
			memset(buffer, 0, sizeof(buffer));
			while ((n = read(fdcopy, buffer, size)) != 0 && n < size) {
				while (n == -1) {
					tries++;
					if (tries == 3) {
						fprintf(stderr, "Error: read: %s\n", strerror(n));
						exit(1);
					}
					n = read(fdcopy, buffer, size);
				}
				tries = 0;
			}
			ptr = &buffer[0];
			write(1, ptr, n);
			timerOff(fdcopy);
			return buffer;
		}

		void getFileData(string path, size_t size) {
			timerOn(fdcopy);
			memset(buffer, 0, sizeof(buffer));
			int tries = 0, to_read = COMMAND_SIZE;
			size_t written = 0;
			ofstream file(path, std::ios_base::binary);
			if(size < COMMAND_SIZE){
				to_read = size;
			}
			while ((n = read(fdcopy, buffer, to_read)) != 0 && written < size) {
				while (n == -1) {
					tries++;
					if (tries == 3) {
						fprintf(stderr, "Error: read: %s\n", strerror(n));
						exit(1);
					}
					n = read(fdcopy, buffer, size);
				}
				tries = 0;
				written += n;
				ptr = &buffer[0];
				file.write(buffer, n);
				write(1, ptr, n);
				memset(buffer, 0, sizeof(buffer));
				to_read = size - written;
				if(to_read > COMMAND_SIZE){
					to_read = COMMAND_SIZE;
				}
			}
			ptr = &buffer[0];
			file.write(buffer, n);
			write(1, ptr, n);
			file.close();
			timerOff(fdcopy);
		}

		void sendData(const char *message, size_t size) {
			timerOn(newfd);
			n = size;
			ptr = &message[0];
			int tries = 0;
			while (n > 0) {
				while ((nw = write(newfd, ptr, n)) == -1) {
					tries++;
					if (tries == 3) {
						fprintf(stderr, "Error: write: %s\n", strerror(nw));
						exit(1);
					}
					nw = write(newfd, message, n);
				}
				tries = 0;
				n -= nw;
				ptr += nw;
			}
			timerOff(newfd);
		}

		~TCPServer() {
			freeaddrinfo(res);
			close(fd);
		}
};
