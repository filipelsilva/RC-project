#include "../Constants.hpp"
#include "./TCPServer.cpp"
#include "./UDPServer.cpp"

const char *processResponse(const char *message) {
	int index;
	string tmp;
	tmp = tmp.assign(message).substr(0, 3);

	auto location = find(UserDS_TCP.begin(), UserDS_TCP.end(), tmp);
	if (location != UserDS_TCP.end()) {
		index = location - UserDS_TCP.begin();
		return UserDS_TCP_response[index].c_str();
	}
	else {
		auto location = find(UserDS_UDP.begin(), UserDS_UDP.end(), tmp.assign(message));
		if (location != UserDS_UDP.end()) {
			index = location - UserDS_UDP.begin();
			return UserDS_UDP_response[index].c_str();
		}
		else {
			fprintf(stderr, "Error: invalid command\n");
			return NULL;
		}
	}
}

int main() {
	for (auto str : UserDS_UDP) {
		const char *response = processResponse(str.c_str());
		printf("%s\n", response);
	}
	int maxfd, counter;
	fd_set mask;
	TCPServer tcp = TCPServer(DSPORT_DEFAULT);
	UDPServer udp = UDPServer(DSPORT_DEFAULT);
	char *message, *code;
	const char *response;

	// Set mask and maxfd to select
	FD_ZERO(&mask);
	maxfd = max(tcp.fd, udp.fd) + 1;

	while (1) {
		FD_SET(tcp.fd, &mask);
		FD_SET(udp.fd, &mask);

		if ((counter = select(maxfd, &mask, (fd_set*)NULL, (fd_set*)NULL,
						(struct timeval*)NULL)) <= 0) {
			fprintf(stderr, "Error: socket: %s\n", gai_strerror(counter));
			exit(1);
		}

		if (FD_ISSET(tcp.fd, &mask)) {
			message = tcp.getData();
			tcp.sendData(message);
			// memcpy(code, message, 3);
			// if ((response = processResponse(code)) != NULL)
			// 	tcp.sendData(response);
			// else
			// 	tcp.sendData("Invalid command\n");
		}

		if (FD_ISSET(udp.fd, &mask)) {
			message = udp.getData();
			udp.sendData(message);
			// memcpy(code, message, 3);
			// if ((response = processResponse(code)) != NULL)
			// 	udp.sendData(response);
			// else
			// 	udp.sendData("Invalid command\n");
		}

		write(1, "INSIDE SERVER: ", strlen("INSIDE SERVER: "));
		write(1, message, strlen(message));
	}

	exit(0);
}
