#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define DSPORT_DEFAULT 58013

int main(int argc, char** argv) {
	const char* usage = "Usage: %s [-p DSport] [-v]\n"
		"\t-p PORT\t\tPort where the DS server accepts requests\n"
		"\t-v\t\tVerbose mode: outputs description of the received requests\n";

	// Default initialization of variables and flags
	const char* DSip = "localhost";
	int tmp, DSport = DSPORT_DEFAULT, verbose = 0;
	char flag;

	// Argument parser
	while ((flag = getopt(argc, argv, "p:v")) != -1) {
		switch (flag) {
			case 'p':
				if ((tmp = strtol(optarg, NULL, 10)) == 0) {
					fprintf(stderr, "Error: invalid value for -p flag\n");
					fprintf(stderr, usage, argv[0]);
					exit(1);
				}
				DSport = tmp;
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

	printf("DSport: %d\nVerbose mode: %d\n", DSport, verbose);

	return 0;
}
