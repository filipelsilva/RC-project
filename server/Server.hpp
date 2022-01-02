#ifndef SERVER
#define SERVER

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
extern "C" {
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
}

class Server {
	protected:
		struct sigaction act;
		struct addrinfo hints, *res;
		int newfd, errcode;
		ssize_t n, nw;
		struct sockaddr_in addr;
		socklen_t addrlen;
		char *ptr, buffer[128];
		const char *port;

	public:
		int fd;

		virtual const char *getData() = 0;
		virtual void sendData(const char *message) = 0;

		virtual ~Server() {
			freeaddrinfo(res);
			close(fd);
		}
};

#endif
