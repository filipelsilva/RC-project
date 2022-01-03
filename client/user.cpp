#include "../Constants.hpp"
#include "./TCPClient.cpp"
#include "./UDPClient.cpp"
#include "./replies.cpp"

bool isTCP(string command){
	for (string cmd: ClientUser_TCP)
		if (command.rfind(cmd, 0) != string::npos)
			return true;

	return false;
}

void functionCaller(string command){
	string cmd = command.substr(0, 3);
	if(cmd.compare("RRG") == 0)
		rrg(command);
	if(cmd.compare("RUN") == 0)
		run(command);
	if(cmd.compare("RLO") == 0)
		rlo(command);
	if(cmd.compare("ROU") == 0)
		rou(command);
	if(cmd.compare("RGL") == 0)
		rgl(command);
	if(cmd.compare("RGS") == 0)
		rgs(command);
	if(cmd.compare("RGU") == 0)
		rgu(command);
	if(cmd.compare("RGM") == 0)
		rgm(command);
	if(cmd.compare("RPT") == 0)
		rpt(command);
	/*
	if(cmd.compare("RRT") == 0)
		rrt(command);
	*/
}

int main(int argc, char** argv) {
	const char* usage = "Usage: %s [-n DSIP] [-p DSport]\n"
		"\t-n DSIP\t\tIP address of the machine where the DS is running\n"
		"\t-p PORT\t\tPort where the DS server accepts requests\n";

	// Default initialization of variables and flags
	const char* DSIP = DSIP_DEFAULT;
	const char* DSport = DSPORT_DEFAULT;
	char flag;

	// Argument parser
	while ((flag = getopt(argc, argv, "n:p:")) != -1) {
		switch (flag) {
			case 'n':
				DSIP = optarg;
				break;

			case 'p':
				if (strtol(optarg, NULL, 10) == 0) {
					fprintf(stderr, "Error: invalid value for -p flag\n");
					fprintf(stderr, usage, argv[0]);
					exit(1);
				}
				DSport = optarg;
				break;

			default:
				fprintf(stderr, usage, argv[0]);
				exit(1);
				break;
		}
	}

	printf("DSIP: %s\nDSport: %s\n", DSIP, DSport);

	while(1){
		write(1, "> ", strlen("> "));
		char command[COMMAND_SIZE];
		string cmd;
		if(fgets(command, COMMAND_SIZE, stdin) == NULL){
			fprintf(stderr, "Error: something went wrong while getting user input\n");
			exit(1);
		}

		if(isTCP(cmd.assign(command))){
			TCPClient tcp = TCPClient(DSIP, DSport);
			tcp.sendData(command);
		}
		else{
			UDPClient udp = UDPClient(DSIP, DSport);
			udp.sendData(command);
		}
	}

	return 0;
}
