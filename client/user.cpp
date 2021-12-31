#include <cstdio>
#include <cstdlib>
#include <string>
extern "C" { // For C libraries, to avoid namespace cluttering
#include <unistd.h>
}
#include "TCPClient.cpp"
#include "UDPClient.cpp"
#include "replies.cpp"
using namespace std;

#define DSPORT_DEFAULT "58013"
#define COMMAND_SIZE 128

bool isTCP(string command){
	command = command.substr(0, 3);
	string cmds[] = {"PST", "RPT", "RTV", "RRT"};
	for(string cmd: cmds){
		if(command.compare(cmd) == 0){
			return true;
		}
	}
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
	const char* DSIP = "localhost";
	const char* DSport = DSPORT_DEFAULT;
	int tmp;
	char flag;

	// Argument parser
	while ((flag = getopt(argc, argv, "n:p:")) != -1) {
		switch (flag) {
			case 'n':
				DSIP = optarg;
				break;

			case 'p':
				if ((tmp = strtol(optarg, NULL, 10)) == 0) {
					fprintf(stderr, "Error: invalid value for -p flag\n");
					fprintf(stderr, usage, argv[0]);
					exit(1);
				}
				DSport = to_string(tmp).c_str();
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
