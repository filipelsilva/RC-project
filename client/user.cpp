#include <cstdio>
#include <cstdlib>
extern "C" { // For C libraries, to avoid namespace cluttering
#include <unistd.h>
}
#include "TCPClient.cpp"
#include "UDPClient.cpp"

#define DSPORT_DEFAULT 58013

int main(int argc, char** argv) {
	const char* usage = "Usage: %s [-n DSIP] [-p DSport]\n"
		"\t-n DSIP\t\tIP address of the machine where the DS is running\n"
		"\t-p PORT\t\tPort where the DS server accepts requests\n";

	// Default initialization of variables and flags
	const char* DSIP = "localhost";
	int tmp, DSport = DSPORT_DEFAULT;
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
				DSport = tmp;
				break;

			default:
				fprintf(stderr, usage, argv[0]);
				exit(1);
				break;
		}
	}

	printf("DSIP: %s\nDSport: %d\n", DSIP, DSport);

	while(1){	
		TCPClient tcp = TCPClient("localhost", "58013");
		UDPClient udp = UDPClient("localhost", "58013");
		tcp.sendData("Teste de servidor TCP\n");
		udp.sendData("Teste de servidor UDP\n");
	}

	return 0;
}
