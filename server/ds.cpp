#include "../Constants.hpp"
#include "./TCPServer.cpp"
#include "./UDPServer.cpp"
#include "./requests.cpp"

using namespace std;

string functionCaller(string command){
	string cmd = command.substr(0, 3);
	if(cmd.compare("REG") == 0)
		return reg(command);
	if(cmd.compare("UNR") == 0)
		return unr(command);
	if(cmd.compare("LOG") == 0)
		return log(command);
	if(cmd.compare("OUT") == 0)
		return out(command);
	if(cmd.compare("GLS") == 0)
		return gls(command);
	if(cmd.compare("GSR") == 0)
		return gsr(command);
	if(cmd.compare("GUR") == 0)
		return gur(command);
	if(cmd.compare("GLM") == 0)
		return glm(command);
	if(cmd.compare("PST") == 0)
		return pst(command);
	if(cmd.compare("RTV") == 0)
		return rtv(command);
}

int main(int argc, char** argv) {
	const char* usage = "Usage: %s [-p DSport] [-v]\n"
		"\t-p PORT\t\tPort where the DS server accepts requests\n"
		"\t-v\t\tVerbose mode: outputs description of the received requests\n";

	// Default initialization of variables and flags
	int verbose = 0;
	char flag;
	const char *DSport = DSPORT_DEFAULT;

	// Argument parser
	while ((flag = getopt(argc, argv, "p:v")) != -1) {
		switch (flag) {
			case 'p':
				if (strtol(optarg, NULL, 10) == 0) {
					fprintf(stderr, "Error: invalid value for -p flag\n");
					fprintf(stderr, usage, argv[0]);
					exit(1);
				}
				DSport = optarg;
				break;

			case 'v':
				verbose = 1;
				break;

			default:
				fprintf(stderr, usage, argv[0]);
				exit(1);
				break;
		}
	}

	printf("DSport: %s\nVerbose mode: %d\n", DSport, verbose);

	int maxfd, counter;
	fd_set mask;
	TCPServer tcp = TCPServer(DSport);
	UDPServer udp = UDPServer(DSport);

	// Set mask and maxfd to select
	FD_ZERO(&mask);
	maxfd = max(tcp.fd, udp.fd) + 1;

	string command;

	while (1) {
		const char *request = "";
		string reply;
		FD_SET(tcp.fd, &mask);
		FD_SET(udp.fd, &mask);

		if ((counter = select(maxfd, &mask, (fd_set*)NULL, (fd_set*)NULL,
						(struct timeval*)NULL)) <= 0) {
			fprintf(stderr, "Error: socket: %s\n", gai_strerror(counter));
			exit(1);
		}

		if (FD_ISSET(tcp.fd, &mask)) {
			request = tcp.getData();
		}

		if (FD_ISSET(udp.fd, &mask)) {
			request = udp.getData();
		}

		write(1, "INSIDE SERVER: ", strlen("INSIDE SERVER: "));
		write(1, request, strlen(request));

		reply = functionCaller(command.assign(request));
	}

	exit(0);
}
