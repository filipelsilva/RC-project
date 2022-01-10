#ifndef SERVER
#define SERVER

#include "../Common.hpp"
#include "./requests.cpp"

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
					if (!local_UID.empty()) {
						write(1, "[UID=", strlen("[UID="));
						write(1, local_UID.c_str(), local_UID.length());
						write(1, "]", strlen("]"));
					}
					if (!local_GID.empty()) {
						write(1, "[GID=", strlen("[UID="));
						write(1, local_GID.c_str(), local_GID.length());
						write(1, "]", strlen("]"));
					}
					write(1, host, strlen(host));
					write(1, ":", strlen(":"));
					write(1, service, strlen(service));
					write(1, " ", strlen(" "));
				}
				write(1, "> ", strlen("> "));
			}
		}

		virtual char *getData(size_t size) = 0;
		virtual void sendData(const char *message, size_t size) = 0;
		virtual ~Server() {};
};

#endif
