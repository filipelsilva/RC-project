#ifndef SERVER
#define SERVER

#include "../Constants.hpp"

// TODO: ver das funções gai_strerror e strerror, porque posso-me ter enganado
// e posto em tudo quando deveria apenas ser numa delas
class Server {
	protected:
		struct sigaction act;
		struct addrinfo hints, *res;
		int newfd, errcode;
		ssize_t n, nw;
		struct sockaddr_in addr;
		socklen_t addrlen;
		char *ptr, buffer[COMMAND_SIZE], host[NI_MAXHOST], service[NI_MAXSERV];
		const char *port;

	public:
		int fd;

		void printVerbose() {
			if ((errcode = getnameinfo((struct sockaddr *)&addr, addrlen, host,
							sizeof(host), service, sizeof(service), 0)) != 0) {
				fprintf(stderr, "error: getnameinfo: %s\n", gai_strerror(errcode));
				exit(1);
			}
			else {
				printf("%s:%s -> ", host, service);
			}
		}

		virtual char *getData() = 0;
		virtual void sendData(const char *message) = 0;
		virtual ~Server() {};
};

#endif
