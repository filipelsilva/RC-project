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
		timerOn(fd);
		memset(buffer, 0, sizeof(buffer));
		int tries = 0;

		ptr = buffer;
		while ((nread = read(fd, ptr, size)) > 0){
			while (nread == -1) {
				tries++;
				if (tries == 3) {
					fprintf(stderr, "Error: read: %s\n", strerror(nread));
					exit(1);
				}
				nread = read(fd, ptr, size);
			}
			tries = 0;
			ptr += nread;
		}

		timerOff(fd);
		return buffer;
	}

	char *getDataRetrieve(ssize_t size) {
		timerOn(fd);
		memset(buffer, 0, sizeof(buffer));

		int tries = 0;
		ptr = buffer;
		while ((nread = read(fd, ptr, size)) != 0 && nread < size){
			while (nread == -1) {
				tries++;
				if (tries == 3) {
					fprintf(stderr, "Error: read: %s\n", strerror(nread));
					exit(1);
				}
				nread = read(fd, ptr, size);
			}
			tries = 0;
			ptr += nread;
		}
		ptr = &buffer[0];
		timerOff(fd);
		return buffer;
	}

	void getFileData(string path, size_t size) {
		timerOn(fd);
		memset(buffer, 0, sizeof(buffer));
		size_t written = 0;
		ofstream file(path, std::ios_base::binary);
		int to_read = COMMAND_SIZE, tries = 0;
		if(size < COMMAND_SIZE){
			to_read = size;
		}
		while ((n = read(fd, buffer, to_read)) != 0 && written < size) {
			while (n == -1) {
				tries++;
				if (tries == 3) {
					fprintf(stderr, "Error: read: %s\n", strerror(n));
					exit(1);
				}
				n = read(fd, buffer, to_read);
			}
			tries = 0;
			written += n;
			ptr = &buffer[0];
			file.write(buffer, n);
			memset(buffer, 0, sizeof(buffer));
			to_read = size - written;
			if(to_read > COMMAND_SIZE){
				to_read = COMMAND_SIZE;
			}
		}
		ptr = &buffer[0];
		file.write(buffer, n);
		file.close();
		timerOff(fd);
	}


	void sendData(const char *message, size_t size) {
		int tries = 0;
		timerOn(fd);
		nleft = size;
		while (nleft > 0) {
			while ((nwritten = write(fd, message, nleft)) <= 0) {
				tries++;
				if (tries == 3) {
					fprintf(stderr, "Error: write: %s\n", strerror(nwritten));
					exit(1);
				}
				nwritten = write(fd, message, nleft);
			}
			tries = 0;
			nleft -= nwritten;
			message += nwritten;
		}
		timerOff(fd);
	}

	~TCPClient() {
		freeaddrinfo(res);
		close(fd);
	}
};
