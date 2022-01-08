#ifndef SERVER
#define SERVER

#include "../Common.hpp"

class Server {
	protected:
		struct sigaction act;
		struct addrinfo hints, *res;
		int newfd, errcode, verbose;
		ssize_t n, nw;
		struct sockaddr_in addr;
		socklen_t addrlen;
		char *ptr, buffer[COMMAND_SIZE], host[NI_MAXHOST], service[NI_MAXSERV];
		const char *port;

	public:
		int fd;

		void printPrompt(int verbose) {
			if ((errcode = getnameinfo((struct sockaddr *)&addr, addrlen, host,
							sizeof(host), service, sizeof(service), 0)) != 0) {
				fprintf(stderr, "error: getnameinfo: %s\n", gai_strerror(errcode));
				exit(1);
			}
			else {
				if (verbose) {
					write(1, host, strlen(host));
					write(1, ":", strlen(":"));
					write(1, service, strlen(service));
					write(1, " ", strlen(" "));
				}
				write(1, "> ", strlen("> "));
			}
		}

<<<<<<< HEAD
		virtual char *getData() = 0;
=======
		virtual char *getData(size_t size) = 0;
>>>>>>> 1854f70075b463041e413514d5a120b2c67e2567
		virtual void sendData(const char *message, size_t size) = 0;
		virtual ~Server() {};
};

#endif
