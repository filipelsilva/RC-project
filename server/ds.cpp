#include "../Common.hpp"
#include "./TCPServer.cpp"
#include "./UDPServer.cpp"
#include "./requests.cpp"

/*Calls the function to execute the command sent by the client(UDP).*/
string functionCaller(string command){
	stringstream ss;
	string cmd;
	ss << command;
	getline(ss, cmd, ' ');
	if(command.compare(cmd) == 0){
		command = remove_new_line(command);
	}
	cmd = remove_new_line(cmd);
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
	return "ERR\n";
}

/*Calls the function to execute the command sent by the client(TCP).*/
void functionCallerTCP(string command, TCPServer &tcp){
	if(command.compare("ULS") == 0){
		uls(tcp);
		return;
	}
	else if(command.compare("PST") == 0){
		pst(tcp);
		return;
	}
	else if(command.compare("RTV") == 0){
		rtv(tcp);
		return;
	}
	tcp.sendData("ERR\n", strlen("ERR\n"));
	return;
}

int main(int argc, char **argv) {
	const char* usage = "Usage: %s [-p DSport] [-v]\n"
		"\t-p PORT\t\tPort where the DS server accepts requests\n"
		"\t-v\t\tVerbose mode: outputs description of the received requests\n";

	// Default initialization of variables and flags
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
	TCPServer tcp = TCPServer(DSport, verbose);
	UDPServer udp = UDPServer(DSport, verbose);

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
			fprintf(stderr, "Error: socket: %s\n", strerror(counter));
			exit(1);
		}

		if (FD_ISSET(tcp.fd, &mask)) {
			tcp.acceptConnection();
			tcp.printPrompt();
			request = tcp.getData(3);
			functionCallerTCP(command.assign(request, 3), tcp);
			tcp.closeConnection();
		}

		if (FD_ISSET(udp.fd, &mask)) {
			request = udp.getData(COMMAND_SIZE);
			reply = functionCaller(command.assign(request));
			udp.sendData(reply.c_str(), reply.length());
		}
	}

	exit(0);
}
