#ifndef CLIENT
#define CLIENT

#include "../Common.hpp"

class Client {
	protected:
		struct sigaction act;
		int fd, errcode;
		ssize_t n, nbytes, nleft, nwritten, nread;
		socklen_t addrlen;
		struct addrinfo hints, *res;
		struct sockaddr_in addr;
		char *ptr, buffer[COMMAND_SIZE], host[NI_MAXHOST], service[NI_MAXSERV];
		const char *server, *port;

	public:
		virtual void sendData(const char *message) = 0;
		virtual ~Client() {};
};

#endif
