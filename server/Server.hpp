#ifndef SERVER
#define SERVER

#include "../Constants.hpp"

class Server {
	protected:
		struct sigaction act;
		struct addrinfo hints, *res;
		int newfd, errcode;
		ssize_t n, nw;
		struct sockaddr_in addr;
		socklen_t addrlen;
		char *ptr, buffer[COMMAND_SIZE];
		const char *port;

	public:
		int fd;

		virtual char *getData() = 0;
		virtual void sendData(const char *message) = 0;
		virtual ~Server() {};
};

#endif
