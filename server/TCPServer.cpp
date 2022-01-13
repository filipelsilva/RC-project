#include "./Server.hpp"

class TCPServer : public Server {
	public:
		int flag = 0;
		int written = 0;
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

		char *getData(size_t size) {
			memset(buffer, 0, sizeof(buffer));

			printPrompt(verbose);
			//write(1, "BEFORE IT READ\n", strlen("BEFORE IT READ\n"));
			while ((n = read(fdcopy, buffer, size)) != 0 && n != size) {
				if (n == -1) {
					fprintf(stderr, "Error: read: %s\n", strerror(n));
					exit(1);
				}
				ptr = &buffer[0];





				/*for (size_t i = 0; i < strlen(buffer); i++) {
					if (buffer[i] == '\n' && (i == strlen(buffer) - 1 || buffer[i+1] == '\0')) {
						flag = 1;
					}
				}
				if (flag) {
					flag = 0;
					break;
				}*/
				//write(1, "IT READ\n", strlen("IT READ\n"));
			}
			//write(1, buffer, n);
			// dup2(fdcopy, fdcopy);
			/*for(int i = 0; i < size; i++){
				if(buffer[i] == '\n')
 					cout << "\n\nTCPS:" << written <<  "\n\n";
 				written++;
			}*/
			// close(fdcopy);
			return buffer;
		}

		void sendData(const char *message, size_t size) {
			// addrlen = sizeof(addr);
			//
			// if ((newfd = accept(fd, (struct sockaddr*)&addr, &addrlen)) == -1) {
			// 	fprintf(stderr, "Error: accept: %s\n", strerror(newfd));
			// 	exit(1);
			// }

			// fdcopy = dup(newfd);
			write(1, "Message to send: ",  strlen("Message to send: "));
			write(1, message, size);
			n = size;
			while (n > 0) {
				//write(1, "BEFORE IT WROTE\n", strlen("BEFORE IT WROTE\n"));
				if ((nw = write(newfd, message, n)) == -1) {
					fprintf(stderr, "Error: write %s\n", strerror(nw));
					exit(1);
				}
				// for (char *i = ptr; i < ptr + nw; i++) {
				// 	if (*i == '\n' && (i == ptr + nw - 1 || *(i+1) == '\0')) {
				// 		flag = 1;
				// 	}
				// }
				// if (flag) {
				// 	flag = 0;
				// 	break;
				// }
				write(1, "IT WROTE\n", strlen("IT WROTE\n"));
				n -= nw;
				ptr += nw;
			}
			// close(newfd);
			// close(fdcopy);
			// write(1, "BEFORE DUP\n", strlen("BEFORE DUP\n"));
			// dup2(newfd, newfd);
			// write(1, "DUP\n", strlen("DUP\n"));
		}

		~TCPServer() {
			freeaddrinfo(res);
			close(fd);
		}
};
