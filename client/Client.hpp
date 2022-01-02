#ifndef CLIENT
#define CLIENT

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

class Client {
	protected:
		struct sigaction act;
		int fd, errcode;
		ssize_t n, nbytes, nleft, nwritten, nread;
		socklen_t addrlen;
		struct addrinfo hints, *res;
		struct sockaddr_in addr;
		char *ptr, buffer[128], host[NI_MAXHOST], service[NI_MAXSERV];
		const char *server, *port;

	public:
		virtual void sendData(const char *message) = 0;
		virtual ~Client() {};
};

#endif
