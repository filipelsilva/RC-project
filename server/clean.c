#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define max(A,B) (A >= B ? A : B)

#define PORT "58001"

int main(void)
{
	int errcode, tcpfd, udpfd, newfd, maxfd, counter, afd = 0;
	ssize_t n;
	char *buffer;
	enum {idle, busy} state;
	struct addrinfo tcphints, udphints, *tcpres, *udpres;
	struct sockaddr_in addr;
	socklen_t addrlen;
	fd_set mask;

	// TCP server setup
	if ((tcpfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) exit(1);

	memset(&tcphints, 0, sizeof(tcphints));
	tcphints.ai_family = AF_INET;
	tcphints.ai_socktype = SOCK_STREAM;
	tcphints.ai_flags = AI_PASSIVE;

	if ((errcode = getaddrinfo(NULL, PORT, &tcphints, &tcpres)) != 0) exit(1);
	if (bind(tcpfd, tcpres->ai_addr, tcpres->ai_addrlen) == -1) exit(1);
	if (listen(tcpfd, 5) == -1) exit(1);

	// UDP server setup
	if ((udpfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) exit(1);

	memset(&udphints, 0, sizeof(udphints));
	udphints.ai_family = AF_INET;
	udphints.ai_socktype = SOCK_DGRAM;
	udphints.ai_flags = AI_PASSIVE;

	if ((errcode = getaddrinfo(NULL, PORT, &udphints, &udpres)) != 0) exit(1);
	if (bind(udpfd, udpres->ai_addr, udpres->ai_addrlen) == -1) exit(1);

	state = idle;
	while (1) {
		FD_ZERO(&mask);
		FD_SET(tcpfd, &mask);
		FD_SET(udpfd, &mask);

		// maxfd = fd;
		// if (state == busy) {
		// 	FD_SET(afd, &mask); 
		// 	maxfd = max(maxfd, afd);
		// }

		write(1, "Espera\n", strlen("Espera\n"));
		counter = select(maxfd+1, &mask, (fd_set*)NULL, (fd_set*)NULL, (struct
					timeval*)NULL);
		write(1, "Espera acabou\n", strlen("Espera acabou\n"));
		if (counter <= 0)
			exit(1);

		if (FD_ISSET(tcpfd, &mask)) {
			addrlen = sizeof(addr);
			if ((newfd = accept(fd, (struct sockaddr*)&addr, &addrlen)) == -1)
				exit(1);
			while ((n = read(newfd, buffer, 128)) != 0) {
				if (n == -1) exit(1);
				ptr = &buffer[0];
				write(1, "received: ", 10);
				write(1, ptr, n);
				while (n > 0) {
					if ((nw = write(newfd, ptr, n)) == -1) exit(1);
					n -= nw;
					ptr += nw;
				}
			}
			close(newfd);

			// switch (state) {
			// 	case idle: 
			// 		afd = newfd;
			// 		state = busy;
			// 		break;
			//
			// 	case busy: /* ... *///write “busy\n” in newfd
			// 		close(newfd);
			// 		break;
			// }
		}
		if (FD_ISSET(udpfd,&mask)) {
			addrlen = sizeof(addr);
			n = recvfrom(udpfd, buffer, 128, 0, (struct sockaddr*)&addr, &addrlen);
			if (n == -1) /* error */ exit(1);
			write(1, "received: ", 10); write(1, buffer, n);

			n = sendto(udpfd, buffer, n, 0, (struct sockaddr*)&addr, addrlen);
			if (n == -1) /* error */ exit(1);
		}

		// if (FD_ISSET(afd, &mask)) {
		// 	if ((n = read(afd, buffer, 128)) != 0) {
		// 		if(n == -1)
		// 			exit(1);
		// 		/* ... *///write buffer in afd
		// 	}
		// 	else {
		// 		close(afd);
		// 		state = idle;
		// 	}//connection closed by peer
		// }
	}
	close(udpfd);

	exit(0);
}
